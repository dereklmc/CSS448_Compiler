#include "stdType.h"
    
StdType::StdType(std::string name) : Type()
{
    this->name = name;
}

std::string StdType::getName() const
{
    return this->name;
}	    

void StdType::print(std::ostream& out) const
{
     out << name; 
}

Type* StdType::clone() const
{
    Type *clone = new StdType(name);
    return clone;
}

std::string StdType::generateCode() const
{
    std::string tempString = "StdType code here!\n";
    return tempString;
}
