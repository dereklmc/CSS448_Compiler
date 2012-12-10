#include "stdtype.h"
#include "symbolictype.h"
#include <typeinfo>

StdType *INTEGER_TYPE = NULL;
StdType *BOOLEAN_TYPE = NULL;
StdType *STRING_TYPE = NULL;
StdType *REAL_TYPE = NULL;
StdType *CHAR_TYPE = NULL;
StdType *NIL_TYPE = NULL;

void initStdTypes()
{
    INTEGER_TYPE = new StdType("integer", "int");
    BOOLEAN_TYPE = new StdType("boolean", "bool");
    STRING_TYPE = new StdType("string", "string");
    REAL_TYPE = new StdType("real", "double");
    CHAR_TYPE = new StdType("char", "char");
	NIL_TYPE = new StdType("nil", "NULL");
}

StdType::StdType(std::string pname, std::string cname) : Type()
{
    this->pname = pname;
	this->cname = cname;
}

std::string StdType::getName() const
{
    return this->pname;
}	    

void StdType::print(std::ostream& out) const
{
     out << pname; 
}

Type* StdType::clone() const
{
    Type *clone = new StdType(pname, cname);
    return clone;
}

std::string StdType::generateVarDeclCode() const
{
	return cname;
}	

std::string StdType::generateTypeCode() const
{
    return cname;
}

bool StdType::equals(Type *other) const
{
	StdType *castedStdType = dynamic_cast<StdType*>(other);
	if (castedStdType == NULL) {
		SymbolicType *castedSymType = dynamic_cast<SymbolicType*>(other);
		if (castedSymType == NULL) {
			return false;
		}

		return (castedSymType->getSymbol()->name.compare(pname) == 0);
	}
	int result = castedStdType->pname.compare(pname);

	if (result == 0)
		return true;
	else 
		return false;
}
