#include <string>
#include <iostream>
#include <unordered_set>
#include "symbol.h"
#ifndef STACKFRAME_H
#define STACKFRAME_H

class StackFrame 
{
    friend ostream& operator<<(ostream& out, const StackFrame& f)
    {
        f.print(out);
    }
    private:
        string name;
        std::unordered_set<Symbol*> symbols;
        StackFrame* previous;
    public:
        StackFrame(){};
        StackFrame(string n)
        {
            name = n;
        }
        bool addSymbol(Symbol s)
        {
            symbols.insert(s);
        }
        bool hasSymbol(Symbol s)
        {
            if(unordered_set.find(s)!=unordered_set.end())
                return true;
            else
                return false;
        }
        void print(ostream& out)
        {
            out << name << " " << "contains: " << std::endl;
            for ( auto it = symbol.begin(); it != symbol.end(); ++it )
                out << " " << *it;
            out << std::endl;
        }

}
