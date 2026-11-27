CC=gcc
CCFLAGS=-std=gnu17 -Wall -Wextra -pthread
PREFIX=/usr/local
TARGET=rmentn

SRC=src/main.c \
	src/worker/scan.c \
	src/worker/queue.c \
	src/worker/result.c \
	src/worker/worker.c \
	src/path/path.c \
	src/remove/remove.c \
	src/remove/confirm.c \
	src/flags/parse.c \
	src/error/error.c \
	src/wildcard/match.c

OBJ=$(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJ)
	rm -f $(OBJ)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

install: $(TARGET)
	install -Dm755 $(TARGET) $(PREFIX)/bin/$(TARGET)

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)