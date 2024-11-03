%{
#include "syntax.tab.h"
%}

%%
[ \t\n]+          ;                    // ignore whitespaces
"if"              { return IF; }
"else"            { return ELSE; }
"while"           { return WHILE; }
"int"             { return INT; }
"return"          { return RETURN; }
[0-9]+            { yylval = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { return IDENTIFIER; }
"=="              { return EQ; }
"="               { return ASSIGN; }
"<"               { return LT; }
"+"               { return PLUS; }
"-"               { return MINUS; }
"*"               { return TIMES; }
"/"               { return DIVIDE; }
"("               { return LPAREN; }
")"               { return RPAREN; }
"{"               { return LBRACE; }
"}"               { return RBRACE; }
";"               { return SEMI; }
.                 { return yytext[0]; }
%%