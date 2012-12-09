#include "constrange.h"

ConstRange::ConstRange(ConstValue *start, ConstValue *end)
{
    
    this->start = start;
    this->end = end;
}

ConstRange::~ConstRange()
{
    delete start;
    start = NULL;
    delete end;
    end = NULL;
}

Range* ConstRange::clone() const
{
    return new ConstRange(new ConstValue(*start), new ConstValue(*end));
}

std::string ConstRange::getLength() const
{
    std::stringstream tempEndStream, tempStartStream;
    // check start types
    if(start->getType() == INTEGER || start->getType() == SYMBOL)
        tempStartStream << start->generateCode();
    if(start->getType() == CHAR)
        tempStartStream << "'"<< start->generateCode()[0] << "'";

    // check end types
    if(end->getType() == INTEGER || end->getType() == SYMBOL)
        tempEndStream << end->generateCode();
    if(end->getType() == CHAR)
        tempEndStream << "'" << end->generateCode()[0] << "'";


    return (tempEndStream.str()+"-"+tempStartStream.str()+"+1");
}

void ConstRange::print(std::ostream& out) const
{
    out << *start << " " << *end;
}
