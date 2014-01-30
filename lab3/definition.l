/* Set any options first. */
%option noyywrap

/* Any C code you want to include goes here. */
%{
#include <stdio.h>
%}

/* Define the identifiers. */
NUM         [1-9][0-9]*
IDENT       [a-zA-Z]([a-zA-Z]|')*

%%

{IDENT} {
    printf("\tI found an identifier: %s\n", yytext);
}

{NUM} {
    printf("\tI found a number: %s\n", yytext);
}

.

%%
