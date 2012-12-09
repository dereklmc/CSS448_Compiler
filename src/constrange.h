#ifndef CONSTRANGE_H
#define CONSTRANGE_H

#include "range.h"

class ConstRange : public Range
{

    public:
        ConstRange(ConstValue *start, ConstValue *end);
        
        ~ConstRange();
        
        Range* clone() const;

        std::string getLength() const;
        std::string getStart() const;
        
    protected:
        void print(std::ostream& out) const;

    private:
        ConstValue *start, *end;
        
        std::string limit2Str(ConstValue *) const;

};

#endif
