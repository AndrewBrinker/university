%{
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
extern YYSTYPE yylval;
%}

%%

[ \t] ;

"0" {
    return ZERO;
}

"1" {
    return ONE;
}

"e" {
    return EPSILON;
}

"\n" ;
. ;

%%
