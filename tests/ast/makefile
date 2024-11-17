# Compiler and flags
CC = gcc
CFLAGS = -g -I $(DEP_CMC)  # Include current directory and the DEP_CMC path for headers

# Dependency include directory
DEP_CMC = ../../dep/cmc

# Executable names for different tests
TARGETS = test_rprint_ast

# Main source files (shared by all tests)
AST_SRCS = test_ast.c  # Add more AST source files if necessary

# Include paths and libraries
INCLUDES = -I $(DEP_CMC)  # Include current directory and dependency directory for headers
LIBS = -lcmc    # Link the CMCList library (or replace with appropriate library)

# Test source files
TEST_RPRINT_SRCS = test_rprint_ast.c

# Default target: compile all tests
all: $(TARGETS)


# Compile the rprintAST test
test_rprintAST: $(AST_SRCS:.c=.o) $(TEST_RPRINT_SRCS:.c=.o)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the rprintAST test
rprint: test_rprint
	./test_rprint_ast

# Clean up build files
clean:
	rm -f *.o $(TARGETS)

# PHONY targets
.PHONY: all clean rprint
