#include "actions.h"

template <class T>
bool searchStack(const char *ident, T *&castSymbol)
{
    Symbol *symbol = NULL;
    std::string identStr(ident);
    bool isFound = scopeStack.searchStack(identStr, symbol);
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
        while (!identQueue.empty()) 
        {
            Parameter param(identQueue.front(), type, true);
            parameterQueue.push_back(param);
            identQueue.pop_front();
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
    while (!parameterQueue.empty()) { // yacc error ::  yacc: e - line 374 of "grammar.y", $4 is untyped
        funcPtr->addParameter(parameterQueue.front());
        parameterQueue.pop_front();
    }
}

void createFunctionDecl(const char* ident, Function*& funcPtr)
{
    /* Check if return type is valid */
    Symbol *symbolType = NULL;
    scopeStack.searchStack(ident, symbolType);
    Type *type = dynamic_cast<Type*>(symbolType);
    /* Put function in parent scope */
    scopeStack.current->addSymbol(funcPtr);
    /* Enter Function Scope */
    scopeStack.createScope(funcPtr->name);
    /* Put procedure params on symbol stack. */
    std::vector<Parameter> toPutOnStack = funcPtr->getParameters();
    for (int i = 0; i < toPutOnStack.size(); i++) {
        scopeStack.current->addSymbol(&toPutOnStack[i]);
    }

}

void createProcedureDecl(const char* ident)
{
    /* TODO: put in actions.cpp */
    /* Put procedure in parent scope */
    scopeStack.current->addSymbol(ident);
    /* Enter Procedure Scope */
    scopeStack.createScope(ident->name);
    /* Put procedure params on symbol stack. */
    std::vector<Parameter> toPutOnStack = ident->getParameters();
    for (int i = 0; i < toPutOnStack.size(); i++) {
        scopeStack.current->addSymbol(&toPutOnStack[i]);
    }

}

void exitScope()
{
    /* Exit Function scope */
    StackFrame *scope = scopeStack.leaveScope();
    /* Print exited scope. */
    cout << scope;
    /* Mem management */
    delete scope;
    scope = NULL;
}
