CC = gcc
CFLAGS = -Wall -Wextra -I include
SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/compiler

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(OBJECTS) -o $@
	@echo "Build successful: $(TARGET)"

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)
	@echo "Clean complete"

run: $(TARGET)
	@./$(TARGET) test.unn

test: $(TARGET)
	@echo "Running test on test.unn..."
	@./$(TARGET) test.unn

.DEFAULT_GOAL := all
