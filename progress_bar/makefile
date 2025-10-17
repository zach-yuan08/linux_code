BIN=process
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
CC=gcc
RM=rm -f

$(BIN):$(OBJ)
	@$(CC) $^ -o $@

%.o:%.c
	@$(CC) -c $<

.PHONY:clean
clean:
	@$(RM) $(OBJ) $(BIN)
