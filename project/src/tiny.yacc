%{
#include <stdio.h>
#include <stdlib.h>
#include "../lib/filename.c"

FILE *output_file;
FILE *input_file;

// Forward declarations
int yylex();
int yyerror(char *s);
int yywrap();
%}

%start program
%union {
    int i;
    char c;
    float f;
    char *s;
}
%type <s> program block line statement exprlist varlist expression term
factor number
%token INTEGER LETTER DECIMAL
%left "<" "<=" ">" ">=" "==" "!="
%left '+' '-'
%left '*' '/'
%left UMINUS

%%


program:    block;


block:      block line |
            line {
                fprintf(output_file, "%s", $1);
            };


line:       INTEGER statement "\n" {
                // Do something
            } |
            statement "\n" {
                // Do something
            };


statement:  "PRINT" exprlist {
                // Do something
            } |
            "IF" expression relop expression "THEN" statement {
                // Do something
            } |
            "GOTO" expression {
                // Do something
            } |
            "INPUT" varlist {
                // Do something
            } |
            "LET" var '=' expression {
                // Do something
            } |
            "END" {
                // Do something
            };


exprlist:  exprlist ',' expression {
                // Do something
            } |
            expression {
                // Do something
            };


varlist:   varlist ',' var {
                // Do something
            } |
            var {
                // Do something
            };


expression: expression '+' term {
                // Do something
            } |
            expression '-' term {
                // Do something
            } |
            term;


term:       term '*' factor {
                // Do something
            } |
            term '/' factor {
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
            '(' expression ')' {
                // Do something
            };


number:     INTEGER {
                // Do something
            } |
            DECIMAL {
                // Do something
            };


var:        LETTER {
                // Do something
            };


relop:      "<" | "<=" | ">" | ">=" | "==" | "!=";

%%

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *input_file_name  = argv[1];
    char *output_file_name = convert_file_name(input_file_name);
    free(output_file_name);

    input_file = fopen(input_file_name, "r");
    if (!input_file) {
        printf("%s can't be opened. Exiting...\n", input_file_name);
        exit(EXIT_FAILURE);
    }

    output_file = fopen(output_file_name, "w");
    if (!output_file) {
        printf("%s can't be created. Exiting...\n", output_file_name);
        exit(EXIT_FAILURE);
    }

    return yyparse();
}

int yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    return 1;
}

int yywrap() {
    return 1;
}
