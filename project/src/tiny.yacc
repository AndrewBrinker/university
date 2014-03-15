%{
#include <stdio.h>
#include <stdlib.h>
#include "../lib/filename.c"

extern FILE *yyin;
extern int line_number;

int yylex();
int yyerror(char *s);
int yywrap();
%}

%start program

%union {
    int i;
    char c;
    double d;
    char *s;
}

%type <i> INTEGER
%type <d> DECIMAL
%type <c> LETTER
%token INTEGER LETTER DECIMAL GOTO IF THEN LET PRINT END INPUT NEWLINE
%left '=' ','
%left LT LE GT GE EQ NQ
%left '+' '-'
%left '*' '/'

%%


program:    block;

block:      block line |
            line;

line:       INTEGER statement NEWLINE |
            statement NEWLINE;

statement:  PRINT exprlist |
            IF expression relop expression THEN statement |
            GOTO expression |
            INPUT varlist |
            LET var '=' expression |
            END;

exprlist:   exprlist ',' expression |
            expression;

varlist:    varlist ',' var |
            var;

expression: expression '+' term |
            expression '-' term |
            term;

term:       term '*' factor |
            term '/' factor |
            factor;

factor:     var |
            number |
            '(' expression ')';

number:     INTEGER {
                printf("Integer: %d\n", $1);
            } |
            DECIMAL {
                printf("Decimal: %f\n", $1);
            };

var:        LETTER {
                printf("Letter: %c\n", $1);
            };

relop:      LT | LE | GT | GE | EQ | NQ;

%%

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *input_file_name  = argv[1];
    char *output_file_name = convert_file_name(input_file_name);
    free(output_file_name);

    FILE *input_file = fopen(input_file_name, "r");
    if (!input_file) {
        printf("%s can't be opened. Exiting...\n", input_file_name);
        exit(EXIT_FAILURE);
    }

    FILE *output_file = fopen(output_file_name, "w");
    if (!output_file) {
        printf("%s can't be created. Exiting...\n", output_file_name);
        exit(EXIT_FAILURE);
    }

    yyin = input_file;
    return yyparse();
}

int yyerror(char *s) {
    printf("[%d] %s\n", line_number, s);
    return 1;
}

int yywrap() {
    return 1;
}
