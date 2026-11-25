CC=gcc
CCFLAGS=-std=gnu17 -Wall -Wextra -pthread
TARGET=rmentn

SRC=src/main.c \
	src/worker/scan.c \
	src/worker/enqueue.c \
	src/worker/result.c \
	src/worker/worker.c

OBJ=$(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJ)
	rm -f $(OBJ)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)