%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/filename.c"
#include "../lib/chomp.c"
#include "../lib/label.c"

extern FILE *yyin;
extern int line_number;

FILE *yyout;

int yylex();
int yyerror(char *s);
int yywrap();
%}

%start program

%union {
    char *s;
}

%type <s> INTEGER DECIMAL LETTER
%type <s> expression term factor number var relop exprlist varlist statement

%token INTEGER LETTER DECIMAL
%token GOTO IF THEN LET PRINT END INPUT
%token NEWLINE ASSIGN COMMA
%token PLUS MINUS TIMES DIVIDED_BY LEFT_PAREN RIGHT_PAREN

%left '=' ','
%left LT LE GT GE EQ NQ
%left '+' '-'
%left '*' '/'

%%


program:    block;

block:      block line |
            line;

line:       INTEGER {
                label *label = putlabel(atoi($1));
                fprintf(yyout, "%s: ", label->name);
            } statement endl |
            statement endl;

statement:  PRINT exprlist |
            IF expression relop expression {
                printf("%s\n", $2);
                size_t length = strlen($2);
                size_t diff = strlen("THEN");
                $2[length - 1 - diff] = '\0';
                fprintf(yyout, "if (%s) {\n", $2);
            } THEN statement {
                fprintf(yyout, "}\n");
            } |
            GOTO expression {
                chomp($2, strlen($2));
                fprintf(yyout, "goto %s;\n", $2);
            } |
            INPUT varlist |
            LET var ASSIGN expression {
                chomp($4, strlen($4));
                fprintf(yyout, "auto %c = %s;\n", $2[0], $4);
            } |
            END;

exprlist:   exprlist COMMA expression {
                char *combined = string_combine($1, ", " , $3);
                $$ = combined;
                free(combined);
            } |
            expression {
                $$ = $1;
            };

varlist:    varlist COMMA var {
                char *combined = string_combine($1, ", " , $3);
                $$ = combined;
                free(combined);
            } |
            var {
                $$ = $1;
            };

expression: expression PLUS term {
                char *combined = string_combine($1, " + " , $3);
                $$ = combined;
                free(combined);
            } |
            expression MINUS term {
                char *combined = string_combine($1, " - " , $3);
                $$ = combined;
                free(combined);
            } |
            term {
                $$ = $1;
            };

term:       term TIMES factor {
                char *combined = string_combine($1, " * " , $3);
                $$ = combined;
                free(combined);
            } |
            term DIVIDED_BY factor {
                char *combined = string_combine($1, " / " , $3);
                $$ = combined;
                free(combined);
            } |
            factor {
                $$ = $1;
            };

factor:     var {
                $$ = $1;
            } |
            number {
                $$ = $1;
            } |
            LEFT_PAREN expression RIGHT_PAREN {
                char *combined = string_combine("(", $2 , ")");
                $$ = combined;
                free(combined);
            };

number:     INTEGER {
                $$ = $1;
            } |
            DECIMAL {
                $$ = $1;
            };

var:        LETTER {
                $$ = $1;
            };

relop:      LT {
                $$ = "<";
            } | LE {
                $$ = "<=";
            } | GT {
                $$ = ">";
            } | GE {
                $$ = ">=";
            } | EQ {
                $$ = "==";
            } | NQ {
                $$ = "!=";
            };

endl:       endl NEWLINE |
            NEWLINE;

%%

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *input_file_name  = argv[1];
    char *output_file_name = convert_file_name(input_file_name);

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
    yyout = output_file;

    free(output_file_name);
    fprintf(yyout, "int main() {\n");
    int result = yyparse();
    fprintf(yyout, "}\n");
    return result;
}

int yyerror(char *s) {
    printf("[%d] %s\n", line_number, s);
    return 1;
}

int yywrap() {
    return 1;
}
