#include "actions.h"

std::deque<Parameter> paramBuffer;
std::deque<std::string> identBuffer;
std::deque<PointerType> ptrBuffer;

Stack symbolTable;

template <class T>

/******************************************************************************
 * searchStack
 * Takes in a char array representing the name that will be searched for in
 * the Symbol Table and a pointer to a reference that will be cast into the
 * proper Symbol type.
 * Using Stack's searchStack method, it will attempt to find a Symbol matching
 * the name that was passed in. Once found it will be dynamically cast into
 * the proper type.
 * Returns: bool isFound - true if a symbol with a matching name was found in
 *					the symbol table
 *****************************************************************************/
bool searchStack(const char *ident, T *&castSymbol)
{
    Symbol *symbol = NULL;
    std::string identStr(ident);
    bool isFound = symbolTable.searchStack(identStr, symbol);
    castSymbol = dynamic_cast<T*>(symbol); // = foundType;
    return isFound;
}

void createProgram(const char* ident)
{
	std::string programName(ident);
	/* Enter program scope */
	symbolTable.createScope(programName);

	/* Put program parameters on the stack */
	while (!identBuffer.empty())
	{
		//TODO : are we going to put the program parameters in the symbol
		// 		 table?
		identBuffer.pop_front();
	}
}

void addToIdentBuffer(const char* ident)
{
	std::string identStr(ident);
	identBuffer.push_back(identStr);
}


/******************************************************************************
 * createParameter
 * Attempts to create parameters based on a char array representing the type
 * the parameters will be. 
 * It first searches the symbol table by name to see if the type already 
 * exists. If found, one by one, names will be popped off the identBuffer and
 * added to a new parameter object. This object will then get added to the
 * paramBuffer.
 *****************************************************************************/
void createParameter(const char* ident)
{
    //printf("%s ", ident);
    /* Search Symbol Table for Type corresponding to yident. */
    Type *type = NULL;
    bool isFound = searchStack<Type>(ident, type);

    /* Create parameters and add to parameter queue */  
    if (isFound)
    {
        while (!identBuffer.empty()) 
        {
			// Check if name is already taken
            Parameter param(identBuffer.front(), type, true);
            paramBuffer.push_back(param);
            identBuffer.pop_front();
        }
    }
}

/******************************************************************************
 * createFunction
 * Takes in an array of chars which will represent the name of the new function
 * and a pointer to a reference of a Function object that will be initialized
 * with that name.
 *****************************************************************************/
void createFunction(const char *ident, Function *&funcPtr)
{
    funcPtr = new Function(std::string(ident)); // NOTE May need to dynamically create?
}

/******************************************************************************
 * createFunctionWithParams
 * Like createFunction, this method takes in an array of chars that will 
 * represent the name of the new function and a pointer to a reference of a 
 * Function object that will be initialized with that name. After calling
 * createFunction to initialize its funcPtr, it pops all of the parameters off
 * of the paramBuffer and adds them to the function.
 *****************************************************************************/
void createFunctionWithParams(const char *ident, Function *&funcPtr)
{
    createFunction(ident, funcPtr);
    /* Add parameters */
    while (!paramBuffer.empty()) { // yacc error ::  yacc: e - line 374 of "grammar.y", $4 is untyped
        funcPtr->addParameter(paramBuffer.front());
        paramBuffer.pop_front();
    }
}

/******************************************************************************
 * createFunctionDecl
 * Takes in a char array that represents the name of the return type of 
 * the function and a pointer to a reference of a Function object.
 * It first checks for a Symbol already in the Symbol Table with a name 
 * matching the one passed in. 
 * It then proceeds to add the function object to the current scope, then 
 * creates a new scope for the function. Lastly, it gets the Function's 
 * parameters and adds them to the new scope.
 *****************************************************************************/
void createFunctionDecl(const char* ident, Function*& funcPtr)
{
    /* Check if return type is valid */
    Symbol *symbolType = NULL;
    symbolTable.searchStack(ident, symbolType);
    Type *type = dynamic_cast<Type*>(symbolType); // Wasn't this already dynamically cast
													// in searchStack?
    /* Put function in parent scope */
    symbolTable.current->addSymbol(funcPtr);
    /* Enter Function Scope */
    symbolTable.createScope(funcPtr->name);
    /* Put procedure params on symbol stack. */
    std::vector<Parameter> toPutOnStack = funcPtr->getParameters();
    for (int i = 0; i < toPutOnStack.size(); i++) {
        symbolTable.current->addSymbol(&toPutOnStack[i]);
    }

}

/******************************************************************************
 * createProcedureDecl
 * Takes in a pointer to a Procedure object.
 * It adds the Procedure to the current scope, then creates a new scope. 
 * Lastly, it gets the Procedure object's parameters (if any) and adds them
 * to the new current Procedure scope.
 *****************************************************************************/
void createProcedureDecl(Procedure* ident)
{
    /* TODO: put in actions.cpp */
    /* Put procedure in parent scope */
    symbolTable.current->addSymbol(ident);
    /* Enter Procedure Scope */
    symbolTable.createScope(ident->name);
    /* Put procedure params on symbol stack. */
    std::vector<Parameter> toPutOnStack = ident->getParameters();
    for (int i = 0; i < toPutOnStack.size(); i++) {
        symbolTable.current->addSymbol(&toPutOnStack[i]);
    }

}

void createPointer(const char* n, Symbol*& pointee)
{
	PointerType ptr(n, pointee);
	ptrBuffer.push_back(ptr);
}

/******************************************************************************
 * exitScope
 * Calls the symbol table's leaveScope() method, which returns the stackframe
 * scope that is being exited. The contents of this scope is then printed out,
 * and the StackFrame object is deleted.
 *****************************************************************************/
void exitScope()
{
    /* Exit Function scope */
    StackFrame *scope = symbolTable.leaveScope();
    /* Print exited scope. */
    std::cout << scope;
    /* Mem management */
    delete scope;
    scope = NULL;
}
