#include "actions.h"

#include <iostream>
#include <vector>
#include <stack>

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
// Queue for ranges and records. Hence "ar"
std::deque<TypeSymbol*> arBuffer;
std::deque<TypeSymbol*> ptrGenBuffer;

Stack symbolTable;

std::stack<Symbol*> designators;
std::deque<Range*> accessedArrayRanges;


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
Type* processFunctionCall(Function *func) {
    if (func != NULL) {
        // Check the parameters to see if their types match
        std::vector<Parameter*> params = func->getParameters();
        
        std::vector<Type*> typeVector;
        int temp = params.size();
        for (int i = 0; i < temp; i++) {
            typeVector.push_back(params[i]->type);
        }
        compareParamTypes(typeVector);
        
        std::cout << ").call()._returnValue;" << std::endl;
        
        return func->getReturnType();
    }
}

/*****************************************************************************
 * addCaseLabel(ConstValue*)
 * Adds the given ConstValue to a buffer, to be handled later.
 ****************************************************************************/
void addCaseLabel(ConstValue* c)
{
    caseLabelBuffer.push_back(c);
}

/*****************************************************************************
 * createAR()
 * Outputs code of all the Arrays and Records placed into the arBuffer, 
 * clearing the buffer at the same time.
 ****************************************************************************/
void createAR()
{
    while(!arBuffer.empty())
    {
        stringstream ss;
        TypeSymbol* symbol = arBuffer.front();
        std::cout << getTabs() << symbol->generateTypeDeclCode() + ";\n";
        arBuffer.pop_front();
    }
}

/******************************************************************************
 * processProcedureCall(const char*)
 * Handles when a procedure call is encountered in the grammar. First searches
 * if a symbol by ident's name exists in the symbol table. If it does, then it
 * checks to see if it is a procedure object. If it is a procedure object, it
 * moves along without printing any errors.
 * It then moves on and grabs the correct parameter types for the procedure
 * object on the stack and uses compareParamTypes() to evaluate if the 
 * correct parameter types and the ones passed in match.
 *****************************************************************************/
void processProcedureCall(Procedure *procClass) {
    // Check the parameters to see if their types match
    std::vector<Parameter*> params = procClass->getParameters();
    std::vector<Type*> typeVector;
    int temp = params.size();
    for (int i = 0; i < temp; i++) {
        typeVector.push_back(params[i]->type);
    }
    compareParamTypes(typeVector);
    
    std::cout << ").call();" << std::endl;
}

/*****************************************************************************
 * addParameterType(Type*)
 * Adds the given Type to a buffer, to be handled later.
 ****************************************************************************/
void addParameterType(Type* t)
{
    parameterTypeCheckBuffer.push_back(t);
}

/*****************************************************************************
 * addPointers(const char*, Type*)
 * Generates a TypeSymbol to a buffer, to be handled later.
 ****************************************************************************/
void addPointers(const char* ident, Type* type)
{
    //add to symbolTable
    std::string name(ident);
    TypeSymbol *symbol = new TypeSymbol(name, type);
    symbolTable.current->addSymbol(symbol);
    ptrGenBuffer.push_back(symbol);

}

/*****************************************************************************
 * addAR(const char*, Type*)
 * Generates a TypeSymbol to a buffer, to be handled later, and adds it to the
 * symbol table.
 ****************************************************************************/
void addAR(const char* ident, Type* type)
{
    if (type != NULL) { 
        type->typeDefed = true;
        SetType* sType = dynamic_cast<SetType*>(type);
        if (sType == NULL) { 
            std::string name(ident);
            TypeSymbol *symbol = new TypeSymbol(name, type);
            symbolTable.current->addSymbol(symbol); 
            arBuffer.push_back(symbol);
            
            if (dynamic_cast<RecordType*>(type) != NULL) {
                std::cout << getTabs() << "struct " << ident << ";" << std::endl;
            }
        }
        else {
            std::cout << getTabs() << "/* This is where set type " << ident << " would have been defined */" << std::endl;
        }
    }
}

/*****************************************************************************
 * checkConditionalExpressionType(Type*)
 * Checks if the expression is conditional or not.
 ****************************************************************************/
void checkConditionalExpressionType(Type* t) {
	if (!BOOLEAN_TYPE->equals(t)) {
	    std::stringstream ss;
	    ss << "***ERROR(line: " << lineNumber << "): Expression is not conditional";
	    addError(ss.str());	
    }
}

/*****************************************************************************
 * setCaseType(Type*)
 * Sets caseType to the given Type.
 ****************************************************************************/
void setCaseType(Type* t)
{
	caseType = t;
}

/*****************************************************************************
 * getTabs()
 * Asks the symbol table for the proper tab output. Should be used before any
 * output for any line.
 ****************************************************************************/
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
				ss << "***ERROR(line: " << lineNumber << "): Case label \"" 
					<< val->generateCode() << "\"(int) does not match case expression type"; 
				errorLog.push_back(ss.str());
			}
			// If case label's type is a real
			else if ((type == REAL) && (!checkTypesEqual(REAL_TYPE, caseType))){
				ss << "***ERROR(line: " << lineNumber << "): Case label \"" 
					<< val->generateCode() << "\"(real) does not match case expression type"; 

				errorLog.push_back(ss.str());
			}
			// Boolean
			else if ((type == BOOLEAN) && (!checkTypesEqual(BOOLEAN_TYPE, caseType))){
				ss << std::cout << "***ERROR(line: " << lineNumber 
					<< "): Case label \"" << val->generateCode() <<
					"\"(bool) does not match case expression type"; 
				errorLog.push_back(ss.str());
			}
			// String
			else if ((type == STRING) && (!checkTypesEqual(STRING_TYPE, caseType))){
				ss << "***ERROR(line: " << lineNumber << "): Case label \"" 
					<< val->generateCode() << "\"(string) does not match case expression type"; 
				errorLog.push_back(ss.str());
			}
			// Char
			else if ((type == CHAR) && (!checkTypesEqual(CHAR_TYPE, caseType))){
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
void createProgramScope(const char *programName) {
    std::string scopeName(programName);
	// Discard program parameters
    while (!identBuffer.empty()) {
		identBuffer.pop_front();
    }	
    symbolTable.createScope(scopeName);
    
    // Generate code.
    std::cout << "#include <iostream>" << std::endl;
    std::cout << "using namespace std;" << std::endl;
    std::cout << "class " << programName << " {" << std::endl;
    std::cout << "public:" << std::endl;
}

/******************************************************************************
 * endProgram(const char*)
 * Special method only used at the end of a program. Generates the main method
 * nessesary to run, and prints out an error log with all errors encountered.
 *****************************************************************************/
void endProgram(const char *programName)
{
    std::cout << ";" << std::endl << std::endl;
    std::cout << "int main() {" << std::endl;
    std::cout << "\t" << programName << " p;" << std::endl;
    std::cout << "\tp.call();" << std::endl;
    std::cout << "\treturn 0;" << std::endl;
    std::cout << "}" << std::endl;
    
    printErrorLog();
}

/******************************************************************************
 * createParameter(const char*)
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
    std::vector<Parameter*> procParams = proc->getParameters();
    for (int i = 0; i < procParams.size(); i++) {
        Parameter *param = procParams[i];
        symbolTable.current->addSymbol(new Parameter(*param));
        std::cout << getTabs() << param->generateDeclaration() << ";" << std::endl;
    }
    std::cout << getTabs() << symbolTable.current->previous->name << " *parent;" << std::endl;
    
    std::cout << getTabs() << proc->name << "(";
    std::cout << symbolTable.current->previous->name << " *parent";
    
    for (int i = 0; i < procParams.size(); i++) {
        std::cout << ", " << procParams[i]->generateFunctorParam() << std::flush;
    }
    
    std::cout << ") {" << std::endl;
    
    for (int i = 0; i < procParams.size(); i++) {
        std::cout << getTabs() << "\tthis->" << procParams[i]->generateInit() << ";" << std::endl;
    }
    
    std::cout << getTabs() << "\tthis->parent = parent;" << std::endl;
    std::cout << getTabs() << "}" << std::endl;
}

void createLoopCaseScope(const char *ident)
{
	symbolTable.createControlScope(std::string(ident));
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
        type->typeDefed = true;
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
 * generatePointers()
 * Pops pointers off ptrGenBuffer, and outputs their code.
 *****************************************************************************/
void generatePointers()
{
    while(!ptrGenBuffer.empty())
    {
        stringstream ss;
        TypeSymbol* symbol = ptrGenBuffer.front();

        std::cout << getTabs() << symbol->generateTypeDeclCode() + ";\n";
        ptrGenBuffer.pop_front();
    }

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
		ss << "***ERROR(line: " << lineNumber << "): Type \"" << name << 
		    "\" is undefined.";
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
            if(!symbolTable.current->addSymbol(var))
            {
                std::stringstream ss;
                ss << "***ERROR(line: " << lineNumber << "): " << var->name 
				<< " already exists in record";
	    	    addError(ss.str());
            }
            std::cout << getTabs() << var->generateCode() << ";"<<std::endl;
        }
        delete type;
        type = NULL;
    }
}

/******************************************************************************
 * createRecordType(Type*&)
 * Generates a RecordType and inserts all the variables in variableBuffer into
 * it. Returns that record as the createdType pointer.
 *****************************************************************************/
void createRecordType(Type *&createdType) {
    std::stringstream ss;

    RecordType *record = new RecordType(symbolTable.currentScope);
    while (!variableBuffer.empty()) {
        Variable *var = variableBuffer.front();
        if (!record->addField(var)) {
	    	ss << "***ERROR(line: " << lineNumber << "): " << var->name 
				<< " already exists in record";
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
    /* Mem management */
    delete scope;
    scope = NULL;
    std::cout << getTabs() << "}";
}

/******************************************************************************
 * exitControlScope
 * Calls the symbol table's leaveControlScope() method, which returns the stackframe
 * scope that is being exited. The contents of this scope is then printed out,
 * and the StackFrame object is deleted.
 *****************************************************************************/
void exitControlScope()
{
    /* Exit Function scope */
    StackFrame *scope = symbolTable.leaveControlScope();
    /* Print exited scope. */
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
}

/******************************************************************************
 * checkForReturnValue(const char*)
 * Checks to see if the ident passed in matches the current scope's name
 * If so, it means that this ident is going to be a return statement
 * Returns the correct return Type if true, NULL if not
 *****************************************************************************/
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
		std::cout << "b equals nil";
		areEqual = true;
	}
	else if (a->equals(NIL_TYPE)) {
		std::cout << "a equals nil";
		areEqual = true;
	}
	else if (a->equals(b)) {
		areEqual = true;
	}

	return areEqual;
}
/******************************************************************************
 *****************************************************************************/
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
			//NOTE Should we take error message out of checkTypesEqual and make it return a bool?
			
			Type* aType = a[currentIndex];
			Type* bType = parameterTypeCheckBuffer[currentIndex];
			std::cout << "Atype = " << *aType << " bType= " << *bType;

			if (!checkTypesEqual(aType, bType)) {	// Neither types are Constants
				std::cout << "<<< "<<*aType << " " << *bType;
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
/******************************************************************************
 *****************************************************************************/
void addError(std::string s)
{
	errorLog.push_back(s);
}
/******************************************************************************
 *****************************************************************************/
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
		ss << "***ERROR(line: " << lineNumber << "): left or righthand Type in div/mod is NULL";
		addError(ss.str());
		return NULL;
	}
	
	bool leftIsInteger = INTEGER_TYPE->equals(left);
	bool rightIsInteger = INTEGER_TYPE->equals(right);
	
	bool leftIsReal = REAL_TYPE->equals(left);
	bool rightIsReal = REAL_TYPE->equals(right);
	
	if (!leftIsInteger && !leftIsReal) {
		ss << "***ERROR(line: " << lineNumber << "): wrong left hand arg type to \"*\"";
		addError(ss.str());
		return NULL;
	}

	if (!rightIsInteger && !rightIsReal) {
		ss << "***ERROR(line: " << lineNumber << "): wrong right hand arg type to \"mod\"";
		addError(ss.str());
		return NULL;
	}
	
	// No reals allowed for mod and div
	if (leftIsReal || rightIsReal)
	{
		ss << "***ERROR(line: " << lineNumber << "): either right or left hand args for \"div\" are reals";
		addError(ss.str());
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
		return NULL;
	}

	if (!rightIsInteger && !rightIsReal) {
		ss << "***ERROR(line: " << lineNumber << "): wrong right hand arg type to \"/\"";
		addError(ss.str());
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
		return NULL;
	}
	
	bool leftIsInteger = INTEGER_TYPE->equals(left);
	bool rightIsInteger = INTEGER_TYPE->equals(right);
	
	bool leftIsReal = REAL_TYPE->equals(left);
	bool rightIsReal = REAL_TYPE->equals(right);
	
	if (!leftIsInteger && !leftIsReal) {
		ss << "***ERROR(line: " << lineNumber << "): wrong left hand arg type to \"*/+/-\"";
		addError(ss.str());
		return NULL;
	}

	if (!rightIsInteger && !rightIsReal) {
		ss << "***ERROR(line: " << lineNumber << "): wrong right hand arg type to \"*/+/-\"";
		addError(ss.str());
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
	return NULL;
}

/******************************************************************************
 * getRawType(Type*)
 * 
 *****************************************************************************/
Type* getRawType(Type* type)
{
    SymbolicType *st = dynamic_cast<SymbolicType*>(type);
    if (st == NULL) {
        return type;
    }
    return st->getSymbol()->getMyType();
}

/******************************************************************************
 * getSymbolType(Symbol*)
 * 
 *****************************************************************************/
Type* getSymbolType(Symbol* s)
{
    Variable *var = dynamic_cast<Variable*>(s);
    if (var != NULL) {
        return getRawType(var->type);
    }
    
    TypeSymbol *typedSymbol = dynamic_cast<TypeSymbol*>(s);
    if (typedSymbol != NULL) {
        return getRawType(typedSymbol->getMyType());
    }
    
    std::cout << "Symbol \"" << s->name << "\" does not have a type!" << std::endl;
    return NULL;
}

/******************************************************************************
 * dereferenceDesignator()
 * 
 *****************************************************************************/
void dereferenceDesignator()
{
    if (designators.top() == NULL) {
        std::cout << "*** Error Cannot dereference NULL designator." << std::endl;
        return;
    }
    Type *designatorType = getSymbolType(designators.top());
    PointerType *ptr = dynamic_cast<PointerType*>(designatorType);
    if (ptr == NULL) {
        std::cout << "*** Error Cannot dereference \"" << designators.top()->name << "\"of non-pointer type." << std::endl;
        designators.pop();
        designators.push(NULL);
        return;
    }

    designators.pop();
    designators.push(ptr->getPointee());
    std::cout << "[0]" << flush;
}

/******************************************************************************
 * accessArray()
 * 
 *****************************************************************************/
void accessArray()
{
    if (designators.top() == NULL) {
        std::cout << "*** Error Cannot dereference NULL designator." << std::endl;
        return;
    }
    Type *designatorType = getSymbolType(designators.top());
    ArrayType *arrayType = dynamic_cast<ArrayType*>(designatorType);
    if (arrayType == NULL) {
        std::cout << "*** Error Cannot access value in symbol \"" << designators.top()->name << "\"of non-array type." << std::endl;
        designators.pop();
        designators.push(NULL);
        return;
    }
    
    for (int i = arrayType->ranges.size()-1; i > -1; i--) {
        accessedArrayRanges.push_front(arrayType->ranges[i]);
    }

    Symbol *newDesignator = new TypeSymbol("array-temp", arrayType->type);
    designators.pop();
    designators.push(newDesignator);
}

/******************************************************************************
 * accessField(const char*)
 * 
 *****************************************************************************/
void accessField(const char* ident)
{
    if (designators.top() == NULL) {
        std::cout << "*** Error Cannot dereference NULL designator." << 
            std::endl;
        return;
    }
    
    Type *designatorType = getSymbolType(designators.top());
    RecordType *recordType = dynamic_cast<RecordType*>(designatorType);
    if (recordType == NULL) {
        std::cout << "*** Error Cannot access field in \"" << 
            designators.top()->name << "\"of non-record type." << std::endl;
        designators.pop();
        designators.push(NULL);
        return;
    }
    
    Symbol *newDesignator = recordType->getField(std::string(ident));
    if (newDesignator == NULL) {
        std::cout << "*** Error Field \"" << ident << 
            "\" is not a valid field in record\"" << designators.top()->name 
            << "\"!" << std::endl;
    }
    designators.pop();
    designators.push(newDesignator);
    std::cout << "." << ident << std::flush;
}

/******************************************************************************
 * startBlock()
 * Generates the beginning of a code block.
 *****************************************************************************/
void startBlock()
{
    std::cout << getTabs() << (symbolTable.current)->name << "& call() {" << 
        std::endl;
}

/******************************************************************************
 * endBlock()
 * Generates the end of a code block.
 *****************************************************************************/
void endBlock()
{
    std::cout << getTabs() << "\treturn *this;" << std::endl;
    std::cout << getTabs() << "}" << std::endl;
}
