%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyerror(char *s);
int yywrap();
%}

%start x
%token ONE ZERO EPSILON

%%

x: ZERO x ZERO | ONE x ONE | EPSILON;

%%

int main() {
    return yyparse();
}

int yyerror(char *s) {
    printf("%s\n", s);
    return 1;
}

int yywrap() {
    return 1;
}
