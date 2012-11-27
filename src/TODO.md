# TODO List

1. <s>_Hugo_ Change Union</s>
    - <s>%type <arraytype> ArrayType</s>
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
    - Generate them! (Do more when Types are figured out)
    - One variable per line.
6. _Nina ;->)_ Calling Procedures
    - Make sure we are still discarding input/output variables.
        - __EMAIL ZANDER!__
7. <s>_Hugo_ Errors</s>
    - <s>Should we spit out code if we have determined that the expression might be illegal?</s>
        - <s>__EMAIL ZANDER!__</s> __CONTINUE ON!__
8. <s> _Derek_ Modify grammar so I/O statements are standard procedures in the SymbolTable </s>
    Must be baked into grammar; but, type checking is not necessary as Pascal is writing out vars.
9. Will be outputting curly-braces for all if/while (one line or not)
10. _Derek_ Constants
    - Implement Symbol#isConstant bool
        - Defaults to false
    - Figure out how to generate Constants
        - Look at the Dragon Book
        
11. Create a PascalArray (template class - params type of array, and type of indices)
	- Defined in header file
	- Has an index converter (0 based)
	- Used for code generation
	- Recursively generate for multi-dimensional arrays
