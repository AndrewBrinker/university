%option noyywrap
%option yylineno

%{
#include <stdio.h>
static FILE *file = fopen("temp.txt", "w");
%}

NUMBER   [1-9][0-9]*
IDENT    [a-zA-Z_][a-zA-Z0-9_]*
LINE     \n
STRING   \"(\\.|[^"])*\"
KEYWORD  alignas|alignof|and|and_eq|asm|auto|bitand|bitor|bool|break|case|catch|char|char16_t|char32_t|class|compl|const|constexpr|const_cast|continue|decltype|default|delete|do|double|dynamic_cast|else|enum|explicit|export|extern|false|float|for|friend|goto|if|inline|int|long|mutable|namespace|new|noexcept|not|not_eq|nullptr|operator|or|or_eq|private|protected|public|register|reinterpret_cast|return|short|signed|sizeof|static|static_assert|static_cast|struct|switch|template|this|thread_local|throw|true|try|typedef|typeid|typename|union|unsigned|using|virtual|void|volatile|wchar_t|while|xor|xor_eq

%%

{STRING}  {}
{KEYWORD} {}
{IDENT}   { fprintf(file, "%d %s\n", yylineno, yytext); }
{NUMBER}  {}
{LINE}    {}
.         {}

%%

int main(int argc, char *argv[]) {
    ++argv, --argc;
    if (argc > 0) {
        yyin = fopen(argv[0], "r");
    } else {
        yyin = stdin;
    }
    yylex();
}
