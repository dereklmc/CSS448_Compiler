#ifndef SETTYPE_H
#define SETTYPE_H

#include "Type.h"
#include "range.h"

#include <iostream>

class SetType : public Type
{
    public:
        SetType(Range* range)
        {
            this->range = range;
        }
        
        ~SetType()
        {
            delete range;
            range = NULL;
        }
        
        Type* clone() const
        {
            Type *clone = new SetType(range->clone());
            return clone;
        }
        
    private:
        Range* range;

    protected:
        void print(std::ostream& out) const
        {
            out << *range;
        }
        std::string generateCode() const
        {
            std::string tempString = "setType code here!\n";
            return tempString;
        }
};

#endif
