# TODO List

1. _Hugo_ Change Union
    - %type <arraytype> ArrayType
2. _Hugo_ Add to Type
    - virtual std::string Type::generateCode() const
3. _Nina_ Actions.h
    - void printCode(std::string code)
4. _Derek_ Types are hard, mmhkay?
    - Hold off on adding to symbol table until we check all the shiznit.
    - Hold off on code until aforementioned shiznit is checked.
    
    int numChanges;
    do:
        numChanges = 0;
        for typeSymbol in typeBuffer:
            if typeSymbol.hasType():
                    if typeSymbol.type in SymbolTable
                        numChanges++;
                        Add Type to SymbolTable;
                        GenerateCode();
                        buffer.remove(typeSymbol);
            else:
                numChanges++;
                Add Type to SymbolTable;
                GenerateCode();
                buffer.remove(typeSymbol);
    until numChanges == 0;
5. _Nina_ Variables
    - Generate them!
    - One variable per line.
6. _Nina ;->)_ Calling Procedures
    - Make sure we are still discarding input/output variables.
        - __EMAIL ZANDER!__
7. _Hugo_ Errors
    - Should we spit out code if we have determined that the expression might be illegal?
        - __EMAIL ZANDER!__
8. _Derek_ Modify grammar so I/O statements are standard procedures in the SymbolTable
9. Will be outputting curly-braces for all if/while (one line or not)
10. _Derek_ Constants
    - Implement Symbol#isConstant bool
        - Defaults to false
    - Figure out how to generate Constants
        - Look at the Dragon Book
