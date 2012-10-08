H = tokenconsts.h analyzer.h
SRC = analyzer.lex
LEX = lex.yy.c
APP = analyzer.run

lex.run: $H $(LEX)
	g++ $(LEX) -o $(APP) -lfl

$(LEX): $(SRC)
	flex -o $(LEX) $(SRC)
