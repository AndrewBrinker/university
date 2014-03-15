y.tab.c y.tab.h: example.yacc
	yacc -d example.yacc

lex.yy.c: example.lex y.tab.h
	lex example.lex

snazzle: lex.yy.c y.tab.c y.tab.h
	gcc y.tab.c lex.yy.c -ll -o snazzle
