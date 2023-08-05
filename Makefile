CC=/bin/cc
CFLAGS=-std=c11 -g -static -fuse-ld=mold
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

minicc: $(OBJS)
	$(CC) -o minicc $(OBJS) $(LDFLAGS)

$(OBJS): minicc.h

test: minicc
	./test.sh

clean:
	rm -f minicc *.o *~ tmp*

.PHONY: test clean
