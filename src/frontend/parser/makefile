# Variables
LEX = flex
YACC = bison
CC = gcc
LEX_DIR = lexer
YACC_DIR = parser
LEX_FILE = lex.l
YACC_FILE = syntax.y
EXEC = structura_parser
DEP_CMC = ../../../dep/cmc

# Intermediate files
LEX_C = lex.yy.c
YACC_C = syntax.tab.c
YACC_H = syntax.tab.h

# Default target
all: $(EXEC)

# Generate the parser source code from Bison (produces both .c and .h files)
$(YACC_C) $(YACC_H): $(YACC_FILE)
	$(YACC) -d $(YACC_FILE) -o $(YACC_C)

# Generate the lexer source code from Flex (depends on syntax.tab.h being present)
$(LEX_C): $(LEX_FILE) $(YACC_H)
	$(LEX) -o $(LEX_C) $(LEX_FILE)

# Compile and link everything together
$(EXEC): $(LEX_C) $(YACC_C)
	$(CC) $(LEX_C) $(YACC_C) ../defs/*.c -o $(EXEC) -Wall -lfl -I $(DEP_CMC)

# Clean up intermediate and generated files
clean:
	rm -f $(LEX_C) $(YACC_C) $(YACC_H) $(EXEC)

# PHONY targets to avoid conflicts with files named "all", "clean", etc.
.PHONY: all clean
