%{
    #include <stdio.h>
    #include <math.h>
    #include "calc.c"
    int yylex(void);
    void yyerror(char const *);
%}

%define api.value.type union
%token <double>  NUM
%token <symrec*> VAR FNCT
%type  <double>  exp

%precedence '='
%left '-' '+'
%left '*' '/'
%precendence NEG
%right '^'

%%

input:
    /* empty */ |
    input line;

line:
    '\n' |
    exp '\n' {
        printf("%.10g\n", $1);
    } |
    error '\n' {
        yyerok;
    };

exp:
    NUM {
        $$ = $1;
    } |
    VAR {
        $$ = $1->value.var;
    } |
    VAR '=' exp {
        $$ = $3; $1->value.var = $3;
    } |
    FNCT '(' exp ')' {
        $$ = (*($1->value.functptr))($3);
    } |
    exp '+' exp {
        $$ = $1 + $3;
    } |
    exp '-' exp {
        $$ = $1 - $3;
    } |
    exp '*' exp {
        $$ = $1 * $3;
    } |
    exp '/' exp {
        $$ = $1 / $3;
    } |
    '-' exp %prec NEG {
        $$ = -$2;
    } |
    exp '^' exp {
        $$ = pow($1, $3);
    } |
    '(' exp ')' {
        $$ = $2;
    };

%%

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}

int main(int argc, const char **argv) {
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-p")) {
            yydebug = 1;
        }
    }
    init_table();
    return yyparse();
}

int yylex(void) {
    int c;
    while ((c = getchar()) == ' ' || c == '\t') {
        continue;
    }
    if (c == EOF) return 0;
    if (c == '.' || isdigit (c)) {
        ungetc(c, stdin);
        scanf("%lf", &yylval.NUM);
        return NUM;
    }
    if (isalpha(c)) {
        static size_t length = 40;
        static char *symbuf = 0;
        symrec *s;
        int i;
        if (!symbuf) {
            symbuf = (char *) malloc(length + 1);
        }
        i = 0;
        do {
            if (i == length) {
                length *= 2;
                symbuf = (char *) realloc(symbuf, length + 1);
            }
            symbuf[i++] = c;
            c = getchar();
        } while (isalnum (c));
        ungetc(c, stdin);
        symbuf[i] = '\0';
        s = getsym (symbuf);
        if (s == 0) {
            s = putsym (symbuf, VAR);
        }
        *((symrec**) &yylval) = s;
        return s->type;
    }
    return c;
}




