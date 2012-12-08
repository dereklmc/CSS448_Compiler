#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include "Type.h"
#include "range.h"
#include <sstream>

#include <iostream>

class ArrayType : public Type
{

    public:
        ArrayType(Type *type)
        {
            this->type = type;
            typeDefed = false;
            optionalCode = type->generateTypeCode();
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

    	std::string printCName() const {return "";};
    private:
        Type *type;
		std::string optionalCode;
        std::vector<Range*> ranges;
        
    protected:
        void print(std::ostream& out) const
        {
            for (int i = 0; i < ranges.size(); i++) {
                out << *ranges[i];
            }
            out << " " << *type;
        }

		std::string generateOptionalCode() const 
		{	
				std::string temp = optionalCode;
				return temp;

		}

        std::string generateTypeCode() const
        {
			std::ostringstream tempString;
            for(int x = 0; x < ranges.size(); x++)
            {
                tempString << "[";
                tempString << ranges[x]->getLength();
                tempString << "]";
            }
		    return tempString.str();
        }

		std::string generateVarDeclCode() const
		{
			std::ostringstream tempString;
            tempString << optionalCode;
            if(!typeDefed)
            {
                for(int x = 0; x < ranges.size(); x++)
                {
                    tempString << "[";
                    tempString << "]";
                }
            }
		    return tempString.str();
		}

};
#endif
