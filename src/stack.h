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
        
        // Constructor
        Stack()
        {
            currentScope = 0;
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
        //                          searchStack
        // Searches Stack for a symbol with the specific name, returning it if
        // it's found, null if not found.
        bool searchStack(std::string s, Symbol*& found)
        {
            StackFrame* temp = current;
            while(temp != NULL)
            {
				//Now calls StackFrame's findMatch which will return the matching
				//Symbol if found, else NULL
				bool wasFound = temp->findMatch(s, found);
				if (wasFound) {
					return true;
				}
				temp = temp->previous;
            }
            return false;
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
