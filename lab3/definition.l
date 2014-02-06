/* Set any options first. */
%option noyywrap
%option yylineno

/* Any C++ code you want to include goes here. */
%{
#include <iostream>
#include <vector>
#include <string>
#include <iterator>

int lines = 0;

static const char *keywords_array[] = {
    "alignas",         "alignof",         "and",             "and_eq",
    "asm",             "auto",            "bitand",          "bitor",
    "bool",            "break",           "case",            "catch",
    "char",            "char16_t",        "char32_t",        "class",
    "compl",           "const",           "constexpr",       "const_cast",
    "continue",        "decltype",        "default",         "delete",
    "do",              "double",          "dynamic_cast",    "else",
    "enum",            "explicit",        "export",          "extern",
    "false",           "float",           "for",             "friend",
    "goto",            "if",              "inline",          "int",
    "long",            "mutable",         "namespace",       "new",
    "noexcept",        "not",             "not_eq",          "nullptr",
    "operator",        "or",              "or_eq",           "private",
    "protected",       "public",          "register",        "reinterpret_cast",
    "return",          "short",           "signed",          "sizeof",
    "static",          "static_assert",   "static_cast",     "struct",
    "switch",          "template",        "this",            "thread_local",
    "throw",           "true",            "try",             "typedef",
    "typeid",          "typename",        "union",           "unsigned",
    "using",           "virtual",         "void",            "volatile",
    "wchar_t",         "while",           "xor",             "xor_eq"
};

static const std::vector<std::string> keywords(
    std::begin(keywords_array),
    std::end(keywords_array)
);

bool isReserved(char identifier[], unsigned size) {
    for (int i = 0; i < size; ++i) {
        if (identifier == keywords[i]) {
            return true;
        }
    }
    return false;
}
%}

/* Define the identifiers. */
NUM         [1-9][0-9]*
IDENT       [a-zA-Z_]([a-zA-Z_])*
LINE        \n

%%

{IDENT} {
    if (!isReserved(yytext, yyleng)) {
        printf("%s %d\n", yytext, yylineno);
    }
}

{LINE} {}

.

%%

int main(int argc, char **argv) {
    /* skip over program name */
    ++argv, --argc;
    if (argc > 0) {
        yyin = fopen(argv[0], "r");
    } else {
        yyin = stdin;
    }
    yylex();
}
