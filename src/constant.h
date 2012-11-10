#ifndef CONSTANT_H
#define CONSTANT_H
#include "symbol.h"

template <class T>
class Constant : public Symbol
{
    private:
        T type;
    public:
	friend std::ostream& operator<<(std::ostream& out, const Constant s)
    	{
        	s.print(out);
		return out;
    	}
        Constant(std::string n, T t) : Symbol(n)
        {
            type = t;
        }
        void print(std::ostream& out) const
        {
            	Symbol::print(out);
		out << " " << type;
        }
};

#endif
