#include "symbol.h"

Symbol::Symbol(std::string name)
{
    this->name = name;
}

bool Symbol::operator==(const Symbol& rhs)
{
    return name == rhs.name;
}

void Symbol::print(std::ostream& out) const
{
     out << name; 
}

std::string Symbol::getName()
{
    return name;
}

bool Symbol::hasName(std::string n) const
{
    if (name == n) {
        return true;
    } else {
        return false;
    }
}

std::string Symbol::generateCode() const
{
    return name;
}
	    
