#ifndef STACK_H
#define STACK_H

#include "stackframe.h"
#include "stdType.h"
#include "TypeSymbol.h"

#include <iostream>

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
            currentScope = -1;
            zeroeth = new StackFrame(currentScope, "language");
            current = zeroeth;
            
            // Add standard idents to type.
            Symbol *symInt = new TypeSymbol("integer", new StdType("integer"));
            Symbol *symBool = new TypeSymbol("boolean", new StdType("boolean"));
            Symbol *symStr = new TypeSymbol("string", new StdType("string"));
            Symbol *symReal = new TypeSymbol("real", new StdType("real"));
            zeroeth->addSymbol(symInt);
            zeroeth->addSymbol(symBool);
            zeroeth->addSymbol(symStr);
            zeroeth->addSymbol(symReal);
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
};
#endif
