#include STACKFRAME_H
#include <unodered_set>
#ifndef STACK_H
#def STACK_H

class Stack
{
    public:
        StackFrame* current;
        StackFrame* zeroeth;

        Stack(){zeroeth = new StackFrame(); current = zeroeth;}

        ~Stack()
        {
            while(current!=zeroeth)
            {
                StackFrame* temp = current;
                current = current->previous;
                delete temp;
            }
            zeroeth = null;
            delete current;
            current = null;
        }
        bool createScope()
        {
            return false;
        }
        bool leaveScope()
        {
            return false;
        }
        bool searchStack()
        {
            return false;
        }
}
