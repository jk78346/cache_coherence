# globals
APP                = main

# dirs
APP_DIR           	= .
SRC_DIR           	= src
OBJ_DIR			  	= obj
INC_DIR			  	= include
BIN_DIR				= bin

# compilers
CC				  = gcc

INC = 	-I$(APP_DIR)/$(INC_DIR)/
		
# flags
CFLAGS            = -O3 -Wall -m64 -fopenmp -g

all: test

$(APP_DIR)/$(OBJ_DIR)/$(APP).o: $(APP_DIR)/$(SRC_DIR)/$(APP).c
	@echo 'making $(APP) <- $(APP).o'
	@$(CC) $(CFLAGS) $(INC) -c -o $(APP_DIR)/$(OBJ_DIR)/$(APP).o $(APP_DIR)/$(SRC_DIR)/$(APP).c

$(APP_DIR)/$(OBJ_DIR)/edgelist.o: $(APP_DIR)/$(SRC_DIR)/edgelist.c $(APP_DIR)/$(INC_DIR)/edgelist.h
	@echo 'making $(APP) <- edgelist.o'
	@$(CC) $(CFLAGS) $(INC) -c -o $(APP_DIR)/$(OBJ_DIR)/edgelist.o $(APP_DIR)/$(SRC_DIR)/edgelist.c

$(APP_DIR)/$(OBJ_DIR)/sort.o: $(APP_DIR)/$(SRC_DIR)/sort.c $(APP_DIR)/$(INC_DIR)/sort.h
	@echo 'making $(APP) <- sort.o'
	@$(CC) $(CFLAGS) $(INC) -c -o $(APP_DIR)/$(OBJ_DIR)/sort.o $(APP_DIR)/$(SRC_DIR)/sort.c

$(APP_DIR)/$(OBJ_DIR)/vertex.o: $(APP_DIR)/$(SRC_DIR)/vertex.c $(APP_DIR)/$(INC_DIR)/vertex.h
	@echo 'making $(APP) <- vertex.o'
	@$(CC) $(CFLAGS) $(INC) -c -o $(APP_DIR)/$(OBJ_DIR)/vertex.o $(APP_DIR)/$(SRC_DIR)/vertex.c

$(APP_DIR)/$(OBJ_DIR)/timer.o: $(APP_DIR)/$(SRC_DIR)/timer.c $(APP_DIR)/$(INC_DIR)/timer.h
	@echo 'making $(APP) <- timer.o'
	@$(CC) $(CFLAGS) $(INC) -c -o $(APP_DIR)/$(OBJ_DIR)/timer.o $(APP_DIR)/$(SRC_DIR)/timer.c

$(APP_DIR)/$(OBJ_DIR)/bfs.o: $(APP_DIR)/$(SRC_DIR)/bfs.c $(APP_DIR)/$(INC_DIR)/bfs.h
	@echo 'making $(APP) <- bfs.o'
	@$(CC) $(CFLAGS) $(INC) -c -o $(APP_DIR)/$(OBJ_DIR)/bfs.o $(APP_DIR)/$(SRC_DIR)/bfs.c

app: $(APP_DIR)/$(OBJ_DIR)/$(APP).o
edgelist: $(APP_DIR)/$(OBJ_DIR)/edgelist.o
sort: $(APP_DIR)/$(OBJ_DIR)/sort.o
vertex: $(APP_DIR)/$(OBJ_DIR)/vertex.o
timer: $(APP_DIR)/$(OBJ_DIR)/timer.o
bfs: $(APP_DIR)/$(OBJ_DIR)/bfs.o

test: app edgelist sort vertex timer bfs
	@echo 'linking $(APP) <- $(APP).o edgelist.o sort.o'
	@$(CC) 	$(APP_DIR)/$(OBJ_DIR)/$(APP).o 			\
			$(APP_DIR)/$(OBJ_DIR)/edgelist.o 		\
			$(APP_DIR)/$(OBJ_DIR)/sort.o 			\
			$(APP_DIR)/$(OBJ_DIR)/vertex.o 			\
			$(APP_DIR)/$(OBJ_DIR)/timer.o  			\
			$(APP_DIR)/$(OBJ_DIR)/bfs.o 			\
		 -o $(APP_DIR)/$(BIN_DIR)/$(APP)			\
			$(CFLAGS)

run: test
	$(APP_DIR)/$(BIN_DIR)/$(APP) 

debug: test	
	gdb $(APP_DIR)/$(BIN_DIR)/$(APP)

clean:
	@rm -f  $(APP_DIR)/$(BIN_DIR)/*
	@rm -f  $(APP_DIR)/$(OBJ_DIR)/*
	
