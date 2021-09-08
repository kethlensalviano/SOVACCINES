OBJ_dir = obj
SRC_dir = src
INC_dir = include
BIN_dir = bin

OBJETOS = main.o client.o memory.o memory-private.o process.o proxy.o server.o synchronization.o main-private.o configuration.o sotime.o sosignal.o log.o stats.o

CC = gcc
CFLAGS = -g -Wall -I $(INC_dir) -Wextra
LIBS = -lrt -lpthread

vpath %.o $(OBJ_dir)

bin/sovaccines: $(OBJETOS)
	$(CC)  $(addprefix $(OBJ_dir)/,$(OBJETOS)) -o $(BIN_dir)/sovaccines $(LIBS)

%.o: $(SRC_dir)/%.c $($@)
	$(CC) $(CFLAGS) -o $(OBJ_dir)/$@ -c $<

clean:
	rm -f *.o $(BIN_dir)/sovaccines
	rm -f $(BIN_dir)/sovaccines
	rm -f log
	rm -f stats
