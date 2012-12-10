#include "arraytype.h"
#include "Type.h"
#include <iostream>
#include <sstream>
#include "range.h"

ArrayType::ArrayType(Type *type)
{
    this->type = type;
    typeDefed = false;
    AR = true;
    optionalCode = type->generateTypeCode();
}

ArrayType::~ArrayType()
{
    for (int i = 0; i < ranges.size(); i++) {
        delete ranges[i];
    }
    ranges.clear();
    delete type;
    type = NULL;
}

void ArrayType::addRange(Range *range)
{
    ranges.push_back(range);
}


Type* ArrayType::clone() const
{
    ArrayType *clone = new ArrayType(type->clone());
    for (int i = 0; i < ranges.size(); i++) {
        clone->ranges.push_back(ranges[i]->clone());
    }
    return clone;
}

void ArrayType::print(std::ostream& out) const
{
    for (int i = 0; i < ranges.size(); i++) {
        out << *ranges[i];
    }
    out << " " << *type;
}

std::string ArrayType::generateOptionalCode() const 
{	
		std::string temp = optionalCode;
		return temp;
}

std::string ArrayType::generateTypeCode() const
{
	std::ostringstream tempString;
    for(int x = 0; x < ranges.size(); x++)
    {
        tempString << "[";
        tempString << ranges[x]->getLength();
        tempString << "]";
    }
    return tempString.str();
}

std::string ArrayType::generateVarDeclCode() const
{
	std::ostringstream tempString;
    if(!typeDefed)
    {
        tempString << optionalCode;
        for(int x = 0; x < ranges.size(); x++)
        {
            tempString << "[";
            tempString << ranges[x]->getLength();
            tempString << "]";
        }
    }
    return tempString.str();
}
