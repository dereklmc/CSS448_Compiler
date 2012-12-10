#ifndef STACK_H
#define STACK_H

#include "stackframe.h"
#include "stdtype.h"
#include "TypeSymbol.h"
#include "constant.h"
#include <iostream>
using namespace std;
class Stack
{
    friend std::ostream& operator<<(std::ostream& out, const Stack& s)
    {
        s.print(out);
        return out;
    }
    
    public:
        // Public pointers are bad, mmkay?
        StackFrame* current;
        StackFrame* zeroeth;
        
        int currentScope;
        int currentControlScope;
        
        // Constructor
        Stack()
        {
            currentScope = 0;
            currentControlScope = 0;
            zeroeth = new StackFrame(currentScope, "language");
            current = zeroeth;
            
            // Add standard idents to type.
            //Type *intType = new StdType("integer", "int");
            //Type *boolType = new StdType("boolean", "bool");
            //Type *strType = new StdType("string", "string");
            //Type *realType = new StdType("real", "double");
            initStdTypes();
            Symbol *symInt = new TypeSymbol(std::string("integer"), INTEGER_TYPE);
            Symbol *symBool = new TypeSymbol("boolean", BOOLEAN_TYPE);
            Symbol *symStr = new TypeSymbol("string", STRING_TYPE);
            Symbol *symReal = new TypeSymbol("real", REAL_TYPE);
	    	Symbol *symChar = new TypeSymbol("char", CHAR_TYPE);
            Symbol *symTrue = new Constant("true", new ConstValue("true", BOOLEAN));
            Symbol *symFalse = new Constant("false", new ConstValue("false", BOOLEAN));
			Symbol *symNil = new Constant("NULL", new ConstValue("NULL", NIL));
            zeroeth->addSymbol(symInt);
            zeroeth->addSymbol(symBool);
            zeroeth->addSymbol(symStr);
            zeroeth->addSymbol(symReal);
	        zeroeth->addSymbol(symChar);
            zeroeth->addSymbol(symTrue);
            zeroeth->addSymbol(symFalse);
			zeroeth->addSymbol(symNil);
        }
        
        // Destructor
        ~Stack()
        {
            while(current != zeroeth)
            {
                StackFrame* temp = current;
                current = current->previous;
                delete temp;
            }
            zeroeth = NULL;
            delete current;
            current = NULL;
        }
        //---------------------------------------------------------------------
        //                          createScope
        // Creates the StackFrame with the given string, appends it to the
        // Linked-List.
        bool createScope(std::string s)
        {
            currentScope++;
            StackFrame* temp = new StackFrame(currentScope,s);
            temp->previous = current;
            current = temp;
            return true;
        }
        //---------------------------------------------------------------------
        //                          createScope
        // Creates the StackFrame with the given string for a control structure,
        // appends it to the Linked-List.
        bool createControlScope(std::string s)
        {
            createScope(s);
            currentControlScope++;
            return true;
        }
        //---------------------------------------------------------------------
        //                          leaveScope
        // "Pops" off the current StackFrame and returns a pointer to it.
        StackFrame* leaveScope()
        {
            StackFrame* temp = current;
            current = current->previous;
            currentScope--;
            return temp;
        }
        //---------------------------------------------------------------------
        //                          leaveScope
        // "Pops" off the current control structure StackFrame and returns a 
        // pointer to it.
        StackFrame* leaveControlScope()
        {
            currentControlScope--;
            return leaveScope();
        }

        //---------------------------------------------------------------------
        //                          searchStack
        // Searches Stack for a symbol with the specific name, returning it if
        // it's found, null if not found.
        bool searchStack(std::string s, Symbol*& found)
        {
            return findSymbol(s,found) != -1;
        }
        
        //---------------------------------------------------------------------
        //                          searchStack
        // Searches Stack for a symbol with the specific name, returning the
        // distance between the current scope and the scope in which the symbol
        // exists. Returns -1 if not found.
        int findSymbol(std::string s, Symbol*& found)
        {
            int distance = 0;
            StackFrame* temp = current;
            while(temp != NULL)
            {
				//Now calls StackFrame's findMatch which will return the matching
				//Symbol if found, else NULL
				bool wasFound = temp->findMatch(s, found);
				if (wasFound) {
				    if (distance < currentControlScope){
				        return 0;
			        } else {
					    return distance - currentControlScope;
				    }
				}
				temp = temp->previous;
				distance++;
            }
            return -1;
        }

        void print(std::ostream& out) const
        {
            StackFrame* temp = current;
            while(temp != NULL)
            {
                out<<*temp;
            }
        }

		std::string getCurrentTabs()
		{
			return current->getMyTabs();
		}
};
#endif
