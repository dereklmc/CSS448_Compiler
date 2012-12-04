#ifndef RANGE_H
#define RANGE_H

#include "constvalue.h"
#include <sstream>

class Range
{
    friend std::ostream& operator<<(std::ostream& out, const Range &r)
    {
        r.print(out);
        return out;
    }
    
    public:
        virtual Range* clone() const =0;
        virtual ~Range() { }
        virtual int getLength() const { return -1; } 
    
    protected:
        virtual void print(std::ostream& out) const =0;
};

class CharRange : public Range
{

    public:
        CharRange(char start, char end)
        {
            this->start = start;
            this->end = end;
        }
        
        Range* clone() const
        {
            return new CharRange(start, end);
        }
        
        int getLength() const
        {
            return (end-start);
        }
    protected:
        void print(std::ostream& out) const
        {
            out << start << " " << end;
        }
        
    private:
        char start, end;

};

class ConstRange : public Range
{

    public:
        ConstRange(ConstValue *start, ConstValue *end)
        {
            this->start = start;
            this->end = end;
        }
        
        ~ConstRange()
        {
            delete start;
            start = NULL;
            delete end;
            end = NULL;
        }
        
        Range* clone() const
        {
            return new ConstRange(new ConstValue(*start), new ConstValue(*end));
        }

        int getLength() const
        {
            if(start->getType() == INTEGER)
            {
                std::stringstream temp;
                std::cout << "END CODE: " << end->generateCode()<< std::endl;
                temp << end->generateCode(); 
                int tempEnd, tempStart;
                temp >> tempEnd;
                temp << start->generateCode();
                temp >> tempStart;
                return tempEnd-tempStart+1;
            }
            if(start->getType() == CHAR)
                return (end->generateCode()[0]-start->generateCode()[0]+1);
            return -1;
        }
        
    protected:
        void print(std::ostream& out) const
        {
            out << *start << " " << *end;
        }
        
    private:
        ConstValue *start, *end;

};

#endif
