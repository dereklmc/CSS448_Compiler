#ifndef RANGE_H
#define RANGE_H

#include "constvalue.h"

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
        
    protected:
        void print(std::ostream& out) const
        {
            out << *start << " " << *end;
        }
        
    private:
        ConstValue *start, *end;

};

#endif
