%{
#include <stdlib.h>
#include "y.tab.h"

extern YYSTYPE yylval;
%}

%%

" " ;

[A-Z] {
    yylval.c = yytext[0];
    return LETTER;
}

[0-9]+\.[0-9]+ {
    yylval.f = atof(yytext);
    return DECIMAL;
}

[0-9]+ {
    yylval.i = atoi(yytext);
    return INTEGER;
}
