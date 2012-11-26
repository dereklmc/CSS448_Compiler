#include "TypeSymbol.h"
        
// Standard ctr, see Symbol.
TypeSymbol::TypeSymbol(std::string name, Type *type) :
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

/******************************************************************************
 * generateCode
 * Used when generating code for variable declarations
 */
std::string TypeSymbol::generateCode()
{
	std::string cName = type->printCName();
	if (cName == "")
		return name + type->generateVarDeclCode();
	else
		return type->generateVarDeclCode();
}

/******************************************************************************
 * generateTypeDeclCode
 * Used when generating code for variable declarations
 */
std::string TypeSymbol::generateTypeDeclCode()
{
	return ("typedef " + type->generateOptionalCode()+ " " + name + type->generateTypeCode() + ";");
};
