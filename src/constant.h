#include "symbol.h"
#ifndef CONSTANT_H
#define CONSTANT_H
#endif 

template <class T>
class Constant : public Symbol
{
    private:
        T type;
    public:
        Constant(std::string n, T t) : Symbol(n)
        {
            type = t;
        }
        void print(std::ostream& out) const
        {
            out << name << " " << type;
        }
};
