#ifndef CONSTRANGE_H
#define CONSTRANGE_H

#include "range.h"

class ConstRange : public Range
{

    public:
        ConstRange(ConstValue *start, ConstValue *end);
        
        ~ConstRange();
        
        Range* clone() const;

        int getLength() const;
        
    protected:
        void print(std::ostream& out) const;

    private:
        ConstValue *start, *end;

};

#endif
