/*
 * Copyright (c) 1982, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)ffs_balloc.c	8.8 (Berkeley) 06/16/95
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/proc.h>
#include <sys/file.h>
#include <sys/vnode.h>

#include <vm/vm.h>

#include <ufs/ufs/quota.h>
#include <ufs/ufs/inode.h>
#include <ufs/ufs/ufs_extern.h>

#include <ufs/ffs/fs.h>
#include <ufs/ffs/ffs_extern.h>

/*
 * Balloc defines the structure of file system storage
 * by allocating the physical blocks on a device given
 * the inode and the logical block number in a file.
 */
ffs_balloc(ip, lbn, size, cred, bpp, flags)
	register struct inode *ip;
	register ufs_daddr_t lbn;
	int size;
	struct ucred *cred;
	struct buf **bpp;
	int flags;
{
	register struct fs *fs;
	register ufs_daddr_t nb;
	struct buf *bp, *nbp;
	struct vnode *vp = ITOV(ip);
	struct indir indirs[NIADDR + 2];
	ufs_daddr_t newb, *bap, pref;
	int deallocated, osize, nsize, num, i, error;
	ufs_daddr_t *allocib, *blkp, *allocblk, allociblk[NIADDR + 1];

	*bpp = NULL;
	if (lbn < 0)
		return (EFBIG);
	fs = ip->i_fs;

	/*
	 * If the next write will extend the file into a new block,
	 * and the file is currently composed of a fragment
	 * this fragment has to be extended to be a full block.
	 */
	nb = lblkno(fs, ip->i_size);
	if (nb < NDADDR && nb < lbn) {
		osize = blksize(fs, ip, nb);
		if (osize < fs->fs_bsize && osize > 0) {
			error = ffs_realloccg(ip, nb,
				ffs_blkpref(ip, nb, (int)nb, &ip->i_db[0]),
				osize, (int)fs->fs_bsize, cred, &bp);
			if (error)
				return (error);
			ip->i_size = (nb + 1) * fs->fs_bsize;
			vnode_pager_setsize(vp, (u_long)ip->i_size);
			ip->i_db[nb] = dbtofsb(fs, bp->b_blkno);
			ip->i_flag |= IN_CHANGE | IN_UPDATE;
			if (flags & B_SYNC)
				bwrite(bp);
			else
				bawrite(bp);
		}
	}
	/*
	 * The first NDADDR blocks are direct blocks
	 */
	if (lbn < NDADDR) {
		nb = ip->i_db[lbn];
		if (nb != 0 && ip->i_size >= (lbn + 1) * fs->fs_bsize) {
			error = bread(vp, lbn, fs->fs_bsize, NOCRED, &bp);
			if (error) {
				brelse(bp);
				return (error);
			}
			*bpp = bp;
			return (0);
		}
		if (nb != 0) {
			/*
			 * Consider need to reallocate a fragment.
			 */
			osize = fragroundup(fs, blkoff(fs, ip->i_size));
			nsize = fragroundup(fs, size);
			if (nsize <= osize) {
				error = bread(vp, lbn, osize, NOCRED, &bp);
				if (error) {
					brelse(bp);
					return (error);
				}
			} else {
				error = ffs_realloccg(ip, lbn,
				    ffs_blkpref(ip, lbn, (int)lbn,
					&ip->i_db[0]), osize, nsize, cred, &bp);
				if (error)
					return (error);
			}
		} else {
			if (ip->i_size < (lbn + 1) * fs->fs_bsize)
				nsize = fragroundup(fs, size);
			else
				nsize = fs->fs_bsize;
			error = ffs_alloc(ip, lbn,
			    ffs_blkpref(ip, lbn, (int)lbn, &ip->i_db[0]),
			    nsize, cred, &newb);
			if (error)
				return (error);
			bp = getblk(vp, lbn, nsize, 0, 0);
			bp->b_blkno = fsbtodb(fs, newb);
			if (flags & B_CLRBUF)
				clrbuf(bp);
		}
		ip->i_db[lbn] = dbtofsb(fs, bp->b_blkno);
		ip->i_flag |= IN_CHANGE | IN_UPDATE;
		*bpp = bp;
		return (0);
	}
	/*
	 * Determine the number of levels of indirection.
	 */
	pref = 0;
	if (error = ufs_getlbns(vp, lbn, indirs, &num))
		return(error);
#ifdef DIAGNOSTIC
	if (num < 1)
		panic ("ffs_balloc: ufs_bmaparray returned indirect block\n");
#endif
	/*
	 * Fetch the first indirect block allocating if necessary.
	 */
	--num;
	nb = ip->i_ib[indirs[0].in_off];
	allocib = NULL;
	allocblk = allociblk;
	if (nb == 0) {
		pref = ffs_blkpref(ip, lbn, 0, (ufs_daddr_t *)0);
	        if (error = ffs_alloc(ip, lbn, pref, (int)fs->fs_bsize,
		    cred, &newb))
			return (error);
		nb = newb;
		*allocblk++ = nb;
		bp = getblk(vp, indirs[1].in_lbn, fs->fs_bsize, 0, 0);
		bp->b_blkno = fsbtodb(fs, nb);
		clrbuf(bp);
		/*
		 * Write synchronously so that indirect blocks
		 * never point at garbage.
		 */
		if (error = bwrite(bp))
			goto fail;
		allocib = &ip->i_ib[indirs[0].in_off];
		*allocib = nb;
		ip->i_flag |= IN_CHANGE | IN_UPDATE;
	}
	/*
	 * Fetch through the indirect blocks, allocating as necessary.
	 */
	for (i = 1;;) {
		error = bread(vp,
		    indirs[i].in_lbn, (int)fs->fs_bsize, NOCRED, &bp);
		if (error) {
			brelse(bp);
			goto fail;
		}
		bap = (ufs_daddr_t *)bp->b_data;
		nb = bap[indirs[i].in_off];
		if (i == num)
			break;
		i += 1;
		if (nb != 0) {
			brelse(bp);
			continue;
		}
		if (pref == 0)
			pref = ffs_blkpref(ip, lbn, 0, (ufs_daddr_t *)0);
		if (error =
		    ffs_alloc(ip, lbn, pref, (int)fs->fs_bsize, cred, &newb)) {
			brelse(bp);
			goto fail;
		}
		nb = newb;
		*allocblk++ = nb;
		nbp = getblk(vp, indirs[i].in_lbn, fs->fs_bsize, 0, 0);
		nbp->b_blkno = fsbtodb(fs, nb);
		clrbuf(nbp);
		/*
		 * Write synchronously so that indirect blocks
		 * never point at garbage.
		 */
		if (error = bwrite(nbp)) {
			brelse(bp);
			goto fail;
		}
		bap[indirs[i - 1].in_off] = nb;
		/*
		 * If required, write synchronously, otherwise use
		 * delayed write.
		 */
		if (flags & B_SYNC) {
			bwrite(bp);
		} else {
			bdwrite(bp);
		}
	}
	/*
	 * Get the data block, allocating if necessary.
	 */
	if (nb == 0) {
		pref = ffs_blkpref(ip, lbn, indirs[i].in_off, &bap[0]);
		if (error = ffs_alloc(ip,
		    lbn, pref, (int)fs->fs_bsize, cred, &newb)) {
			brelse(bp);
			goto fail;
		}
		nb = newb;
		*allocblk++ = nb;
		nbp = getblk(vp, lbn, fs->fs_bsize, 0, 0);
		nbp->b_blkno = fsbtodb(fs, nb);
		if (flags & B_CLRBUF)
			clrbuf(nbp);
		bap[indirs[i].in_off] = nb;
		/*
		 * If required, write synchronously, otherwise use
		 * delayed write.
		 */
		if (flags & B_SYNC) {
			bwrite(bp);
		} else {
			bdwrite(bp);
		}
		*bpp = nbp;
		return (0);
	}
	brelse(bp);
	if (flags & B_CLRBUF) {
		error = bread(vp, lbn, (int)fs->fs_bsize, NOCRED, &nbp);
		if (error) {
			brelse(nbp);
			goto fail;
		}
	} else {
		nbp = getblk(vp, lbn, fs->fs_bsize, 0, 0);
		nbp->b_blkno = fsbtodb(fs, nb);
	}
	*bpp = nbp;
	return (0);
fail:
	/*
	 * If we have failed part way through block allocation, we
	 * have to deallocate any indirect blocks that we have allocated.
	 */
	for (deallocated = 0, blkp = allociblk; blkp < allocblk; blkp++) {
		ffs_blkfree(ip, *blkp, fs->fs_bsize);
		deallocated += fs->fs_bsize;
	}
	if (allocib != NULL)
		*allocib = 0;
	if (deallocated) {
#ifdef QUOTA
		/*
		 * Restore user's disk quota because allocation failed.
		 */
		(void) chkdq(ip, (long)-btodb(deallocated), cred, FORCE);
#endif
		ip->i_blocks -= btodb(deallocated);
		ip->i_flag |= IN_CHANGE | IN_UPDATE;
	}
	return (error);
}
