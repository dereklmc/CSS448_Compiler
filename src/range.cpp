#ifndef RANGE_H
#define RANGE_H

#include "constvalue.h"
#include <sstream>

friend std::ostream& operator<<(std::ostream& out, const Range &r)
{
    r.print(out);
    return out;
}


CharRange::CharRange(char start, char end)
{
    this->start = start;
    this->end = end;
}

Range* CharRange::clone() const
{
    return new CharRange(start, end);
}

std::string CharRange::getLength() const
{
    std::stringstream temp;
    temp << (end-start);
    std::string tempString = temp.str();
    return tempString;
}

std::string CharRange::getStart() const
{
    std::stringstream temp;
    temp << start;
    std::string tempString = temp.str();
    return tempString;
}

void CharRange::print(std::ostream& out) const
{
    out << start << " " << end;
}

