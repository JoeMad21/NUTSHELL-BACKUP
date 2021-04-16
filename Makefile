all: flex-config bison-config lexer nutshell

bison-config:
	bison -d parser.y

flex-config: lexer.l
	flex lexer.l

lexer: lex.yy.c
	g++ -c lex.yy.c -o lexer.lex.o

nutshell: nutshell.cpp lex.yy.c
	g++ nutshell.cpp parser.tab.c lex.yy.c -o nutshell
clean:
	rm *.yy.c
	rm *.tab.c
	rm *.tab.h
	rm *.o
	rm nutshell
