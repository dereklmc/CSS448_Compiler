#ifndef STACKFRAME_H
#define STACKFRAME_H
#include <string>
#include <iostream>
#include <vector>
#include "symbol.h"
#include "Parameter.h"

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
        
        int scope;
        
        void print(std::ostream& out) const
        {
            printScopeTabs(out);
            out << name << " " << "contains: (symbols= "<< symbols.size() 
<< std::endl;
	    int symbolSize = symbols.size();
            for(int x = symbolSize - 1; x > -1; x--) {
                printScopeTabs(out);
                out << "\t" << *symbols[x];
		out << "<test>" << std::endl;
            }
            out << std::endl;
        }
        
        void printScopeTabs(std::ostream &output) const
        {    
            for(int i = 0; i < scope; i++) {
                std::cout << "\t";
            }
        }

    public:
        StackFrame* previous;
        
        StackFrame(int scope, std::string name)
        {
            this->scope = scope;
            this->name = name;
            previous = NULL;
        }
        
        ~StackFrame() {
            for (int i = 0; i < symbols.size(); i++) {
                if (dynamic_cast<Parameter*>(symbols[i]) == NULL) {
                    delete symbols[i];
                }
            }
            symbols.clear();
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
