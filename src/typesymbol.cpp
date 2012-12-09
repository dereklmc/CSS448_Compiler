#include "TypeSymbol.h"
#include <typeinfo>
#include "RecordType.h"
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

Type* TypeSymbol::getMyType() const
{
	return type;
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
	RecordType* record = dynamic_cast<RecordType*>(type);
	if (record != NULL)
		return (type->generateOptionalCode()+ " " + name + type->generateTypeCode());
	return ("typedef " + type->generateOptionalCode()+ " " + name + type->generateTypeCode());
};

/******************************************************************************
 * clone
 * TODO
 */
Symbol* TypeSymbol::clone() const
{
    return new TypeSymbol(name, type->clone());
}

std::string TypeSymbol::getName() const
{
    return name;
}
