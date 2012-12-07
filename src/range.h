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

#endif
