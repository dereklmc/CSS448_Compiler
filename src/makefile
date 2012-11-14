# This is a comment.
# A makefile must be called makefile or Makefile.  The command is 'make' .

# If you don't tell make what to make, it makes the first thing in this file.
# These 'targets' start in column 1 and are followed by a colon.  Following
# the colon are the dependencies (files).  If any file in the dependency list
# is updated, then 'make' executes the commands found on the following lines.  
# These commands _must_ start with a tab.

# The 'touch' command is useful for faking an update.  'touch y3.y' fools
# make into thinking that the file y3.y has been changed by updating its date.

OBJ = y.tab.o main.o lex.yy.o actions.o parameter.o pointertype.o procedure.o recordtype.o variable.o stdtype.o typesymbol.o   # all object files, the  .o  files
H = y.tab.h parser.h stack.h actions.h arrayType.h constant.h constvalue.h function.h Parameter.h PointerType.h Procedure.h range.h RecordType.h setType.h stackframe.h  stdProcedure.h stdType.h symbol.h Type.h TypeSymbol.h Variable.h
ACT = actions.h      

# The $ is a macro reference.  The following ident is replaced with the
# specified list.  Parens must be used if the ident is longer than one char.

a.out: $(OBJ) $H $(ACT)         # a.out depends on everything
	g++ -g $(OBJ) -o a.out

main.o: $H main.cpp                   # main.o depends on main.cpp
	g++ -g -c  main.cpp

parameter.o: $H parameter.cpp
	g++ -g -c parameter.cpp

pointertype.o: $H pointertype.cpp
	g++ -g -c pointertype.cpp

procedure.o: $H procedure.cpp
	g++ -g -c procedure.cpp

recordtype.o: $H recordtype.cpp
	g++ -g -c recordtype.cpp

variable.o: variable.cpp
	g++ -g -c variable.cpp

stdtype.o: stdtype.cpp
	g++ -g -c stdtype.cpp
	
typesymbol.o: typesymbol.cpp
	g++ -g -c typesymbol.cpp

actions.o: $(ACT) $H actions.cpp    # actions.o depends on the .h and .cpp
	g++ -g -c actions.cpp

lex.yy.o: $H lex.yy.c                 # lex.yy.o depend on lex.yy.c
	g++ -g -c lex.yy.c         # include this if you used (f)lex

lex.yy.c: $H lex.l                 # lex.yy.c depend on the lex file, lex.l
	flex lex.l                 # include only if you used (f)lex

y.tab.o: y.tab.c                   # y.tab.o depends on y.tab.c
	g++ -g -c y.tab.c

y.tab.c: grammar.y                 # y.tab.c depends on the yacc file, y   
	yacc -d -v grammar.y       # -d separates out defines into .h file

clean:
	rm *.o y.tab.c y.tab.h lex.yy.c a.out
