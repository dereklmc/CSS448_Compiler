#include "TypeSymbol.h"
        
// Standard ctr, see Symbol.
TypeSymbol::TypeSymbol(std::string name, Type *&type) :
        Symbol(name)
{
    this->type = type;
    type->hasSymbol = true;
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
