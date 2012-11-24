#include "symbolictype.h"

SymbolicType::SymbolicType(TypeSymbol *s)
{
    symbol = s;
}

SymbolicType::~SymbolicType()
{
    symbol = NULL;
}

// overriddent print method from Type.
void SymbolicType::print(std::ostream &out) const
{
    out << symbol->name;
}

Type* SymbolicType::clone() const
{
    SymbolicType *clone = new SymbolicType(symbol);
    return clone;
}

std::string SymbolicType::generateCode() const
{
    // TODO write
	std::string output = symbol->generateCode();
    return output ;
}
