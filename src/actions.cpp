#include "actions.h"

#include <iostream>
#include <vector>

std::deque<Parameter*> paramBuffer;
std::deque<std::string> identBuffer;
std::deque<PointerType*> ptrBuffer;
std::deque<Range*> rangeBuffer;
std::deque<Variable*> variableBuffer;

std::vector<Type*> parameterTypeCheckBuffer;

std::deque<std::string> errorLog;
Type* caseType;
std::deque<ConstValue*> caseLabelBuffer;
std::deque<ConstValue*> caseLabelTypeCheckBuffer;

Stack symbolTable;

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
Type* processFunctionCall(const char* ident) {
	std::stringstream ss;
	Symbol *fSymbol = NULL;
	bool exists = searchStack(ident, fSymbol);
	Function *functionClass;
	if (exists) {
		//Attempt to cast as function
		functionClass = dynamic_cast<Function*>(fSymbol);
		if (functionClass == NULL) {
			ss << "***ERROR(" << lineNumber << "): Symbol " 
				<< ident << " is not a function definition";
			addError(ss.str());
		}
	}
	else {
		/* TODO - record error */
		ss << "***ERROR(" << lineNumber << "): Symbol " << ident 
			<< " has not been declared";
		addError(ss.str());
	}

	// Check the parameters to see if their types match
	if (exists) {
		std::vector<Parameter*> params = functionClass->getParameters();
			
		std::vector<Type*> typeVector;
		int temp = params.size();
		for (int i = 0; i < temp; i++) {
			typeVector.push_back(params[i]->type);
		}
		compareParamTypes(typeVector);
		return functionClass->getReturnType();
 	}
	else
		while(!parameterTypeCheckBuffer.empty())
			parameterTypeCheckBuffer.pop_back();
		return NULL;
	
}

/******************************************************************************
 * Handles when a procedure call is encountered in the grammar. First searches
 * if a symbol by ident's name exists in the symbol table. If it does, then it
 * checks to see if it is a procedure object. If it is a procedure object, it
 * moves along without printing any errors.
 * It then moves on and grabs the correct parameter types for the procedure
 * object on the stack and uses compareParamTypes() to evaluate if the 
 * correct parameter types and the ones passed in match.
 *****************************************************************************/
void processProcedureCall(const char* ident) {
	std::stringstream ss;
	Symbol *fSymbol = NULL;
	bool exists = searchStack(ident, fSymbol);
	Procedure *procClass;
	if (exists) {
		//Attempt to cast as procedure
		procClass = dynamic_cast<Procedure*>(fSymbol);
		if (procClass == NULL) {
			ss << "***ERROR(" << lineNumber << "): Symbol " 
				<< ident << " is not a procedure definition";
			addError(ss.str());
		}
	}
	else {
		/* TODO - record error */
		ss << "***ERROR(" << lineNumber << "): Symbol " << ident 
			<< " has not been declared";
		addError(ss.str());
	}

	// Check the parameters to see if their types match
	if (exists) {
		std::vector<Parameter*> params = procClass->getParameters();
			
		std::vector<Type*> typeVector;
		int temp = params.size();
		for (int i = 0; i < temp; i++) {
			typeVector.push_back(params[i]->type);
		}

		compareParamTypes(typeVector);
	}
	else {
		while (!parameterTypeCheckBuffer.empty())
			parameterTypeCheckBuffer.pop_back();
	}
}

void addParameterType(Type* t)
{
	parameterTypeCheckBuffer.push_back(t);
}

void checkConditionalExpressionType(Type* t) {
	if (!BOOLEAN_TYPE->equals(t)) {
	    std::stringstream ss;
	    ss << "***ERROR(line: " << lineNumber << "): Expression is not conditional";
	    addError(ss.str());
	    //std::cout << "ERROR: Expression is not conditional" << std::endl;
	}
}

void setCaseType(Type* t)
{
	caseType = t;
}

void addCaseLabel(ConstValue* c)
{
	caseLabelBuffer.push_back(c);
}

std::string getTabs()
{
	return symbolTable.getCurrentTabs();
}

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
void printCaseLabel()
{
	std::cout << getTabs() << "case ";
	std::deque<ConstValue*> tempQueue;
	while (!caseLabelBuffer.empty()) {
        ConstValue* val = caseLabelBuffer.front();
		caseLabelTypeCheckBuffer.push_back(val);	// Push this value on a separate buffer for
													// type checking later
		int type = val->getType();
		if (type == STRING)
			std::cout << "\"" << val->generateCode() << "\"";
		else if (type == CHAR)
			std::cout << "\'" << val->generateCode() << "\'";
		else
			std::cout << val->generateCode();
        caseLabelBuffer.pop_front();
		if (!caseLabelBuffer.empty())
			std::cout << ", ";
    }

	std::cout << ": " << std::endl << getTabs() << "{" << std::endl;
}

/******************************************************************************
 * typeCheckCaseLabel()
 * Goes through the caseLabelTypeCheckBuffer and evaluates if its type is 
 * valid for that switch case statement. Evaluates if the case label is of 
 * integer, real, boolean, string, char, or symbol and if it equals the type 
 * of the switch case's expression.
 *****************************************************************************/
void typeCheckCaseLabel()
{
	std::stringstream ss;
	while (!caseLabelTypeCheckBuffer.empty()) {
			ConstValue* val = caseLabelTypeCheckBuffer.front();
			int type = val->getType();
			// If the case label's type is an int, compare int type to the type
			// of the expression in switch(expression)
			if ((type == INTEGER) && (!checkTypesEqual(INTEGER_TYPE, caseType))){
				// TODO record error
				ss << "***ERROR(line: " << lineNumber << "): Case label \"" 
					<< val->generateCode() << "\"(int) does not match case expression type"; 
				errorLog.push_back(ss.str());
			}
			// If case label's type is a real
			else if ((type == REAL) && (!checkTypesEqual(REAL_TYPE, caseType))){
				// TODO record error
				ss << "***ERROR(line: " << lineNumber << "): Case label \"" 
					<< val->generateCode() << "\"(real) does not match case expression type"; 

				errorLog.push_back(ss.str());
			}
			// Boolean
			else if ((type == BOOLEAN) && (!checkTypesEqual(BOOLEAN_TYPE, caseType))){
				// TODO record error
				ss << std::cout << "***ERROR(line: " << lineNumber 
					<< "): Case label \"" << val->generateCode() <<
					"\"(bool) does not match case expression type"; 
				errorLog.push_back(ss.str());
			}
			// String
			else if ((type == STRING) && (!checkTypesEqual(STRING_TYPE, caseType))){
				// TODO record error
				ss << "***ERROR(line: " << lineNumber << "): Case label \"" 
					<< val->generateCode() << "\"(string) does not match case expression type"; 
				errorLog.push_back(ss.str());
			}
			// Char
			else if ((type == CHAR) && (!checkTypesEqual(CHAR_TYPE, caseType))){
				// TODO record error
				ss << "***ERROR(line: " << lineNumber << "): Case label \"" 
					<< val->generateCode() << "\"(char) does not match case expression type";
				errorLog.push_back(ss.str());
			}
			else if (type == SYMBOL) {
				std::string symbolNameStr = val->generateCode();
				const char* sName = symbolNameStr.c_str();
				Variable* v = NULL;
				if (searchStack(sName, v)) {
					if (!checkTypesEqual(v->type, caseType)) {
						ss << "***ERROR(line: " << lineNumber << "): Case label \"" 
							<< val->generateCode() 
							<< "\"(Symbol)'s type does not match case expression type";
						errorLog.push_back(ss.str()); 				
					}
				}
				else
					{}
					// symbol does not exist
			}
			caseLabelTypeCheckBuffer.pop_front();
			ss.clear();
	}
}

/******************************************************************************
 * addIdent
 * Takes a char* array, which is then converted to a string before being 
 * added to the identBuffer for later use.
 ******************************************************************************/
void addIdent(const char *ident)
{
    identBuffer.push_back(std::string(ident));
}

/******************************************************************************
 * stackHasSymbol(const char*)
 * Takes in a char array representing the name that will be searched for in
 * the Symbol Table. Returns whether or not a symbol by that name was found.
 *****************************************************************************/
bool stackHasSymbol(const char *ident)
{
    Symbol *symbol = NULL;
    std::string identStr(ident);
    return symbolTable.searchStack(identStr, symbol);
}

/******************************************************************************
 * createProgramScope
 * This method is called whenever the grammar encounters a new program 
 * declaration. Converts the program name from a char* array to a string, 
 * discards any program parameters (at this point in time) and creates a new
 * scope with the program's name.
 *****************************************************************************/
void createProgramScope(const char *ident) {
    std::string scopeName(ident);
	// Discard program parameters
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
    std::stringstream ss;
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
		// undefined, empty ident buffer
		while (!identBuffer.empty()) {
			identBuffer.pop_front();
		}
    }
}

/******************************************************************************
 * createProcedure(const char*, Procedure*&)
 * Instantiates the Procedure pointer passed in with the char* array passed in.
 * If there were any parameters placed in the paramBuffer, they are now 
 * popped off the buffer and added to the Procedure object.
 *****************************************************************************/
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
    while (!paramBuffer.empty()) {
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
void createFunctionDecl(const char* ident, Function* funcPtr)
{
    /* Check if return type is valid */
    TypeSymbol *symbolType = NULL;
    bool found = searchStack<TypeSymbol>(ident, symbolType);
    // TODO print error message if bad type was found.
    funcPtr->setType(new SymbolicType(symbolType));
    
    createProcedureDecl(funcPtr);
    
    std::cout << getTabs() << symbolType->generateCode() << " _returnValue;" << std::endl;
}

/******************************************************************************
 * createProcedureDecl(Procedure*)
 * Takes in a pointer to a Procedure object.
 * It adds the Procedure to the current scope, then creates a new scope, gets 
 * the Procedure object's parameters (if any), adds them
 * to the new current Procedure scope, and generates the code for a procedure
 * declaration.
 *****************************************************************************/
void createProcedureDecl(Procedure* proc)
{
    /* Put procedure in parent scope */
    symbolTable.current->addSymbol(proc);
    /* Enter Procedure Scope */
    std::cout << getTabs() << "class " << proc->name << " {" << std::endl;
    std::cout << getTabs() << "public:" << std::endl ;
    symbolTable.createScope(proc->name);

    /* Put procedure params on symbol stack. */
    std::vector<Parameter*> toPutOnStack = proc->getParameters();
    for (int i = 0; i < toPutOnStack.size(); i++) {
        Symbol *paramVarSymbol = toPutOnStack[i]->getVariable();
        symbolTable.current->addSymbol(paramVarSymbol);
        
        std::cout << getTabs() << paramVarSymbol->generateCode() << ";" << std::endl;
    }
    
    std::cout << getTabs() << proc->name << "(";
    
    if (toPutOnStack.size() > 0) {
        std::cout << toPutOnStack[0]->generateCode() << std::flush;
    }
    for (int i = 1; i < toPutOnStack.size(); i++) {
        Parameter *param = toPutOnStack[i];
        std::cout << ", " << toPutOnStack[i]->generateCode() << std::flush;
    }
    
    std::cout << ") {" << std::endl;
    
    for (int i = 0; i < toPutOnStack.size(); i++) {
        Parameter *param = toPutOnStack[i];
        std::cout << getTabs() << "\tthis->" << param->name << " = " 
			<< param->name << ";" << std::endl;
    }
    
    std::cout << getTabs() << "}" << std::endl;
}

void createLoopCaseScope(const char *ident)
{
	symbolTable.createScope(std::string(ident));
}

/******************************************************************************
 * createTypeSymbol(const char*, Type*)
 * Check to see if the Type* passed in is not NULL. Then attempts to cast the 
 * type as a SetType. If it is a SetType, no TypeSymbol is created. This would
 * be removed if we were to implement handling of SetTypes. 
 * If the Type* was not a SetType, then a new TypeSymbol will be
 * instantiated with the char* array representing its name, and with the Type*
 * passed in. This TypeSymbol will then be added to the current scope.
 ******************************************************************************/
void createTypeSymbol(const char *ident, Type *type)
{
    if (type != NULL) {
	SetType* sType = dynamic_cast<SetType*>(type);
	if (sType == NULL) {
        	std::string name(ident);
        	TypeSymbol *symbol = new TypeSymbol(name, type);
        	symbolTable.current->addSymbol(symbol);
        	std::cout << getTabs() << symbol->generateTypeDeclCode() + ";\n";
	}
	else {
		std::cout << getTabs() << "/* This is where set type " << ident << " would have been defined */";
	}
    }
}

/******************************************************************************
 * createPointer(Type*&, const char*)
 * A Symbol object is instantiated with the char* array for its name. A new
 * PointerType object is then instantiated using this Symbol object and
 * pushed onto the ptrBuffer. This pointer is also saved to the Type*& that
 * was passed in.
 *****************************************************************************/
void createPointer(PointerType*& createdType, const char *ident)
{
    // Find symbol for ident
    Symbol *s = new Symbol(ident);
    
    createdType = new PointerType(s);
    ptrBuffer.push_back(createdType);
}

/******************************************************************************
 * checkPointers()
 * While the ptrBuffer is not empty, takes the pointer from the front and 
 * searches the symbol table for the pointee's name. Whether or not the pointee
 * is found in the symbol table, the pointer will be popped off the ptrBuffer.
 *****************************************************************************/
void checkPointers()
{
    while(!ptrBuffer.empty())
    {
        PointerType* ptr = ptrBuffer.front();
        Symbol* foundSymbol = NULL;
        bool found = symbolTable.searchStack(ptr->getPointee()->name, foundSymbol);
        if(found)
        {
            ptr->setPointee(foundSymbol);
        } else {
            std::stringstream ss;
            ss << "***ERROR(" << lineNumber << "): \"" << ptr->getPointee()->name << "\" is undefined!";
            addError(ss.str());
            std::cout << "Error: Pointee does not exist." << std::endl;
        }
     
        ptrBuffer.pop_front();
    }
}

/******************************************************************************
 * getSymbolicType(Type*&, const char*)
 * First searches the stack for a TypeSymbol matching the name passed in. If 
 * that name is found, then a new SymbolicType object is saved to the Type*&
 * that was passed in. If the name was not found, an error is displayed.
 ******************************************************************************/
void getSymbolicType(Type *&type, const char *name)
{
    TypeSymbol *typeSymbol = NULL;
    bool isFound = searchStack<TypeSymbol>(name, typeSymbol);
    if (isFound && typeSymbol != NULL) {
        type = new SymbolicType(typeSymbol);
    } else {
        type = NULL;
		std::stringstream ss;
		ss << "***ERROR(line: " << lineNumber << "): Type \"" << name << "\" is undefined.";
		errorLog.push_back(ss.str());
    }
}

/******************************************************************************
 * createArrayType(ArrayType *&, Type*)
 * Instantiates the ArrayType object passed in with the Type* also passed in.
 * While there are still Range objects in the rangeBuffer, they will be popped
 * off and added to the ArrayType object.
 *****************************************************************************/
void createArrayType(ArrayType *&arrayType, Type *contentsType)
{
    arrayType = new ArrayType(contentsType);
    /* Add ranges */
    while (!rangeBuffer.empty()) {
        arrayType->addRange(rangeBuffer.front());
        rangeBuffer.pop_front();
    }
}

/******************************************************************************
 * createSetType(Type*&)
 * Instantiates the passed in Type*& as a SetType using a Range object in the
 * rangeBuffer. Top of rangeBuffer is then popped off.
 *****************************************************************************/
void createSetType(Type *&createdType)
{
    createdType = new SetType(rangeBuffer.front());
    rangeBuffer.pop_front();
}

/******************************************************************************
 * creatStringRange(const char*, const char*)
 * Creates a new Range object using the two char* passed in. Then pushes this 
 * object onto the rangeBuffer.
 *****************************************************************************/
void createStringRange(const char* start, const char* stop) {
    Range *range = new CharRange(start[0], stop[0]);
    rangeBuffer.push_back(range);
}

/******************************************************************************
 * createConstRange(ConstValue*, ConstValue*)
 * Creates a new Range object using the ConstValue*s passed in. Then pushes 
 * this Range object onto the rangeBuffer.
 *****************************************************************************/
void createConstRange(ConstValue *start, ConstValue *stop) {
    Range *range = new ConstRange(start, stop);
    rangeBuffer.push_back(range);
}

/******************************************************************************
 * createVariableList(Type*&)
 * If the Type*& passed in is not NULL, each string in the identBuffer will be
 * popped off and used to create a new Variable object of the type passed in.
 * These Variable objects are then pushed onto the variableBuffer. 
 * Deletes the Type object passed in and nullifies the pointer.
 * Used when creating RecordType objects.
 *****************************************************************************/
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

/******************************************************************************
 * createVariables(Type*&)
 * Using the Type*& passed in (as long as it is not NULL), the identBuffer
 * will be emptied, creating new Variables with that name and Type*&, and 
 * attempt to add them to the current scope.
 * Lastly, generates code for each Variable object created.
 * Used in the VariableDecl.
 *****************************************************************************/
void createVariables(Type *&type) {
    if (type != NULL) {
        while (!identBuffer.empty()) {
            std::string ident = identBuffer.front();
            identBuffer.pop_front();
            
            Variable* var = new Variable(ident,type->clone());
            symbolTable.current->addSymbol(var);
            std::cout << getTabs() << var->generateCode() << ";"<<std::endl;
        }
        delete type;
        type = NULL;
    }
}

void createRecordType(Type *&createdType) {
    std::stringstream ss;

    RecordType *record = new RecordType(symbolTable.currentScope);
    while (!variableBuffer.empty()) {
        Variable *var = variableBuffer.front();
        if (!record->addField(var)) {
	    	ss << "***ERROR(line: " << lineNumber << "): " << var->name 
				<< "already exists in record";
	    	addError(ss.str());
	    	variableBuffer.pop_front();

            delete var;
            var = NULL;
        }
	else 
            variableBuffer.pop_front();
    }
    // in case there are stranglers
    while (!identBuffer.empty())
		identBuffer.pop_front();
    createdType = record;
}

/******************************************************************************
 * createConstValue(ConstValue*&, const char*, ConstValueType)
 * Instantiates the ConstValue object passed in with the const char* array
 * and ConstValueType also passed in.
 ******************************************************************************/
void createConstValue(ConstValue *&constValue, const char *value, ConstValueType type) {
    std::string strValue(value);
    constValue = new ConstValue(strValue, type);
}

/******************************************************************************
 * createConstant(const char*, ConstValue*&)
 * Creates a new Constant object using the char* array and ConstValue passed
 * in. Then addes this Constant object to the current scope and generates code
 * for it.
 *****************************************************************************/
void createConstant(const char *ident, ConstValue *&value) {
    Constant *symbol = new Constant(std::string(ident), value);
    symbolTable.current->addSymbol(symbol);
    std::cout << getTabs() << symbol->generateCode() << std::endl;
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
    //std::cout << *scope;
    /* Mem management */
    delete scope;
    scope = NULL;
    std::cout << getTabs() << "}";
}

/******************************************************************************
 * getConstantType(Constant*)
 * Infers the Constant's type based on the enum value stored in the Constant,
 * then returns the type it represents.
 * NOTE: Have not handled Constant's with a Symbol value!
 *****************************************************************************/
Type* getConstantType(Constant* c) {
	int constType = c->getEnumType();
	if (constType == INTEGER) {
		return INTEGER_TYPE;
	}
	else if (constType == REAL) {
		return REAL_TYPE;
	}
	else if (constType == BOOLEAN) {
		return BOOLEAN_TYPE;
	}
	else if (constType == STRING) {
		return STRING_TYPE;
	}
	else if (constType == CHAR) {
		return CHAR_TYPE;
	}
	else if (constType == NIL) {
		return NIL_TYPE;	
	}
	//TODO handle the symbol 
}

// Checks to see if the ident passed in matches the current scope's name
// If so, it means that this ident is going to be a return statement
// Returns the correct return Type if true, NULL if not
Type* checkForReturnValue(const char* c) {

	Type* retVal = NULL;
	std::string currentName = symbolTable.current->name; //Get current scope's name
	std::string otherName = (std::string)(c);
	if (currentName.compare(otherName) == 0) {	//if the name matches
		// Find the function's object in the frame before it
		Function* funcSym = NULL;
		bool exists = searchStack(currentName.c_str(), funcSym);
		// Verify that the symbol found was a function object
		if (funcSym != NULL) {
			retVal = funcSym->getReturnType(); //get the correct return type for this	
		}
	}
	return retVal;
}

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
bool checkTypesEqual(Type *a, Type *b)
{
	bool areEqual = false;
	std::stringstream ss;
	if (a == NULL) {
		ss << "***ERROR(line: " << lineNumber << "): Lefthand side Type in type checks is null";
		if (b == NULL)
			ss << ". Righthand side Type in type checks also is null";
		addError(ss.str());
	}
	else if (b == NULL) {
		ss << "***ERROR(line: " << lineNumber << "): Righthand side Type in type checks is null";
		addError(ss.str());
	}
	else if (b->equals(NIL_TYPE)) {
		areEqual = true;
	}
	else if (a->equals(b)) {
		areEqual = true;
	}

	return areEqual;
}

void  compareParamTypes(std::vector<Type*> a)
{
	std::stringstream ss;
	
	int alength = a.size();
	int blength = parameterTypeCheckBuffer.size();
	if (alength != blength) {
		//Already know that param sets not equal
		ss << "***ERROR(line: " << lineNumber << "): Parameter sets are not equal";
		addError(ss.str());
	}
	else if ((alength == 0) && (blength == 0)) {
		// Parameter sets are equal
	}
	else {
		//Compare each of the Types in both vectors
		int currentIndex = 0;
		while ((currentIndex < alength) && (currentIndex < blength)) {
			//checkTypesEqual(a[currentIndex]->type, b[currentIndex]->type);
			//NOTE Should we take error message out of checkTypesEqual and make it return a bool?
			if (!checkTypesEqual(a[currentIndex], parameterTypeCheckBuffer[currentIndex])) {
				ss << "***ERROR(line: " << lineNumber << "): Parameter set types are not equal";
				addError(ss.str());
				break;
			}
			currentIndex++;
		}
		if ((currentIndex < alength) || (currentIndex < blength)) {
			ss <<  "***ERROR(line: " << lineNumber << "): Parameter sets are not equal";
			addError(ss.str());
		}
	}

	parameterTypeCheckBuffer.clear(); // Clear the buffer for the next comparison
}

void addError(std::string s)
{
	errorLog.push_back(s);
}

void printErrorLog()
{
    while (!errorLog.empty()) {
		std::cout << errorLog.front() << std::endl;
		errorLog.pop_front();
    }
}

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
Type* getDivModType(Type *left, Type *right)
{
	std::stringstream ss;
	if (left == NULL || right == NULL) {
		// TODO: log error
		ss << "***ERROR(line: " << lineNumber << "): left or righthand Type in div/mod is NULL";
		addError(ss.str());
		//std::cout << "***ERROR(line: " << lineNumber << "): left or righthand Type in div/mod is NULL" << std::endl;
		return NULL;
	}
	
	bool leftIsInteger = INTEGER_TYPE->equals(left);
	bool rightIsInteger = INTEGER_TYPE->equals(right);
	
	bool leftIsReal = REAL_TYPE->equals(left);
	bool rightIsReal = REAL_TYPE->equals(right);
	
	if (!leftIsInteger && !leftIsReal) {
		// TODO log error
		ss << "***ERROR(line: " << lineNumber << "): wrong left hand arg type to \"*\"";
		addError(ss.str());
		//std::cout << "***ERROR(line: " << lineNumber << "): wrong left hand arg type to \"*\"" << std::endl;
		return NULL;
	}

	if (!rightIsInteger && !rightIsReal) {
		// TODO log error
		ss << "***ERROR(line: " << lineNumber << "): wrong right hand arg type to \"mod\"";
		addError(ss.str());
		//std::cout << "***ERROR(line: " << lineNumber << "): wrong right hand arg type to \"mod\"" << std::endl;
		return NULL;
	}
	
	// No reals allowed for mod and div
	if (leftIsReal || rightIsReal)
	{
		// TODO log error
		ss << "***ERROR(line: " << lineNumber << "): either right or left hand args for \"div\" are reals";
		addError(ss.str());
		//std::cout << "***ERROR(line: " << lineNumber << "): either right or left hand args for \"div\" are reals" << std::endl;
		return NULL;
	}

	return INTEGER_TYPE;
}

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
Type* getDivideType(Type *left, Type *right)
{
	std::stringstream ss;
	if (left == NULL || right == NULL) {
		ss << "***ERROR(line: " << lineNumber << "): left or righthand Type of divide is NULL";
		addError(ss.str());
		//std::cout << "***ERROR(line: " << lineNumber << "): left or righthand Type of divide is NULL" << std::endl;
		return NULL;
	}
	bool leftIsInteger = INTEGER_TYPE->equals(left);
	bool rightIsInteger = INTEGER_TYPE->equals(right);
	bool leftIsReal = REAL_TYPE->equals(left);
	bool rightIsReal = REAL_TYPE->equals(right);

	if (!leftIsInteger && !leftIsReal) {
		ss << "***ERROR(line: " << lineNumber << "): wrong left hand arg type to \"/\"";
		addError(ss.str());
		//std::cout << "***ERROR(line: " << lineNumber << "): wrong left hand arg type to \"/\"" << std::endl;
		return NULL;
	}

	if (!rightIsInteger && !rightIsReal) {
		ss << "***ERROR(line: " << lineNumber << "): wrong right hand arg type to \"/\"";
		addError(ss.str());
		//std::cout << "***ERROR(line: " << lineNumber << "): wrong right hand arg type to \"/\"" << std::endl;
		return NULL;
	}
	
	// As long as the right and left terms are an int or a real, then it will
	// always return a real
	return REAL_TYPE;
}

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
Type* getMultAddSubType(Type *left, Type *right)
{
	std::stringstream ss;
	if (left == NULL || right == NULL) {
		ss << "***ERROR(line: " << lineNumber 
			<< "): left or righthand Type of multiplication/addition/subraction is NULL";
		addError(ss.str());
		//std::cout << "***ERROR(line: " << lineNumber << "): left or righthand Type of multiplication/addition/subraction is NULL" << std::endl;
		return NULL;
	}
	
	bool leftIsInteger = INTEGER_TYPE->equals(left);
	bool rightIsInteger = INTEGER_TYPE->equals(right);
	
	bool leftIsReal = REAL_TYPE->equals(left);
	bool rightIsReal = REAL_TYPE->equals(right);
	
	if (!leftIsInteger && !leftIsReal) {
		ss << "***ERROR(line: " << lineNumber << "): wrong left hand arg type to \"*/+/-\"";
		addError(ss.str());
		//std::cout << "***ERROR(line: " << lineNumber << "): wrong left hand arg type to \"*/+/-\"" << std::endl;
		return NULL;
	}

	if (!rightIsInteger && !rightIsReal) {
		ss << "***ERROR(line: " << lineNumber << "): wrong right hand arg type to \"*/+/-\"";
		addError(ss.str());
		//std::cout << "***ERROR(line: " << lineNumber << "): wrong right hand arg type to \"*/+/-\"" << std::endl;
		return NULL;
	}

	if (leftIsInteger && rightIsInteger) {
		return INTEGER_TYPE;
	}

	if (leftIsReal || rightIsReal) {
		return REAL_TYPE;
	}

	// SHOULD NOT REACH THIS POINT !!!
	ss << "***ERROR(line: " << lineNumber << "): fatal";
	addError(ss.str());
	//std::cout << "***ERROR(line: " << lineNumber << "): fatal" << std::endl;
	return NULL;
}


