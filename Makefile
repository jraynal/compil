LEX=lex
YACC=yacc
CFLAGS=-Wall
CC=gcc

all:parse

parse:y.tab.c lex.yy.c sem_actions.c
	$(CC) $(CFLAGS) -o $@ $^

y.tab.c:grammar.y
	$(YACC) -d $^

lex.yy.c:scanner.l
	$(LEX) -o $@ $^

clean:
	rm -f y.tab.c lex.yy.c parse
