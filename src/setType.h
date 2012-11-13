#ifndef SETTYPE_H
#define SETTYPE_H

#include "Type.h"
#inlcude "Range.h"

#include <iostream>

template <class T>
class SetType : public Type
{
    public:
        SetType(Range<T> r) : Type()
        {
            range = r;
        }
    private:
        Range<T> range;

    protected:
        void print(std::ostream& out) const
        {
            Type::print(out);
            range.print(out);
        }
};

#endif
