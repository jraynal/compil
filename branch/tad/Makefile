CC=gcc
CFLAGS= -W -Wall -Wextra
DBFLAGS= -g -O0 -DDEBUG

SRC=test_tad.c tree.c
SRC_LIST = test_tad_list.c list.c
SRC_CODE = test_code.c code.c
OBJ=$(SRC:.c=.o)
OBJ_LIST=$(SRC_LIST:.c=.o)
OBJ_CODE=$(SRC_CODE:.c=.o)
EXEC=tree
EXEC_LIST = list
EXEC_CODE = code

all: $(EXEC) $(EXEC_LIST) $(EXEC_CODE)

$(EXEC_LIST): $(OBJ_LIST)
	@echo "Linking\t\t [$@]"
	@$(CC) $(CFLAGS) $(DBFLAGS) -o $@ $^

$(EXEC_CODE): $(OBJ_CODE)
	@echo "Linking\t\t [$@]"
	@$(CC) $(CFLAGS) $(DBFLAGS) -o $@ $^

$(EXEC): $(OBJ)
	@echo "Linking\t\t [$@]"
	@$(CC) $(CFLAGS) $(DBFLAGS) -o $@ $^
	
%.o:%.c
	@echo "Compiling\t [$@]"
	@$(CC) $(CFLAGS) $(DBFLAGS) -o $@ -c $<
	
.PHONY:all

clean:
	@rm -rf *.o $(EXEC) $(EXEC_LIST)
