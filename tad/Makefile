CC=gcc
CFLAGS= -W -Wall -Wextra
DBFLAGS= -g -O0

SRC=test_tad.c tree.c
OBJ=$(SRC:.c=.o)
EXEC=tree

all: $(EXEC)

$(EXEC): $(OBJ)
	@echo "Linking\t\t [$@]"
	@$(CC) $(CFLAGS) $(DBFLAGS) -o $@ $^
	
%.o:%.c
	@echo "Compiling\t [$@]"
	@$(CC) $(CFLAGS) $(DBFLAGS) -o $@ -c $<
	
.PHONY:all

clean:
	@rm -rf *.o $(EXEC)
