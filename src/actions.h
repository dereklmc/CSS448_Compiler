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

#include <deque>

extern Stack symbolTable;
void checkPointers();

void addIdent(const char *);

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
bool searchStack(const char*, T*&);

void createProgramScope(const char*);

/******************************************************************************
 * createParameter
 * Attempts to create parameters based on a char array representing the type
 * the parameters will be. 
 * It first searches the symbol table by name to see if the type already 
 * exists. If found, one by one, names will be popped off the identBuffer and
 * added to a new parameter object. This object will then get added to the
 * paramBuffer.
 *****************************************************************************/
void createParameter(const char*);

void createProcedure(const char*, Procedure*&);

void createProcedureWithParams(const char*, Procedure*&);

/******************************************************************************
 * createFunction
 * Takes in an array of chars which will represent the name of the new function
 * and a pointer to a reference of a Function object that will be initialized
 * with that name.
 *****************************************************************************/
void createFunction(const char*, Function*&);

/******************************************************************************
 * createFunctionWithParams
 * Like createFunction, this method takes in an array of chars that will 
 * represent the name of the new function and a pointer to a reference of a 
 * Function object that will be initialized with that name. After calling
 * createFunction to initialize its funcPtr, it pops all of the parameters off
 * of the paramBuffer and adds them to the function.
 *****************************************************************************/
void createFunctionWithParams(const char*, Function*&);

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
void createFunctionDecl(const char*, Function*&);

/******************************************************************************
 * createProcedureDecl
 * Takes in a pointer to a Procedure object.
 * It adds the Procedure to the current scope, then creates a new scope. 
 * Lastly, it gets the Procedure object's parameters (if any) and adds them
 * to the new current Procedure scope.
 *****************************************************************************/
void createProcedureDecl(Procedure*);

void createTypeSymbol(const char *, Type*);

void createPointer(Type*&, const char*);

void getSymbolicType(Type *&, const char*);

void createArrayType(ArrayType *&, Type *);

void createSetType(Type *&);

void createStringRange(const char*, const char*);

void createConstRange(ConstValue*, ConstValue*);

void createVariableList(Type *&);

void createVariables(Type *&);

void createRecordType(Type *&);

void createConstValue(ConstValue*&, const char*, ConstValueType);

void createConstant(const char*, ConstValue*&);

void pushVarOnStack();

/******************************************************************************
 * exitScope
 * Calls the symbol table's leaveScope() method, which returns the stackframe
 * scope that is being exited. The contents of this scope is then printed out,
 * and the StackFrame object is deleted.
 *****************************************************************************/
void exitScope();

#endif
