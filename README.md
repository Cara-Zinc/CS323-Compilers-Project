# CS323-Compilers-Project
An extended version of SPL with object-oriented elements, featuring structs that contain functions. This project aims to provide an efficient, flexible language with support to different paradigms.

## Project overview
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
