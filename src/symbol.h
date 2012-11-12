#ifndef SYMBOL_H
#define SYMBOL_H
#include <string>
#include <iostream>
class Symbol
{
    friend std::ostream& operator<<(std::ostream& out, const Symbol s)
    {
        s.print(out);
    }
    
    protected:
        virtual void print(std::ostream& out) const
        {
             out << name; 
        }

    public:
        string name;
        Symbol(std::string n) : name(n)
        {
        }
        virtual bool operator==(const Symbol& rhs)
        {
            if(name==rhs.name)
                return true;
            else
                return false;
        }

	bool hasName(std::string n)
	{
	    if (name == n)
		return true;
	    else
		return false;
	}
};

#endif
