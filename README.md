# CS323-Compilers-Project
An extended version of SPL with object-oriented elements, featuring structs that contain functions. This project aims to provide an efficient, flexible language with support to different paradigms.

## About Submodules
This project uses some submodules. To initialize these submodules:
```sh
git submodule init
git submodule update --recursive
```

## Project overview
```plaintext
project-root/
├── docs/                   
├── src/                    
│   ├── frontend/           
│   │   ├── lexer/          # Lexical analyzer using Flex
│   │   ├── parser/         # Syntax parser using Bison
│   │   ├── ast/            # Abstract Syntax Tree (AST) representation
│   │   └── semantic/       # Semantic analyzer (optional)
│   ├── backend/            
│   │   ├── ir/             # Intermediate Representation (IR) generator TAC
│   │   ├── codegen/        # Code generation
│   │   └── optimizer/      # Optimization passes
│   ├── runtime/            # Runtime support file
│   └── main.c              # Main entry point for the compiler
├── tests/                  # Test cases for all stages
│   ├── lexer_tests/        
│   ├── parser_tests/       
│   └── codegen_tests/      
├── examples/               # Example programs
├── scripts/                               
└── README.md               
```

## A closer look of CFG

| CFG Rule                                                                                                                                                                                                                                                                                                                              | .c File         | Description                                                   |developer|
|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-----------------|---------------------------------------------------------------|------|
| `Program : ExtDefList`                                                                                                                                                                                                                                                                                                                | program.c       | Main program structure                                        |hjz|
| `ExtDefList : ExtDef ExtDefList \| /* empty */`                                                                                                                                                                                                                                                                                       | program.c       | List of external definitions                                 | hjz|
| `ExtDef : Specifier ExtDecList SEMI \| Specifier SEMI \| Specifier FunDec CompSt`                                                                                                                                                                                                                                                     | program.c       | External definition handling                                  |hjz|
| `ExtDecList : VarDec \| VarDec COMMA ExtDecList`                                                                                                                                                                                                                                                                                      | program.c       | External declarations list                                    ||
| `Specifier : TYPE \| StructSpecifier`                                                                                                                                                                                                                                                                                                 | type_struct.c   | Type and struct specifiers                                    |hjz |
| `StructSpecifier : STRUCT ID LC DefList RC \| STRUCT ID`                                                                                                                                                                                                                                                                              | type_struct.c   | Struct definition                                             | hjz|
| `VarDec : ID \| VarDec LB INT RB`                                                                                                                                                                                                                                                                                                     | declaration.c   | Variable declaration, including arrays                        | zhr|
| `FunDec : ID LP VarList RP \| ID LP RP`                                                                                                                                                                                                                                                                                               | declaration.c   | Function declaration                                          | zhr|
| `VarList : ParamDec COMMA VarList \| ParamDec`                                                                                                                                                                                                                                                                                        | declaration.c   | List of function parameters                                   | zhr|
| `ParamDec : Specifier VarDec`                                                                                                                                                                                                                                                                                                         | declaration.c   | Parameter declaration                                         |zhr|
| `CompSt : LC DefList StmtList RC`                                                                                                                                                                                                                                                                                                     | compound_stmt.c | Compound statement (block of code)                            |hhd|
| `StmtList : Stmt StmtList \| /* empty */`                                                                                                                                                                                                                                                                                             | compound_stmt.c | List of statements within a compound statement                |hhd|
| `Stmt : Exp SEMI \| CompSt \| RETURN Exp SEMI \| IF LP Exp RP Stmt \| IF LP Exp RP Stmt ELSE Stmt \| WHILE LP Exp RP Stmt`                                                                                                                                                                                                            | stmt.c          | Statements including if, while, return, and block             |hhd|
| `DefList : Def DefList \| /* empty */`                                                                                                                                                                                                                                                                                                | def.c           | List of definitions within a scope               |             |
| `Def : Specifier DecList SEMI`                                                                                                                                                                                                                                                                                                        | def.c           | Definition handling             |                              |
| `DecList : Dec \| Dec COMMA DecList`                                                                                                                                                                                                                                                                                                  | def.c           | List of declarations          |                                |
| `Dec : VarDec \| VarDec ASSIGN Exp`                                                                                                                                                                                                                                                                                                   | declaration.c           | Declaration with optional initialization         |      zhr       |
| `Exp : Exp ASSIGN Exp \| Exp AND Exp \| Exp OR Exp \| Exp LT Exp \| Exp LE Exp \| Exp GT Exp \| Exp GE Exp \| Exp NE Exp \| Exp EQ Exp \| Exp PLUS Exp \| Exp MINUS Exp \| Exp MUL Exp \| Exp DIV Exp \| LP Exp RP \| MINUS Exp \| NOT Exp \| ID LP Args RP \| ID LP RP \| Exp LB Exp RB \| Exp DOT ID \| ID \| INT \| FLOAT \| CHAR` | exp.c           | Expressions including arithmetic, logical, and function calls | hjz|
| `Args : Exp COMMA Args \| Exp`                                                                                                                                                                                                                                                                                                        | exp.c           | Arguments for function calls                                  | hjz|
