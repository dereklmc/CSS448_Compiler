#include "stackframe.h"
#include <iostream>
#ifndef STACK_H
#define STACK_H
#endif

class Stack
{
    friend std::ostream& operator<<(std::ostream& out, const Stack& f)
    {
        f.print(out);
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
        // Searches Stack for a frame with the specific name, returning it if
        // it's found, null if empty.
        StackFrame* searchStack(const Symbol& s)
        {
            StackFrame* temp = current;
            while(temp != NULL)
            {
                if(temp->hasSymbol(s))
                    break;
                else
                    temp = temp->previous;
            }
            return temp;
        }
        void print(std::ostream& out)
        {
            StackFrame* temp = current;
            while(temp != NULL)
            {
                out<<*temp;
            }
        }
};
