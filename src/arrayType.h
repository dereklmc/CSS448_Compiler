#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include "Type.h"
#include "range.h"

class ArrayType : public Type
{
    friend std::ostream& operator<<(std::ostream& out, const ArrayType s)
    {
	    s.print(out);
	    return out;
    }
    public:
        ArrayType(Type *type)
        {
            this->type = type;
        }
        
        ~ArrayType()
        {
            std::cout << "DTR ARRAY" << std::endl; 
            if (type != NULL && !(type->hasSymbol)) {
                std::cout << "DELETE VARIABLE TYPE " << *type << std::endl;
                delete type;
            }
            for (int i = 0; i < ranges.size(); i++) {
                delete ranges[i];
            }
            type = NULL;
        }
        
        void addRange(Range *range)
        {
            ranges.push_back(range);
        }
    
    private:
        Type *type;
        std::vector<Range*> ranges;
        
    protected:
        void print(std::ostream& out) const
        {
            for (int i = 0; i < ranges.size(); i++) {
                out << *ranges[i];
            }
            out << " " << *type;
        }
};
#endif
