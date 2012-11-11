#ifndef STDTYPE_H
#define STDTYPE_H

#include "symbol.h"

class StdType : public Symbol
{

friend std::ostream& operator<<(std::ostream& out, const StdType s)
{
    s.print(out);
}
	public:
	static const StdType INTEGER;
	static const StdType BOOL;
	static const StdType REAL;
	static const StdType CHAR;

	StdType() : Symbol("") { }
	StdType(string name) : Symbol(name) { }

	private:

	protected:
	void print(std::ostream& out) const
    {
         out << name; 
    }
};

#endif
