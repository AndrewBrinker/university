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
%type <s> expression term factor number var relop exprlist varlist

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
                char *label = get_label();
                fprintf(yyout, "%s: ", label);
            } statement endl |
            statement endl;

statement:  PRINT exprlist |
            IF expression relop expression THEN statement |
            GOTO expression |
            INPUT varlist |
            LET var ASSIGN expression {
                chomp($4, strlen($4));
                fprintf(yyout, "auto %c = %s;\n", $2[0], $4);
            } |
            END;

exprlist:   exprlist COMMA expression {
                char *combined = string_combine($1, ", " , $3);
                // printf("(Yacc) EXPRLIST: %s\n", combined);
                $$ = combined;
                free(combined);
            } |
            expression;

varlist:    varlist COMMA var {
                char *combined = string_combine($1, ", " , $3);
                // printf("(Yacc) VARLIST: %s\n", combined);
                $$ = combined;
                free(combined);
            } |
            var;

expression: expression PLUS term {
                char *combined = string_combine($1, " + " , $3);
                // printf("(Yacc) PLUS: %s\n", combined);
                $$ = combined;
                free(combined);
            } |
            expression MINUS term {
                char *combined = string_combine($1, " - " , $3);
                // printf("(Yacc) MINUS: %s\n", combined);
                $$ = combined;
                free(combined);
            } |
            term;

term:       term TIMES factor {
                char *combined = string_combine($1, " * " , $3);
                // printf("(Yacc) TIMES: %s\n", combined);
                $$ = combined;
                free(combined);
            } |
            term DIVIDED_BY factor {
                char *combined = string_combine($1, " / " , $3);
                // printf("(Yacc) DIVIDED_BY: %s\n", combined);
                $$ = combined;
                free(combined);
            } |
            factor;

factor:     var |
            number |
            LEFT_PAREN expression RIGHT_PAREN {
                // printf("(Yacc) %s\n", $2);
                char *combined = string_combine("(", $2 , ")");
                $$ = combined;
                free(combined);
            };

number:     INTEGER {
                // printf("(Yacc) INT:   %s\n", $1);
                $$ = $1;
            } |
            DECIMAL {
                // printf("(Yacc) FLOAT: %s\n", $1);
                $$ = $1;
            };

var:        LETTER {
                // printf("(Yacc) CHAR:  %s\n", $1);
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
    return yyparse();
}

int yyerror(char *s) {
    printf("[%d] %s\n", line_number, s);
    return 1;
}

int yywrap() {
    return 1;
}
