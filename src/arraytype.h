#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include "Type.h"
#include "range.h"

class ArrayType : public Type
{

    public:
        ArrayType(Type *type)
        {
            this->type = type;
        }
        
        ~ArrayType()
        {
            for (int i = 0; i < ranges.size(); i++) {
                delete ranges[i];
            }
            ranges.clear();
            delete type;
            type = NULL;
        }
        
        void addRange(Range *range)
        {
            ranges.push_back(range);
        }
        
        Type* clone() const
        {
            ArrayType *clone = new ArrayType(type->clone());
            for (int i = 0; i < ranges.size(); i++) {
                clone->ranges.push_back(ranges[i]->clone());
            }
            return clone;
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
        std::string generateCode() const
        {
            std::string tempString = "RecordType code here!\n";
            return tempString;
        }

};
#endif
