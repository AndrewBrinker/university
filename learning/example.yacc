%{
#include <stdio.h>

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int line_num;

int yyerror(char *s);
%}

%union {
    int i;
    float f;
    char *s;
}

%token <i> INT
%token <f> FLOAT
%token <s> STRING
%token SNAZZLE TYPE
%token END ENDL

%%

snazzle:        header template body_section footer {
                    printf("done with a snazzle file\n");
                };

header:         SNAZZLE FLOAT endls {
                    printf("reading a snazzle file version %f\n", $2);
                };

template:       typelines;


typelines:      typelines typeline |
                typeline;

typeline:       TYPE STRING endls {
                    printf("new defined snazzle type: %s\n", $2);
                };

body_section:   body_lines;

body_lines:     body_lines body_line |
                body_line;

body_line:      INT INT INT INT STRING endls {
                    printf("new snazzle %d %d %d %d %s\n", $1, $2, $3, $4, $5);
                };

footer:         END ENDL;

endls:          endls ENDL |
                ENDL;

%%

int main() {
    FILE *myfile = fopen("in.txt", "r");
    if (!myfile) {
        printf("I can't open snazzle.txt!\n");
        return -1;
    }
    yyin = myfile;
    do {
        yyparse();
    } while (!feof(yyin));
}

int yyerror(char *s) {
    printf("%s @ %d\n", s, line_num);
    return 1;
}

int yywrap() {
    return 1;
}
