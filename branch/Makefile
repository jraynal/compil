LEX=lex
YACC=yacc
CFLAGS=-Wall -g
CC=gcc

all:parse

parse:y.tab.c lex.yy.c sem_actions.c tad/tree.c tad/list.c
	$(CC) $(CFLAGS) -o $@ $^ 

y.tab.c:grammar.y
	$(YACC) -d $^

lex.yy.c:scanner.l
	$(LEX) -o $@ $^

clean:
	rm -f y.tab.c lex.yy.c parse
