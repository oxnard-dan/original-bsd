#	@(#)Makefile	8.1 (Berkeley) 06/06/93

PROG=	strip

install: maninstall
	install -c -o ${BINOWN} -g ${BINOWN} -m ${BINMODE} strip \
	    ${DESTDIR}${BINDIR}
	./strip ${DESTDIR}${BINDIR}/strip
	rm -f ./strip

.include <bsd.prog.mk>
