# Define variables
LEXER_SRC = lexer_test.l
LEXER_EXEC = lexer
TEST_INPUT = lexer_test.txt

# Default target
all: $(LEXER_EXEC)

# Compile the lexer
$(LEXER_EXEC): $(LEXER_SRC)
	@flex $(LEXER_SRC)
	@gcc lex.yy.c -o $(LEXER_EXEC) -lfl
	@echo "Lexer compiled successfully."

# Test target to run lexer on the test file
test: $(LEXER_EXEC) $(TEST_INPUT)
	@./$(LEXER_EXEC) < $(TEST_INPUT)

# Clean target to remove generated files
clean:
	@rm -f $(LEXER_EXEC) lex.yy.c
	@echo "Cleaned up generated files."

# PHONY targets to prevent conflicts with files named 'all', 'clean', etc.
.PHONY: all test clean