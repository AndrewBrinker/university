CCFLAGS  = -c -d -O2 -Wall -Wextra -Isrc -DNDEBUG $(OPTFLAGS)
YCFLAGS  = -d -y

CC       = clang
LX       = flex
YC       = bison

LEX_SRC  = src/tiny.lex
YACC_SRC = src/tiny.yacc

LEX      = lex.yy.c
YACC     = y.tab.c

SRC      = $(LEX) $(YACC)
SOURCES  = $(patsubst %,src/%,$(SRC))
TARGET   = $(patsubst %.yacc,%,$(YACC))

# target: all       Compile ./tiny
all: $(SOURCES) $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $@

$(LEX): $(LEX_SRC)
	$(LX) $(LEX_SRC)
	@mv lex.yy.c src/lex.yy.c

$(YACC): $(YACC_SRC)
	$(YC) $(YCFLAGS) $(YACC_SRC)
	@mv y.tab.c src/y.tab.c
	@mv y.tab.h src/y.tab.h

clean:
	@rm src/lex.yy.c src/y.tab.c src/y.tab.h
