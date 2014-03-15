%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARIABLE 1024

#define DOT '.'
#define SEP '/'
#define EXT ".c"

int regs[MAX_VARIABLE];
int base;
int yylex();
int yyerror(char *s);
int yywrap();
char *convert_file_name(const char* input);
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


line:       INTEGER statement "CR" {
                // Do something
            } |
            statement CR {
                // Do something
            };


statement:  "PRINT" expr-list {
                // Do something
            } |
            "IF" expression relop expression "THEN" statement {
                // Do something
            } |
            "GOTO" expression {
                // Do something
            } |
            "INPUT" var-list {
                // Do something
            } |
            "LET" var '=' expression {
                // Do something
            } |
            "GOTO" expression {
                // Do something
            } |
            "END" {
                // Do something
            };


expr-list:  expr-list ',' expression {
                // Do something
            } |
            expression {
                // Do something
            };


var-list:   var-list ',' var {
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
            term {
                // Do something
            };


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
                // Do something.
            };


relop:      "<" | "<=" | ">" | ">=" | "==" | "!=";

%%

char *convert_file_name(const char* input) {
    char *stripped;
    char *last_dot;
    char *last_sep;
    if (input == NULL) return NULL;
    if ((stripped = malloc(strlen(input) + 1)) == NULL) return NULL;
    strcpy(stripped, input);
    last_dot = strrchr(stripped, DOT);
    last_sep = (SEP == 0) ? NULL : strrchr(stripped, SEP);
    if (last_dot != NULL) {
        if (last_sep != NULL) {
            if (last_sep < last_dot) {
                *last_dot = '\0';
            }
        } else {
            *last_dot = '\0';
        }
    }
    size_t length = strlen(input) + strlen(EXT);
    char *ret = (char*) malloc(length * sizeof(char) + 1);
    *ret = '\0';
    char *combined = strcat(strcat(stripped, input), EXT);
    free(stripped);
    return combined;
}

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

    yyin  = input_file;
    yyout = output_file;
    return yyparse();
}

int yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    return 1;
}
