#ifndef CONSTANT_H
#define CONSTANT_H

#include "symbol.h"
#include "constvalue.h"

class Constant : public Symbol
{

    friend std::ostream& operator<<(std::ostream& out, const Constant s)
	{
    	s.print(out);
	    return out;
	}
	
    private:
         ConstValue *value;
         
    public:
        Constant(std::string name, ConstValue *value) : Symbol(name)
        {
            this->value = value;
        }
        
        ~Constant() {
            delete value;
            value = NULL;
        }
        
	protected:
        void print(std::ostream& out) const
        {
        	Symbol::print(out);
			out << " " << *value;
        }
};

#endif
