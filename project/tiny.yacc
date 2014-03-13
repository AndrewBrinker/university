%{
#include <stdio.h>
int regs[26];
int base;
int yylex();
int yyerror(char *s);
int yywrap();
%}

%option noyywrap
%start list
%union {
    int i;
    char c;
    float f;
}
%type
%token

%%


program:    block {
                // Do something
            };


block:      block line {
                // Do something
            } |
            line {
                // Do something
            };


line:       INTEGER statement CR {
                // Do something
            } |
            statement CR {
                // Do something
            };


statement:  PRINT expr-list {
                // Do something
            } |
            IF expression relop expression THEN statement {
                // Do something
            } |
            GOTO expression {
                // Do something
            } |
            INPUT var-list {
                // Do something
            } |
            LET var = expression {
                // Do something
            } |
            GOTO expression {
                // Do something
            } |
            END {
                // Do something
            };


expr-list:  expr-list , expression {
                // Do something
            } |
            expression {
                // Do something
            };


var-list:   var-list , var {
                // Do something
            } |
            var {
                // Do something
            };


expression: expression + term {
                // Do something
            } |
            expression - term {
                // Do something
            } |
            term {
                // Do something
            };


term:       term * factor {
                // Do something
            } |
            term / factor {
                // Do something
            } |
            factor {
                // Do something
            };


factor:     var {
                // Do something
            } |
            number {
                // Do something
            } |
            ( expression ) {
                // Do something
            };


number:     INTEGER {
                // Do something
            } |
            DECIMAL {
                // Do something
            };


var:        LETTER {
                // Do something.
            };


relop:      "<" | "<=" | ">" | ">=" | "==" | "!=";

%%

int main() {
  return yyparse();
}

int yyerror(char *s) {
  fprintf(stderr, "%s\n", s);
  return 1;
}
