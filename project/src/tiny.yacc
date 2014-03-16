%{
#include <stdio.h>
#include <stdlib.h>
#include "../lib/filename.c"

extern FILE *yyin;
extern int line_number;

FILE *yyout;

int yylex();
void chomp(char *str, size_t n);
int yyerror(char *s);
int yywrap();
%}

%start program

%union {
    char *s;
}

%type <s> INTEGER DECIMAL LETTER
%type <s> expression term factor number var

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

line:       INTEGER statement endl |
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

exprlist:   exprlist COMMA expression |
            expression;

varlist:    varlist COMMA var |
            var;

expression: expression PLUS term |
            expression MINUS term |
            term;

term:       term TIMES factor |
            term DIVIDED_BY factor |
            factor;

factor:     var |
            number |
            LEFT_PAREN expression RIGHT_PAREN {
                $$ = $2;
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

relop:      LT | LE | GT | GE | EQ | NQ;

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
    yyout = output_file;
    return yyparse();
}

void chomp(char *str, size_t n) {
    if (str[n-1] == '\n') str[n-1] = '\0';
}

int yyerror(char *s) {
    printf("[%d] %s\n", line_number, s);
    return 1;
}

int yywrap() {
    return 1;
}
