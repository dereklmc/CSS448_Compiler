#include "actions.h"

template <class T>
bool searchStack(const char *ident, T *&castSymbol)
{
    Symbol *symbol = NULL;
    bool isFound = scopeStack.searchStack(std::string(ident), symbol);
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
