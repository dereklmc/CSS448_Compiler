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
    return (limit2Str(end)+"-"+getStart()+"+1");
}

std::string ConstRange::getStart() const
{
    return limit2Str(start);
}

std::string ConstRange::limit2Str(ConstValue *limit) const
{
    if(limit->getType() == INTEGER || limit->getType() == SYMBOL || limit->getType() == CHAR)
        return limit->generateCode();

    return " ";
}

void ConstRange::print(std::ostream& out) const
{
    out << *start << " " << *end;
}
