# A makefile must be called makefile or Makefile.  The command is 'make' .

# If you don't tell make what to make, it makes the first thing in this file.
# These 'targets' start in column 1 and are followed by a colon.  Following
# the colon are the dependencies (files).  If any file in the dependency list
# is updated, then 'make' executes the commands found on the following lines.  
# These commands _must_ start with a tab.

# The 'touch' command is useful for faking an update.  'touch y3.y' fools
# make into thinking that the file y3.y has been changed by updating its date.

OBJ = y.tab.o main.o lex.yy.o actions.o parameter.o pointertype.o procedure.o recordtype.o variable.o stdtype.o typesymbol.o constant.o constvalue.o symbolictype.o constrange.o symbol.o# all object files, the  .o  files

H = y.tab.h parser.h actions.h

ACT = actions.h stack.h stackframe.h

STH = stack.h arraytype.h constant.h constvalue.h function.h parameter.h PointerType.h procedure.h range.h RecordType.h setType.h stackframe.h stdtype.h symbolictype.h Type.h constrange.h sep.h symbol.h

# The $ is a macro reference.  The following ident is replaced with the
# specified list.  Parens must be used if the ident is longer than one char.

a.out: $(OBJ) $H $(ACT) $(STH)       # a.out depends on everything
	g++ -g $(OBJ) -o a.out

main.o: $H main.cpp                   # main.o depends on main.cpp
	g++ -g -c  main.cpp

constant.o: constant.cpp constant.h symbol.h
	g++ -g -c $<

symbolictype.o: symbolictype.cpp symbolictype.h symbol.h Type.h
	g++ -g -c $<

constvalue.o: constvalue.cpp constvalue.h
	g++ -g -c $<

constrange.o: constrange.cpp constrange.h
	g++ -g -c $<

parameter.o: parameter.cpp parameter.h variable.h Type.h
	g++ -g -c $<

pointertype.o: pointertype.cpp Type.h
	g++ -g -c $<

procedure.o: procedure.cpp
	g++ -g -c $<

recordtype.o: recordtype.cpp Type.h
	g++ -g -c $<

variable.o: variable.cpp Type.h
	g++ -g -c $<

stdtype.o: stdtype.cpp Type.h
	g++ -g -c $<
	
typesymbol.o: typesymbol.cpp Type.h
	g++ -g -c $<
	
symbol.o: symbol.cpp symbol.h Type.h
	g++ -g -c $<

actions.o: $(ACT) $(STH) $H actions.cpp    # actions.o depends on the .h and .cpp
	g++ -g -c actions.cpp

lex.yy.o: lex.yy.c parser.h y.tab.h # lex.yy.o depend on lex.yy.c
	g++ -g -c $<                    # include this if you used (f)lex

lex.yy.c: lex.l                     # lex.yy.c depend on the lex file, lex.l
	flex lex.l                      # include only if you used (f)lex

y.tab.o: y.tab.c $H                   # y.tab.o depends on y.tab.c
	g++ -g -c y.tab.c

y.tab.c: grammar.y                  # y.tab.c depends on the yacc file, y   
	yacc -d -v grammar.y            # -d separates out defines into .h file

clean:
	rm *.o y.tab.c y.tab.h lex.yy.c a.out
