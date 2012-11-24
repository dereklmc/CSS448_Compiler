#include "stdType.h"
    
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
