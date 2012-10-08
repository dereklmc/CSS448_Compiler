H = src/tokenconsts.h src/analyzer.h
SRC = src/analyzer.lex
LEX = src/lex.yy.c
APP = bin/analyzer.run

lex.run: $H $(LEX)
	g++ $(LEX) -o $(APP) -lfl

$(LEX): $(SRC)
	flex -o $(LEX) $(SRC)
