# Define compiler
CC = gcc

# Define compiler flags
CFLAGS = -Wall -Wextra -g

# Detect all source files
SRCS := $(wildcard src/include/*.c) $(wildcard src/*.c)

# Detect all header files in headers/math directory
HDRS := $(wildcard header/math/*.h) $(wildcard header/*.h)

# Define target executable
TARGET = program

# Default target
all: $(TARGET)

# Compile and link C source files into target executable
$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up intermediate files
clean:
	rm -f $(TARGET)
