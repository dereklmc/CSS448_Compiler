#include "stdtype.h"
#include "symbolictype.h"

StdType *INTEGER_TYPE = new StdType("integer", "int");
StdType *BOOLEAN_TYPE = new StdType("boolean", "bool");
StdType *STRING_TYPE = new StdType("string", "string");
StdType *REAL_TYPE = new StdType("real", "double");

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
		return castedSymType->getSymbol()->name == pname;
	}
	
	return castedStdType->pname == pname;
}
