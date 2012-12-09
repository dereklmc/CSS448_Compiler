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
#include <stack>

extern Stack symbolTable;
extern int lineNumber;		// The current line of the program we are parsing

extern std::stack<Symbol*> designators;
extern std::deque<Range*> accessedArrayRanges;

/******************************************************************************
 * addAR(const char* ident, Type*)
 * Takes a Type value that is either an ArrayType or a RecordType and places on
 * to arBuffer.
 *****************************************************************************/
void addAR(const char* ident, Type* type);

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
 * createAR()
 * Creates all the Types that had been fed into the arBuffer.
 *****************************************************************************/
void createAR();

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

/******************************************************************************
 * creatStringRange(const char*, const char*)
 * Creates a new Range object using the two char* passed in. Then pushes this 
 * object onto the rangeBuffer.
 *****************************************************************************/
void createStringRange(const char*, const char*);

/******************************************************************************
 * checkTypesEqual(Type*, Type*)
 * Evaluates the two Type* passed in to test if they are equal or valid to
 * interact with each other.
 * Initially checks to see if either Type* is NULL, in which case, they are
 * not equal. Then checks to see if the righthand side is of a NIL_TYPE, in 
 * which case, the interaction is valid.
 * Lastly checks if the Types are equal using one of the Type's equals() 
 * method.
 * Returns areEqual T/F
 *****************************************************************************/
bool checkTypesEqual(Type*, Type*);

/******************************************************************************
 * createTypeSymbol(const char*, Type*)
 * Check to see if the Type* passed in is not NULL. Then attempts to cast the 
 * type as a SetType. If it is a SetType, no TypeSymbol is created. This would
 * be removed if we were to implement handling of SetTypes. 
 * If the Type* was not a SetType, then a new TypeSymbol will be
 * instantiated with the char* array representing its name, and with the Type*
 * passed in. This TypeSymbol will then be added to the current scope.
 ******************************************************************************/
void createTypeSymbol(const char *, Type*);

/******************************************************************************
 * createVariableList(Type*&)
 * If the Type*& passed in is not NULL, each string in the identBuffer will be
 * popped off and used to create a new Variable object of the type passed in.
 * These Variable objects are then pushed onto the variableBuffer. 
 * Deletes the Type object passed in and nullifies the pointer.
 * Used when creating RecordType objects.
 *****************************************************************************/
void createVariableList(Type *&);

/******************************************************************************
 * createVariables(Type*&)
 * Using the Type*& passed in (as long as it is not NULL), the identBuffer
 * will be emptied, creating new Variables with that name and Type*&, and 
 * attempt to add them to the current scope.
 * Lastly, generates code for each Variable object created.
 * Used in the VariableDecl.
 *****************************************************************************/
void createVariables(Type *&);

/******************************************************************************
 * exitScope()
 * Calls the symbol table's leaveScope() method, which returns the stackframe
 * scope that is being exited. The contents of this scope is then printed out,
 * and the StackFrame object is deleted.
 *****************************************************************************/
void exitScope();

/******************************************************************************
 * exitControlScope()
 * Calls the symbol table's leaveControlScope() method, which returns the stackframe
 * scope that is being exited. The contents of this scope is then printed out,
 * and the StackFrame object is deleted.
 *****************************************************************************/
void exitControlScope();


/******************************************************************************
 * getConstantType(Constant*)
 * Infers the Constant's type based on the enum value stored in the Constant,
 * then returns the type it represents.
 * NOTE: Have not handled Constant's with a Symbol value!
 *****************************************************************************/
Type* getConstantType(Constant*);

/******************************************************************************
 * getDivideType(Type*, Type*)
 * Handles expression comparisons involving the divide operation. It compares
 * the two types to see if they both meet the qualifications to perform the
 * divide operation. 
 * Valid:
 *		- integer (divide) integer = integer
 *		- real (divide) real = integer
 *		- real (divide) integer = integer
 *		- integer (divide) real = integer
 * Invalid:	
 *		- NULLs
 * Returns the type that results from this operation.
 ******************************************************************************/
Type* getDivideType(Type *left, Type *right);

/******************************************************************************
 * getDivModType(Type*, Type*)
 * Handles expression comparisons involving div and mod operations. It compares
 * the two types to see if they both meet the qualifications to perform these
 * operations. 
 * Valid:
 *		- integer (mod/div) integer = integer
 * Invalid:
 *		- real (mod/div) real = illegal
 *		- real (mod/div) integer = illegal
 *		- integer (mod/div) real = illegal
 *		- NULLs
 * Returns the type that results from this operation.
 ******************************************************************************/
Type* getDivModType(Type *left, Type *right);

/******************************************************************************
 * getMultAddSubType(Type*, Type*)
 * Handles expression comparisons involving the multiply, add, and subract 
 * operations. It compares the two types to see if they both meet the 
 * qualifications to perform these operations. 
 * Valid:
 *		- integer (mult/add/sub) integer = integer
 *		- real (mult/add/sub) real = real
 *		- real (mult/add/sub) integer = real
 *		- integer (mult/add/sub) real = real
 * Invalid:	
 *		- NULLs
 * Returns the type that results from this operation.
 ******************************************************************************/
Type* getMultAddSubType(Type*,Type*);

/******************************************************************************
 * getSymbolicType(Type*&, const char*)
 * First searches the stack for a TypeSymbol matching the name passed in. If 
 * that name is found, then a new SymbolicType object is saved to the Type*&
 * that was passed in. If the name was not found, an error is displayed.
 ******************************************************************************/
void getSymbolicType(Type *&, const char*);

/******************************************************************************
 * getTabs()
 * Gets the tabs for the current scope from the symbol table.
 *****************************************************************************/
std::string getTabs();

/******************************************************************************
 * Handles when a function call is encountered in the grammar. First searches
 * if a symbol by ident's name exists in the symbol table. If it does, then it
 * checks to see if it is a function object. If it is a function object, it
 * moves along without printing any errors.
 * It then moves on and grabs the correct parameter types for the function
 * object on the stack and uses compareParamTypes() to evaluate if the 
 * correct parameter types and the ones passed in match.
 * Lastly, returns the correct return type for the function call.
 *****************************************************************************/
Type* processFunctionCall(const char* ident);

/******************************************************************************
 * printCaseLabel()
 * Prints out an individual case label. While the caseLabelBuffer is not empty,
 * it takes a ConstValue* from the buffer and pushes it onto the
 * caseLabelTypeCheckBuffer (used to type check this case label's ConstValues).
 * Depending on the Type of the ConstValue, it will print out a different 
 * sequence for the code generation.
 * If the buffer is not empty by the end of a loop, a comma will be printed
 * out.
 *****************************************************************************/
void printCaseLabel();

/******************************************************************************
 * printErrorLog()
 * Prints all of the strings from the errorLog, one on each line.
 *****************************************************************************/
void printErrorLog();

/******************************************************************************
 * Handles when a procedure call is encountered in the grammar. First searches
 * if a symbol by ident's name exists in the symbol table. If it does, then it
 * checks to see if it is a procedure object. If it is a procedure object, it
 * moves along without printing any errors.
 * It then moves on and grabs the correct parameter types for the procedure
 * object on the stack and uses compareParamTypes() to evaluate if the 
 * correct parameter types and the ones passed in match.
 *****************************************************************************/
void processProcedureCall(const char* ident);

void pushVarOnStack();

/******************************************************************************
 * setCaseType(Type*)
 * Sets the caseType to the passed in type. caseType is used when type 
 * checking the caseLabels.
 *****************************************************************************/
void setCaseType(Type* t);

/******************************************************************************
 * stackHasSymbol(const char*)
 * Takes in a char array representing the name that will be searched for in
 * the Symbol Table. Returns whether or not a symbol by that name was found.
 *****************************************************************************/
bool stackHasSymbol(const char *);

/******************************************************************************
 * typeCheckCaseLabel()
 * Goes through the caseLabelTypeCheckBuffer and evaluates if its type is 
 * valid for that switch case statement. Evaluates if the case label is of 
 * integer, real, boolean, string, char, or symbol and if it equals the type 
 * of the switch case's expression.
 *****************************************************************************/
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

/******************************************************************************
 * dereferenceDesignator(ConstValue* c)
 * Check if the current designator is a pointer and find the symbol it is
 * pointing too, if any. Sets the current designator to that symbol, if found.
 *****************************************************************************/
void dereferenceDesignator();

/******************************************************************************
 * accessField(const char *ident)
 * Check if the current designator is a record and if a given identifier is a
 * field in that record. Sets the current designator to that field, if found.
 *****************************************************************************/
void accessField(const char *ident);

/** TODO */
void accessArray();

/** TODO */
void endProgram(const char *);

/** TODO */
void startBlock();

/** TODO */
void endBlock();

/** TODO */
Type* getRawType(Type *);

#endif
