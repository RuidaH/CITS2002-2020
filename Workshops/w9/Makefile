ALL	= testlist testhashtable

C99	= cc -std=c99 -Wall -Werror -pedantic


all:	$(ALL)

testlist: list.h testlist.c list.c support.c
	$(C99) -o testlist testlist.c list.c support.c

testhashtable: hashtable.h testhashtable.c hashtable.c list.h list.c support.c
	$(C99) -o testhashtable testhashtable.c hashtable.c list.c support.c


compare: $(ALL)
	@./compare.sh
	
plot: $(ALL)
	@./plot-to-html.sh
	@echo 'now open plot.html in your browser'


clean:
	rm -f $(ALL) col? stats.* plot.html
