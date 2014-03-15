CC       = gcc
CC_FLAGS = -c -d -Wall -Wextra -pedantic
LX       = flex
YC       = bison -d -y
EXEC     = tiny
SRC      = src/
LEX      = src/tiny.lex
YACC     = src/tiny.yacc
CPP      = src/lex.yy.c src/y.tab.c

all: lex.yy.c src/y.tab.c

$(EXEC): $(CPP)
	$(CC) $(CPP) -o $@

lex.yy.c: $(LEX)
	$(LX) $(LEX) && mv lex.yy.c $(SRC)lex.yy.c

y.tab.c: $(YACC)
	$(YC) $(YACC)

clean:
	rm src/lex.yy.c src/y.tab.c src/y.tab.h $(EXEC)
