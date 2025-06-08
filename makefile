CC = gcc
CFLAGS = -Wall -std=c11 -D_POSIX_C_SOURCE=200809L -g -Iinclude
SRCDIR = src
INCDIR = include
BINDIR = bin
TESTDIR = test
TESTBINDIR = bin-test
TARGET = $(BINDIR)/simulation

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(BINDIR)/%.o)
OBJ_NO_MAIN = $(filter-out $(BINDIR)/main.o, $(OBJ))

EMBEDDED_SRC = $(SRCDIR)/embedded.c
EMBEDDED_OBJ = $(BINDIR)/embedded.o

TESTSRC = $(wildcard $(TESTDIR)/*_test.c)
TESTOBJ = $(TESTSRC:$(TESTDIR)/%.c=$(TESTBINDIR)/%.o)
TESTMAIN = $(TESTDIR)/test_main.c
TESTTARGET = $(TESTBINDIR)/test_main


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


run:
	@if [ "$(word 2,$(MAKECMDGOALS))" = "" ] || [ "$(word 3,$(MAKECMDGOALS))" = "" ]; then \
		echo "Usage: make run input.json output.json"; \
		exit 1; \
	fi && \
	$(MAKE) all --no-print-directory && \
	./$(TARGET) $(word 2,$(MAKECMDGOALS)) $(word 3,$(MAKECMDGOALS))

run-embedded:
	@if [ "$(word 2,$(MAKECMDGOALS))" = "" ] || [ "$(word 3,$(MAKECMDGOALS))" = "" ]; then \
		echo "Usage: make run-embedded input.json output.json"; \
		exit 1; \
	fi && \
	$(MAKE) embedded --no-print-directory && \
	./$(TARGET) $(word 2,$(MAKECMDGOALS)) $(word 3,$(MAKECMDGOALS))


$(TESTBINDIR)/%.o: $(TESTDIR)/%.c
	@mkdir -p $(TESTBINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TESTTARGET): $(TESTOBJ) $(TESTMAIN)
	@mkdir -p $(TESTBINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(OBJ_NO_MAIN)

test: all $(TESTTARGET)
	@./$(TESTTARGET)

clean-test:
	rm -rf $(TESTBINDIR)


clean: clean-test
	rm -rf $(BINDIR)

.PHONY: all clean clean-test embedded run run-embedded test

%::
	@:
