%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyerror(char *s);
int yywrap();
%}

%start s
%token ONE ZERO EPSILON

%%

s: a | b
a: t ZERO a | t ZERO t
t: ZERO t ONE | ONE t ZERO | EPSILON
b: u ONE b | u ONE u
u: ZERO u ONE | ONE u ZERO | u u | EPSILON

    // s: ZERO s ZERO | ONE s ONE | EPSILON

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
