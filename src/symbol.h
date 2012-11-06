#include <string>
#include <iostream>
#ifndef SYMBOL_H
#define SYMBOL_H
#endif

class Symbol
{
    friend std::ostream& operator<<(std::ostream& out, const Symbol s)
    {
        s.print(out);
    }
    private:
        std::string name;
        virtual void print(std::ostream& out) const
        {
           out << name; 
        }
    public:
        Symbol(std::string n)
        {
            name = n;
        }
        Symbol(const Symbol& s)
        {
            name = s.name;
        }
        virtual bool operator==(const Symbol& rhs)
        {
            if(name==rhs.name)
                return true;
            else
                return false;
        }
};
