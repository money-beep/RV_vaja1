# Variables 
CC = gcc
CFLAGS = -Wall -Wextra -lm
SRCS = main.c numberDifferences.c repeatingDifferences.c bitStack.c absoluteEncoding.c compress.c decompress.c
TARGET = main

# Default target to build the executable
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(SRCS)
	$(CC) -o $(TARGET) $(SRCS) $(CFLAGS)

# Clean up generated files
.PHONY: clean
clean:
	rm -f $(TARGET)


