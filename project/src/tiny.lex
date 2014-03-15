%{
#include <stdlib.h>
#include "y.tab.h"
extern YYSTYPE yylval;
int line_number = 1;
%}

%%

[ \t] ;

"<"     { return LT;    }
"<="    { return LE;    }
">"     { return GT;    }
">="    { return GE;    }
"=="    { return EQ;    }
"!="    { return NQ;    }
"PRINT" { return PRINT; }
"IF"    { return IF;    }
"THEN"  { return THEN;  }
"GOTO"  { return GOTO;  }
"INPUT" { return INPUT; }
"LET"   { return LET;   }
"END"   { return END;   }

[A-Z] {
    yylval.c = yytext[0];
    return LETTER;
}

[0-9]+\.[0-9]+ {
    yylval.d = atof(yytext);
    return DECIMAL;
}

[1-9][0-9]+ {
    yylval.i = atoi(yytext);
    return INTEGER;
}

"\n" {
    ++line_number;
    return NEWLINE;
}

%%
