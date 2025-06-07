CC = gcc
CFLAGS = -Wall -std=c11 -D_POSIX_C_SOURCE=200809L -g -Iinclude
SRCDIR = src
INCDIR = include
BINDIR = bin
TARGET = $(BINDIR)/simulation

SRC = $(wildcard $(SRCDIR)/*.c)

OBJ = $(SRC:$(SRCDIR)/%.c=$(BINDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BINDIR)

.PHONY: all clean
