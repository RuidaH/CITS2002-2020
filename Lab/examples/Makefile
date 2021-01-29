
ALL	= list queue rpn1 rpn2 stack
SRC	= list.c queue.c rpn1.c rpn2.c stack.c

CC	= cc
CFLAGS	= -std=c99 -Wall -Werror -pedantic


all:	$(ALL)

%:	%.c
	$(CC) $(CFLAGS) -o $@ $<

zip:
	zip examples.zip Makefile $(SRC) hp-program
	chmod 644 examples.zip


clean:
	rm -f $(ALL)
