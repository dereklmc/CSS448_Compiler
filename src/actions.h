#ifndef ACTIONS_H
#define ACTIONS_H

#include "y.tab.h"
#include "procedure.h"
#include "function.h"
#include "Type.h"
#include "stack.h"
#include "PointerType.h"
#include "TypeSymbol.h"
#include "arraytype.h"
#include "setType.h"
#include "RecordType.h"
#include "constvalue.h"
#include "constant.h"
#include "symbolictype.h"
#include "stdtype.h"
#include "constrange.h"

#include <deque>

extern Stack symbolTable;
extern int lineNumber;		// The current line of the program we are parsing

/******************************************************************************
 * addCaseLabel(ConstValue* c)
 * Takes a ConstValue and pushes it on the caseLabelBuffer. 
 *****************************************************************************/
void addCaseLabel(ConstValue* c);

/******************************************************************************
 * addIdent(const char*)
 * Takes a const char* array and pushes it on the identBuffer.
 *****************************************************************************/
void addIdent(const char *);

/******************************************************************************
 * addParameterType(Type *t)
 * Takes a Type and pushes it on the parameterTypeCheckBuffer.
 *****************************************************************************/
void addParameterType(Type* t);

/******************************************************************************
 * addError(std::string)
 * Takes a string and pushes it on the errorLog.
 *****************************************************************************/
void addError(std::string);

/******************************************************************************
 * checkConditionalExpressionType(Type*)
 * Compares the Type passed in to the standard BOOLEAN_TYPE. If it does not 
 * equal, then it pushes an error onto the errorLog.
 *****************************************************************************/
void checkConditionalExpressionType(Type*);

/******************************************************************************
 * checkForReturnValue(const char*)
 * Checks to see if the const char* passed in matches the current scope's name
 * If so, it means that this ident is going to be a return statement
 * Returns the correct return Type if true, NULL if not.
 *****************************************************************************/
Type* checkForReturnValue(const char*);

/******************************************************************************
 * checkPointers()
 * While the ptrBuffer is not empty, takes the pointer from the front and 
 * searches the symbol table for the pointee's name. Whether or not the pointee
 * is found in the symbol table, the pointer will be popped off the ptrBuffer.
 *****************************************************************************/
void checkPointers();

/******************************************************************************
 * compareParamTypes(std::vector<Type*> a)
 * Compares the collection of Types passed in to the contents of the
 * parameterTypeCheckBuffer. This method is used when checking to see if the
 * parameters being passed into a function or procedure call match the 
 * parameters of the procedure or function being called.
 *****************************************************************************/
void compareParamTypes(std::vector<Type*> a);

/******************************************************************************
 * createArrayType(ArrayType *&, Type*)
 * Instantiates the ArrayType object passed in with the Type* also passed in.
 * While there are still Range objects in the rangeBuffer, they will be popped
 * off and added to the ArrayType object.
 *****************************************************************************/
void createArrayType(ArrayType *&, Type *);

/******************************************************************************
 * createConstant(const char*, ConstValue*&)
 * Creates a new Constant object using the char* array and ConstValue passed
 * in. Then addes this Constant object to the current scope and generates code
 * for it.
 *****************************************************************************/
void createConstant(const char*, ConstValue*&);

/******************************************************************************
 * createConstRange(ConstValue*, ConstValue*)
 * Creates a new Range object using the ConstValue*s passed in. Then pushes 
 * this Range object onto the rangeBuffer.
 *****************************************************************************/
void createConstRange(ConstValue*, ConstValue*);

/******************************************************************************
 * createConstValue(ConstValue*&, const char*, ConstValueType)
 * Instantiates the ConstValue object passed in with the const char* array
 * and ConstValueType also passed in.
 ******************************************************************************/
void createConstValue(ConstValue*&, const char*, ConstValueType);

/******************************************************************************
 * createFunction(const char*, Function*&)
 * Takes in an array of chars which will represent the name of the new function
 * and a pointer to a reference of a Function object that will be initialized
 * with that name.
 *****************************************************************************/
void createFunction(const char*, Function*&);

/******************************************************************************
 * createFunctionDecl(const char*, Function*)
 * Takes in a char array that represents the name of the return type of 
 * the function and a pointer to a reference of a Function object.
 * It first checks for a Symbol already in the Symbol Table with a name 
 * matching the one passed in. 
 * It then proceeds to add the function object to the current scope, then 
 * creates a new scope for the function. Lastly, it gets the Function's 
 * parameters and adds them to the new scope.
 *****************************************************************************/
void createFunctionDecl(const char*, Function*);

/******************************************************************************
 * createFunctionWithParams(const char*, Function*&)
 * Like createFunction, this method takes in an array of chars that will 
 * represent the name of the new function and a pointer to a reference of a 
 * Function object that will be initialized with that name. After calling
 * createFunction to initialize its funcPtr, it pops all of the parameters off
 * of the paramBuffer and adds them to the function.
 *****************************************************************************/
void createFunctionWithParams(const char*, Function*&);

/******************************************************************************
 * createLoopCaseScope(const char*)
 * Creates a generic scope using the char* array passed in. Used for creating
 * scopes in loops, switch case statements, and if statements.
 *****************************************************************************/
void createLoopCaseScope(const char *ident);

/******************************************************************************
 * createParameter(const char*)
 * Attempts to create parameters based on a char array representing the type
 * the parameters will be. 
 * It first searches the symbol table by name to see if the type already 
 * exists. If found, one by one, names will be popped off the identBuffer and
 * added to a new parameter object. This object will then get added to the
 * paramBuffer.
 *****************************************************************************/
void createParameter(const char*);

/******************************************************************************
 * createPointer(Type*&, const char*)
 * A Symbol object is instantiated with the char* array for its name. A new
 * PointerType object is then instantiated using this Symbol object and
 * pushed onto the ptrBuffer. This pointer is also saved to the Type*& that
 * was passed in.
 *****************************************************************************/
void createPointer(PointerType*&, const char*);

/******************************************************************************
 * createProcedure(const char*, Procedure*&)
 * Instantiates the Procedure pointer passed in with the char* array passed in.
 * If there were any parameters placed in the paramBuffer, they are now 
 * popped off the buffer and added to the Procedure object.
 *****************************************************************************/
void createProcedure(const char*, Procedure*&);

/******************************************************************************
 * createProcedureDecl(Procedure*)
 * Takes in a pointer to a Procedure object.
 * It adds the Procedure to the current scope, then creates a new scope, gets 
 * the Procedure object's parameters (if any), adds them
 * to the new current Procedure scope, and generates the code for a procedure
 * declaration.
 *****************************************************************************/
void createProcedureDecl(Procedure*);

/******************************************************************************
 * createProgramScope(const char*)
 * This method is called whenever the grammar encounters a new program 
 * declaration. Converts the program name from a char* array to a string, 
 * discards any program parameters (at this point in time) and creates a new
 * scope with the program's name.
 *****************************************************************************/
void createProgramScope(const char*);

/******************************************************************************
 * createRecordType(Type *&)
 * Instantiates a new RecordType using the current scope. While there are 
 * variables on the variableBuffer, it pops them off and attempts to add 
 * them to the RecordType object. 
 * For cleanup, if there are any idents left on the identBuffer, they are all
 * discarded. Saves the RecordType object to the Type*& passed in.
 *****************************************************************************/
void createRecordType(Type *&);

/******************************************************************************
 * createSetType(Type*&)
 * Instantiates the passed in Type*& as a SetType using a Range object in the
 * rangeBuffer. Top of rangeBuffer is then popped off.
 *****************************************************************************/
void createSetType(Type *&);
void createStringRange(const char*, const char*);
bool checkTypesEqual(Type*, Type*);
void createTypeSymbol(const char *, Type*);
void createVariableList(Type *&);
void createVariables(Type *&);

/******************************************************************************
 * exitScope()
 * Calls the symbol table's leaveScope() method, which returns the stackframe
 * scope that is being exited. The contents of this scope is then printed out,
 * and the StackFrame object is deleted.
 *****************************************************************************/
void exitScope();

Type* getConstantType(Constant*);
Type* getDivideType(Type *left, Type *right);
Type* getDivModType(Type *left, Type *right);
Type* getMultAddSubType(Type*,Type*);
void getSymbolicType(Type *&, const char*);
std::string getTabs();
Type* processFunctionCall(const char* ident);
void printCaseLabel();
void printErrorLog();
void processProcedureCall(const char* ident);
void pushVarOnStack();
void setCaseType(Type* t);
bool stackHasSymbol(const char *);
void typeCheckCaseLabel();


/******************************************************************************
 * searchStack(const char *ident, T *&castSymbol
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
	std::stringstream ss;
	ss << "***ERROR(line: " << lineNumber << "): Ident \"" << ident << "\" is undefined.";
	addError(ss.str());
        //std::cout << "***ERROR(line: " << lineNumber << ")Ident \"" << ident << "\" is undefined." << std::endl;
    } else {
        castSymbol = dynamic_cast<T*>(symbol); // = foundType;
    }
    return isFound;
}

#endif
