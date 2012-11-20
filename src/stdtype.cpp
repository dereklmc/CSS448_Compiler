#include "stdType.h"
    
StdType::StdType(std::string name) : Type() {
    this->name = name;
}

std::string StdType::getName() const {
    return this->name;
}	    

void StdType::print(std::ostream& out) const
{
     out << name; 
}
std::string StdType::generateCode() const
{
    std::string tempString = "StdType code here!\n";
    return tempString;
}
