#include "TypeSymbol.h"
        
// Standard ctr, see Symbol.
TypeSymbol::TypeSymbol(std::string name, Type *&type) :
        Symbol(name)
{
    this->type = type;
}

TypeSymbol::~TypeSymbol()
{
    delete type;
    type = NULL;
}

void TypeSymbol::print(std::ostream& out) const
{
     Symbol::print(out);
     out << " " << *type;
}

std::string TypeSymbol::generateCode()
{
	return type->generateCode();
}
