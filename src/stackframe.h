#include <string>
#include <iostream>
#include <vector>
#include "symbol.h"
#ifndef STACKFRAME_H
#define STACKFRAME_H
#endif

class StackFrame 
{
    friend std::ostream& operator<<(std::ostream& out, const StackFrame& f)
    {
        f.print(out);
    }
    private:
        std::string name;
        std::vector<Symbol*> symbols; 
        void print(std::ostream& out) const
        {
            out << name << " " << "contains: " << std::endl;
            for(int x = 0; x < symbols.size(); x++)
                out << " " << *symbols[x];
            out << std::endl;
        }     

    public:
        StackFrame* previous;
        StackFrame(){};
        StackFrame(std::string n)
        {
            name = n;
        }
        bool addSymbol(Symbol* s)
        {
            
            symbols.push_back(s);
        }
        bool hasSymbol(const Symbol& s)
        {
            for(int x = 0; x < symbols.size(); x++)
                if(*symbols[x]==s)
                    return true;
            return false;
        }
};
