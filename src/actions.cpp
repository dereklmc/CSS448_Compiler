#include "actions.h"

#include <iostream>

std::deque<Parameter*> paramBuffer;
std::deque<std::string> identBuffer;
std::deque<PointerType*> ptrBuffer;
std::deque<Range*> rangeBuffer;
std::deque<Variable*> variableBuffer;

Stack symbolTable;

void addIdent(const char *ident)
{
    identBuffer.push_back(std::string(ident));
}

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
template <class T>
bool searchStack(const char *ident, T *&castSymbol)
{
    Symbol *symbol = NULL;
    std::string identStr(ident);
    bool isFound = symbolTable.searchStack(identStr, symbol);
    if (!isFound) {
        castSymbol = NULL;
        std::cout << "Ident \"" << ident << "\" does not name a symbol." << std::endl;
    } else {
        castSymbol = dynamic_cast<T*>(symbol); // = foundType;
    }
    return isFound;
}

void createProgramScope(const char *ident) {
    std::string scopeName(ident);
    while (!identBuffer.empty()) {
	identBuffer.pop_front();
    }	
    symbolTable.createScope(scopeName);
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
    TypeSymbol *type = NULL;
    bool isFound = searchStack<TypeSymbol>(ident, type);

    /* Create parameters and add to parameter queue */  
    if (isFound)
    {
        while (!identBuffer.empty()) 
        {
			// Check if name is already taken
			Type *varType = new SymbolicType(type);
            Parameter *param = new Parameter(identBuffer.front(), varType, true);
            paramBuffer.push_back(param);
            identBuffer.pop_front();
        }
    } else {
        std::cout << "ERROR! Type \"" << ident << "\" is undefined." << std::endl;
    }
}

void createProcedure(const char *ident, Procedure *&procedurePtr)
{
    procedurePtr = new Procedure(std::string(ident));
    /* Add parameters */
    while (!paramBuffer.empty()) {
        procedurePtr->addParameter(paramBuffer.front());
        paramBuffer.pop_front();
    }
}

/******************************************************************************
 * createFunction
 * Takes in an array of chars which will represent the name of the new function
 * and a pointer to a reference of a Function object that will be initialized
 * with that name. It pops all of the parameters off of the paramBuffer, if any,
 * and adds them to the function.
 *****************************************************************************/
void createFunction(const char *ident, Function *&funcPtr)
{
    funcPtr = new Function(std::string(ident));
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
    TypeSymbol *symbolType = NULL;
    bool found = searchStack<TypeSymbol>(ident, symbolType);
    // TODO print error message if bad type was found.
    funcPtr->setType(new SymbolicType(symbolType));
    
    /* Put function in parent scope */
    symbolTable.current->addSymbol(funcPtr);
    /* Enter Function Scope */
    symbolTable.createScope(funcPtr->name);
    /* Put procedure params on symbol stack. */
    std::vector<Parameter*> toPutOnStack = funcPtr->getParameters();
    for (int i = 0; i < toPutOnStack.size(); i++) {
        Symbol *paramVarSymbol = toPutOnStack[i]->getVariable();
        symbolTable.current->addSymbol(paramVarSymbol);
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
    std::vector<Parameter*> toPutOnStack = ident->getParameters();
    for (int i = 0; i < toPutOnStack.size(); i++) {
        Symbol *paramVarSymbol = toPutOnStack[i]->getVariable();
        symbolTable.current->addSymbol(paramVarSymbol);
    }
}

void createTypeSymbol(const char *ident, Type *type)
{
    if (type != NULL) {
        std::string name(ident);
        Symbol *symbol = new TypeSymbol(name, type);
        symbolTable.current->addSymbol(symbol);
    }
}

void createPointer(Type*& createdType, const char *ident)
{
    // Find symbol for ident
    Symbol *s = new Symbol(ident);
    
    PointerType *ptr = new PointerType(s);
    ptrBuffer.push_back(ptr);
    createdType = ptr;
}

void getSymbolicType(Type *&type, const char *name)
{
    TypeSymbol *typeSymbol = NULL;
    bool isFound = searchStack<TypeSymbol>(name, typeSymbol);
    if (isFound && typeSymbol != NULL) {
        type = new SymbolicType(typeSymbol);
    } else {
        type = NULL;
        std::cout << "ERROR! Type \"" << name << "\" is undefined." << std::endl;
    }
}

void createArrayType(ArrayType *&arrayType, Type *contentsType)
{
    arrayType = new ArrayType(contentsType);
    /* Add ranges */
    while (!rangeBuffer.empty()) {
        arrayType->addRange(rangeBuffer.front());
        rangeBuffer.pop_front();
    }
}

void createSetType(Type *&createdType)
{
    createdType = new SetType(rangeBuffer.front());
    rangeBuffer.pop_front();
}

void createStringRange(const char* start, const char* stop) {
    Range *range = new CharRange(start[0], stop[0]);
    rangeBuffer.push_back(range);
}

void createConstRange(ConstValue *start, ConstValue *stop) {
    Range *range = new ConstRange(start, stop);
    rangeBuffer.push_back(range);
}

void createVariableList(Type *&type) {
    if (type != NULL) {
        while (!identBuffer.empty()) {
            std::string ident = identBuffer.front();

            identBuffer.pop_front();
            
            Variable* var = new Variable(ident,type->clone());
            variableBuffer.push_back(var);
        }
        delete type;
        type = NULL;
    }
}

void createVariables(Type *&type) {
    if (type != NULL) {
        while (!identBuffer.empty()) {
            std::string ident = identBuffer.front();
	   
            identBuffer.pop_front();
            
            Variable* var = new Variable(ident,type->clone());
	        symbolTable.current->addSymbol(var);
        }
        delete type;
        type = NULL;
    }
}

void createRecordType(Type *&createdType) {
    RecordType *record = new RecordType(symbolTable.currentScope);
    while (!variableBuffer.empty()) {
        Variable *var = variableBuffer.front();
        if (!record->addField(var)) {
            std::cerr << "ERROR: " << var->name << "already exists in record" << std::endl;
            delete var;
            var = NULL;
        }
        variableBuffer.pop_front();
    }
    createdType = record;
}

void createConstValue(ConstValue *&constValue, const char *value, ConstValueType type) {
    std::string strValue(value);
    constValue = new ConstValue(strValue, type);
}

void createConstant(const char *ident, ConstValue *&value) {
    Constant *symbol = new Constant(std::string(ident), value);
    symbolTable.current->addSymbol(symbol);
    std::cout << symbol->generateCode() << std::endl;
}

void checkPointers()
{
    while(!ptrBuffer.empty())
    {
        PointerType* ptr = ptrBuffer.front();
        Symbol* temp = NULL;
        symbolTable.searchStack(ptr->getPointee()->name, temp);
        if(temp != NULL)
        {
            //std::cout << "POINTEE FOUND!" << std::endl;
        }
     
        ptrBuffer.pop_front();
    }
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
    std::cout << *scope;
    /* Mem management */
    delete scope;
    scope = NULL;
}
