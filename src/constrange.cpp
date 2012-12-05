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

int ConstRange::getLength() const
{
    if(start->getType() == INTEGER)
    {
        std::stringstream tempEndStream, tempStartStream;
        std::cout << "END CODE: " << end->generateCode() << std::endl;
        tempEndStream << end->generateCode(); 
        int tempEnd, tempStart;
        tempEndStream >> tempEnd;
        tempStartStream << start->generateCode();
        tempStartStream >> tempStart;
        return tempEnd-tempStart+1;
    }
    if(start->getType() == CHAR)
        return (end->generateCode()[0]-start->generateCode()[0]+1);
    return -1;
}

void ConstRange::print(std::ostream& out) const
{
    out << *start << " " << *end;
}
