CC=gcc
CFLAGS= -W -Wall -Wextra
DBFLAGS= -g -O0 -DDEBUG

SRC=test_tad.c tree.c
SRC_LIST = test_tad_list.c list.c
SRC_CODE = test_code.c code.c
SRC_LAYER = test_layer.c layer.c tree.c list.c
SRC_INT_HEAP = test_int_heap.c int_heap.c

OBJ=$(SRC:.c=.o)
OBJ_LIST=$(SRC_LIST:.c=.o)
OBJ_CODE=$(SRC_CODE:.c=.o)
OBJ_LAYER=$(SRC_LAYER:.c=.o)
OBJ_INT_HEAP=$(SRC_INT_HEAP:.c=.o)

EXEC=tree
EXEC_LIST = list
EXEC_CODE = code
EXEC_LAYER = layer
EXEC_INT_HEAP = int_heap

all: $(EXEC) $(EXEC_LIST) $(EXEC_CODE) $(EXEC_LAYER) $(EXEC_INT_HEAP)

$(EXEC_LIST): $(OBJ_LIST)
	@echo "Linking\t\t [$@]"
	@$(CC) $(CFLAGS) $(DBFLAGS) -o $@ $^

$(EXEC_CODE): $(OBJ_CODE)
	@echo "Linking\t\t [$@]"
	@$(CC) $(CFLAGS) $(DBFLAGS) -o $@ $^

$(EXEC_LAYER): $(OBJ_LAYER)
	@echo "Linking\t\t [$@]"
	@$(CC) $(CFLAGS) $(DBFLAGS) -o $@ $^

$(EXEC_INT_HEAP): $(OBJ_INT_HEAP)
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
	@rm -rf *.o $(EXEC) $(EXEC_LIST) $(EXEC_CODE) $(EXEC_LAYER) $(EXEC_INT_HEAP)
