#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include "Type.h"

template <class T>
class ArrayType : public Type
{
    private:
        Type* type;
        std::vector<Range<T>> range;
}
#endif
