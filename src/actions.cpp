#include "actions.h"

std::deque<Parameter> paramBuffer;
std::deque<std::string> identBuffer;
std::deque<PointerType> ptrBuffer;

Stack symbolTable;

template <class T>
bool searchStack(const char *ident, T *&castSymbol)
{
    Symbol *symbol = NULL;
    std::string identStr(ident);
    bool isFound = symbolTable.searchStack(identStr, symbol);
    castSymbol = dynamic_cast<T*>(symbol); // = foundType;
    return isFound;
}

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
            Parameter param(identBuffer.front(), type, true);
            paramBuffer.push_back(param);
            identBuffer.pop_front();
        }
    }
}

void createFunction(const char *ident, Function *&funcPtr)
{
    funcPtr = new Function(std::string(ident)); // NOTE May need to dynamically create?
}

void createFunctionWithParams(const char *ident, Function *&funcPtr)
{
    createFunction(ident, funcPtr);
    /* Add parameters */
    while (!paramBuffer.empty()) { // yacc error ::  yacc: e - line 374 of "grammar.y", $4 is untyped
        funcPtr->addParameter(paramBuffer.front());
        paramBuffer.pop_front();
    }
}

void createFunctionDecl(const char* ident, Function*& funcPtr)
{
    /* Check if return type is valid */
    Symbol *symbolType = NULL;
    symbolTable.searchStack(ident, symbolType);
    Type *type = dynamic_cast<Type*>(symbolType);
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
