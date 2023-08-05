CFLAGS=-std=c11 -g -static -fuse-ld=mold

minicc: minicc.c

test: minicc
	./test.sh

clean:
	rm -f minicc *.o *~ tmp*

.PHONY: test clean
