LEX=lex
YACC=yacc
CFLAGS=-Wall -g
CC=gcc
SRC=y.tab.c lex.yy.c sem_actions.c tad/tree.c tad/list.c tad/code.c tad/layer.c
OBJ=$(SRC:.c=.o)
EXEC=parse

all: $(EXEC)

$(EXEC): $(OBJ)
	@echo "Linking $^ into [$@]"
	@$(CC) $(CFLAGS) -o $@ $^ 

%.o:%.c
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -o $@ -c $<

y.tab.c:grammar.y
	@echo "Making grammar"
	@$(YACC) -d $^

lex.yy.c:scanner.l
	@echo "Making parsor"
	@$(LEX) -o $@ $^

.PHONY: clean

clean:
	@rm -f y.tab.c lex.yy.c parse *.o
