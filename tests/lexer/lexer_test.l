%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%%
[ \t\n]+                  ;  // Ignore whitespaces

"if"                        { printf("Keyword: IF\n"); }
"else"                      { printf("Keyword: ELSE\n"); }
"while"                     { printf("Keyword: WHILE\n"); }
"return"                    { printf("Keyword: RETURN\n"); }

0|[1-9][0-9]*                 { printf("Integer: %s\n", yytext); }
0x[0-9a-fA-F]+              { printf("Hex Integer: %s\n", yytext); }

[1-9][0-9]*\.[0-9]+         { printf("Float: %s\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*      { printf("Identifier: %s\n", yytext); }

"int"                       { printf("Type: int\n"); }
"float"                     { printf("Type: float\n"); }
"char"                      { printf("Type: char\n"); }

\'\\x[0-9a-fA-F]{2}\'       { printf("Char: %s\n", yytext); }
\'[ -~]\'                   { printf("Char: %s\n", yytext); }
\'\\x.*\'                   {printf("Error: illegal hex char %s \n", yytext);}

"=="                        { printf("Operator: ==\n"); }
"="                         { printf("Operator: =\n"); }
"<"                         { printf("Operator: <\n"); }
"<="                        { printf("Operator: <=\n"); }
">"                         { printf("Operator: >\n"); }
">="                        { printf("Operator: >=\n"); }
"!="                        { printf("Operator: !=\n"); }
"+"                         { printf("Operator: +\n"); }
"-"                         { printf("Operator: -\n"); }
"*"                         { printf("Operator: *\n"); }
"/"                         { printf("Operator: /\n"); }
"&&"                        { printf("Operator: &&\n"); }
"||"                        { printf("Operator: ||\n"); }
"!"                         { printf("Operator: !\n"); }

"("                         { printf("Symbol: (\n"); }
")"                         { printf("Symbol: )\n"); }
"["                         { printf("Symbol: [\n"); }
"]"                         { printf("Symbol: ]\n"); }
"{"                         { printf("Symbol: {\n"); }
"}"                         { printf("Symbol: }\n"); }
";"                         { printf("Symbol: ;\n"); }
","                         { printf("Symbol: ,\n"); }
"."                         { printf("Symbol: .\n"); }

.                           { printf("Error: Undefined token %s\n", yytext); }

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (file) {
            yyin = file;
            yylex();
            fclose(file);
        } else {
            fprintf(stderr, "Error: Unable to open file %s\n", argv[1]);
        }
    } else {
        printf("Error: No input file provided\n");
    }
    return 0;
}
