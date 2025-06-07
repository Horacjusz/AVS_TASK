CC = gcc
CFLAGS = -Wall -std=c11 -D_POSIX_C_SOURCE=200809L -g -Iinclude
SRCDIR = src
INCDIR = include
BINDIR = bin
TARGET = $(BINDIR)/simulation

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(BINDIR)/%.o)

EMBEDDED_SRC = $(SRCDIR)/embedded.c
EMBEDDED_OBJ = $(BINDIR)/embedded.o

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

embedded: CFLAGS += -DEMBEDDED
embedded: $(filter-out $(EMBEDDED_OBJ), $(OBJ)) $(EMBEDDED_OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(TARGET) $^

clean:
	rm -rf $(BINDIR)

.PHONY: all clean embedded
