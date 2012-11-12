#ifndef RANGE_H
#define RANGE_H

template <class T>
class Range
{
    protected:
        T start;
        T end;

    public:
        bool insideRange(T& value);
        bool operator==(const Constant& rhs)
        {
            if(start == rhs.start)
                return true;
            return false;
        }
};

#endif
