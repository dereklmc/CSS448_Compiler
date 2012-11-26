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

std::string SymbolicType::generateVarDeclCode() const
{
	std::string output = symbol->generateCode();
	return output;
}

std::string SymbolicType::generateTypeCode() const
{
	std::string output = symbol->generateCode();
    return output ;
}

bool SymbolicType::equals(Type *other) const
{
	SymbolicType *castedType = dynamic_cast<SymbolicType*>(other);
	if (castedType == NULL) {
		return false;
	}
	
	return *(castedType->symbol) == *symbol;
}

TypeSymbol* SymbolicType::getSymbol() const
{
	return symbol;
}
