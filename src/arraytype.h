#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include "Type.h"
#include "range.h"
#include <sstream>
#include <vector>
#include <iostream>

class ArrayType : public Type
{

    public:
        ArrayType(Type *type)
        {
            this->type = type;
            typeDefed = false;
            AR = true;
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

    	std::string printCName() const {return "";}
    	
        Type *type;
        std::vector<Range*> ranges;

				std::string getRanges() const {
			std::ostringstream tempString;

            for(int x = 0; x < ranges.size(); x++)
            {
                tempString << "[";
                tempString << ranges[x]->getLength();
                tempString << "]";
            }

		    return tempString.str();
		}
        
    private:
		std::string optionalCode;
        
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
            if(!typeDefed)
            {
                tempString << optionalCode;
                for(int x = 0; x < ranges.size(); x++)
                {
                    tempString << "[";
                    tempString << ranges[x]->getLength();
                    tempString << "]";
                }
            }
		    return tempString.str();
		}

};
#endif
