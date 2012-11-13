#include "stackframe.h"
#include <iostream>
#ifndef STACK_H
#define STACK_H

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
        // Constructor
        Stack(){zeroeth = new StackFrame(); current = zeroeth;}
        // Destructor
        ~Stack()
        {
            while(current!=zeroeth)
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
            StackFrame* temp = new StackFrame(s);
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
				if (wasFound)
					return true;
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
