#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include "Type.h"

template <class T>
class ArrayType : public Type
{
    public:
        ArrayType(Type* t) {type = t;};
        void print(std::ostream& out) 
        {
            for(int x = 0; x < vector.size(); x++)
                out << "[" << ranges[0] << "] ";
            out << std::endl;
        };
    private:
        Type* type;
        std::vector<Range<T>> ranges;
}
#endif
