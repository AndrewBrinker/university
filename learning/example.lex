%{
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

int line_num = 1;
%}

%%

[ \t] ;

sNaZZle {
    return SNAZZLE;
}

type {
    return TYPE;
}

end {
    return END;
}

[0-9]+\.[0-9]+ {
    yylval.f = atof(yytext);
    return FLOAT;
}

[0-9]+ {
    yylval.i = atoi(yytext);
    return INT;
}

[a-zA-Z0-9]+ {
    char *result = (char *) malloc((strlen(yytext) + 1) * sizeof(char));
    strcpy(result, yytext);
    yylval.s = result;
    return STRING;
}

"\n" {
    ++line_num;
    return ENDL;
}

. ;

%%
