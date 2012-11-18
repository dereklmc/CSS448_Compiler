#include "TypeSymbol.h"
        
// Standard ctr, see Symbol.
TypeSymbol::TypeSymbol(std::string name, Type *&type) :
        Symbol(name)
{
    this->type = type;
    type->hasSymbol = true;
    type->symbolName = name;
}

TypeSymbol::~TypeSymbol()
{
    delete type;
    type = NULL;
}

void TypeSymbol::print(std::ostream& out) const
{
     Symbol::print(out);
     PointerType *ptr = dynamic_cast<PointerType*>(type);
     if (ptr != NULL) {
         ptr->print(out, false);
     } else {
        out << " " << *type;
     }
}
