#ifndef STACKFRAME_H
#define STACKFRAME_H
#include <string>
#include <iostream>
#include <vector>
#include "symbol.h"

class StackFrame 
{
    friend std::ostream& operator<<(std::ostream& out, const StackFrame& f)
    {
        f.print(out);
        return out;
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
            return true;
        }

	//Searches the entire StackFrame for a Symbol matching the name
	//If found, it will return a ptr to that Symbol
	//Else it will return NULL
        bool findMatch(std::string s, Symbol*& found)
        {
	    	//For each symbol in the stackframe, check if it matches name
            for(int x = 0; x < symbols.size(); x++)
	    	{
				if (symbols[x]->hasName(s))
				{
					found = symbols[x];
					return true;
				}
	    	}
	    	return false;
        }
};
#endif
