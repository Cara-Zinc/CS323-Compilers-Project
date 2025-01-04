# CS323-Compilers-Project
An extended version of SPL with object-oriented elements, featuring structs that contain functions. This project aims to provide an efficient, flexible language with support to different paradigms.

## About Submodules
This project uses some submodules. To initialize these submodules:
```sh
git submodule init
git submodule update --recursive
```

## How to call iterators and foreaches in CMC
See src/frontend/parser/iter_collections_example.c
For more APIS see PFX_iter_xxx functions.
[iterator docs](https://cmc.leoven.dev/Examples/examples.html#iteration)
[foreach docs](https://cmc.leoven.dev/utl/foreach.h/index.html)

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
| `Program : ExtDefList`                                                                                                                                                                                                                                                                                                                | program.c       | Main program structure                                        |xqw|
| `ExtDefList : ExtDef ExtDefList \| /* empty */`                                                                                                                                                                                                                                                                                       | program.c       | List of external definitions                                 | xqw|
| `ExtDef : Specifier ExtDecList SEMI \| Specifier SEMI \| Specifier FunDec CompSt`                                                                                                                                                                                                                                                     | program.c       | External definition handling                                  |xqw|
| `ExtDecList : VarDec \| VarDec COMMA ExtDecList`                                                                                                                                                                                                                                                                                      | program.c       | External declarations list                                    |hjz|
| `Specifier : TYPE \| StructSpecifier`                                                                                                                                                                                                                                                                                                 | type_struct.c   | Type and struct specifiers                                    |xqw |
| `StructSpecifier : STRUCT ID LC DefList RC \| STRUCT ID`                                                                                                                                                                                                                                                                              | type_struct.c   | Struct definition                                             |xqw|
| `VarDec : ID \| VarDec LB INT RB`                                                                                                                                                                                                                                                                                                     | declaration.c   | Variable declaration, including arrays                        | hhd,zhr|
| `FunDec : ID LP VarList RP \| ID LP RP`                                                                                                                                                                                                                                                                                               | declaration.c   | Function declaration                                          | hhd,zhr|
| `VarList : ParamDec COMMA VarList \| ParamDec`                                                                                                                                                                                                                                                                                        | declaration.c   | List of function parameters                                   | hhd, zhr|
| `ParamDec : Specifier VarDec`                                                                                                                                                                                                                                                                                                         | declaration.c   | Parameter declaration                                         |hhd, zhr|
| `CompSt : LC DefList StmtList RC`                                                                                                                                                                                                                                                                                                     | compound_stmt.c | Compound statement (block of code)                            |hhd, zhr|
| `StmtList : Stmt StmtList \| /* empty */`                                                                                                                                                                                                                                                                                             | compound_stmt.c | List of statements within a compound statement                |hhd,zhr|
| `Stmt : Exp SEMI \| CompSt \| RETURN Exp SEMI \| IF LP Exp RP Stmt \| IF LP Exp RP Stmt ELSE Stmt \| WHILE LP Exp RP Stmt`                                                                                                                                                                                                            | stmt.c          | Statements including if, while, return, and block             |hhd,zhr|
| `DefList : Def DefList \| /* empty */`                                                                                                                                                                                                                                                                                                | def.c           | List of definitions within a scope               |      hhd, zhr       |
| `Def : Specifier DecList SEMI`                                                                                                                                                                                                                                                                                                        | def.c           | Definition handling             |                  hhd, zhr            |
| `DecList : Dec \| Dec COMMA DecList`                                                                                                                                                                                                                                                                                                  | def.c           | List of declarations          |                    hhd, zhr            |
| `Dec : VarDec \| VarDec ASSIGN Exp`                                                                                                                                                                                                                                                                                                   | declaration.c           | Declaration with optional initialization         |      hhd, zhr       |
| `Exp : Exp ASSIGN Exp \| Exp AND Exp \| Exp OR Exp \| Exp LT Exp \| Exp LE Exp \| Exp GT Exp \| Exp GE Exp \| Exp NE Exp \| Exp EQ Exp \| Exp PLUS Exp \| Exp MINUS Exp \| Exp MUL Exp \| Exp DIV Exp \| LP Exp RP \| MINUS Exp \| NOT Exp \| ID LP Args RP \| ID LP RP \| Exp LB Exp RB \| Exp DOT ID \| ID \| INT \| FLOAT \| CHAR` | exp.c           | Expressions including arithmetic, logical, and function calls | hjz|
| `Args : Exp COMMA Args \| Exp`                                                                                                                                                                                                                                                                                                        | exp.c           | Arguments for function calls                                  | hjz|
