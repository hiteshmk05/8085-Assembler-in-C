# Makefile

# Compiler
CC = gcc

# Compiler flags (no warnings)
CFLAGS = -O2

# Source files
SRCS = assembler.c hashmap.c string_vector.c preprocessing.c

# Output executable
TARGET = assembler.exe

# Default target
all: $(TARGET)

# Compile and link source files to create executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET)
