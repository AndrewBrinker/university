CC       = gcc
CC_FLAGS = -c -d -Wall -Wextra -pedantic
LX       = flex
YC       = bison -d -y
TARGET   = tiny
SRC_DIR  = src
LEX      = ${SRC_DIR}/tiny.lex
YACC     = ${SRC_DIR}/tiny.yacc
SOURCE   = src/lex.yy.c src/y.tab.c

all: lex.yy.c y.tab.c ${TARGET}

${TARGET}: ${SOURCE}
	${CC} ${SOURCE} -o $@

lex.yy.c: ${LEX}
	${LX} ${LEX}
	@mv lex.yy.c ${SRC_DIR}/lex.yy.c

y.tab.c: ${YACC}
	${YC} ${YACC}
	@mv y.tab.c ${SRC_DIR}/y.tab.c
	@mv y.tab.h ${SRC_DIR}/y.tab.h

clean:
	@rm src/lex.yy.c src/y.tab.c src/y.tab.h ${TARGET}
