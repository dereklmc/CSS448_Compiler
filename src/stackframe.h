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
        std::vector<Symbol*> symbols; 
        
        int scope;
        
        void print(std::ostream& out) const
        {
            printScopeTabs(out);
            out << name << " " << "contains: " << std::endl;
            int symbolSize = symbols.size();
            for(int x = 0; x < symbolSize; x++) {
                printScopeTabs(out);
                out << "\t" << *symbols[x];
                out << std::endl;
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
        std::string name;
        
        StackFrame(int scope, std::string name)
        {
            this->scope = scope;
            this->name = name;
            previous = NULL;
        }
        
        ~StackFrame()
        {
            for (int i = 0; i < symbols.size(); i++) {
                delete symbols[i];
            }
            symbols.clear();
        }
        
        bool addSymbol(Symbol *s)
        {
            symbols.push_back(s);
            findMatch(s->name, s);
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

		std::string getMyTabs()
		{
			std::string retVal = "";
			for(int i = 0; i < scope; i++) {
                retVal += "    ";
            }
			return retVal;
		}
        
};
#endif
