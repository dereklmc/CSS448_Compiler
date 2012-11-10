#ifndef SYMBOL_H
#define SYMBOL_H
#include <string>
#include <iostream>
using namespace std;
class Symbol
{
    friend std::ostream& operator<<(std::ostream& out, const Symbol s)
    {
        s.print(out);
    }

    private:
        string name;

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
	
	virtual void print(std::ostream& out) const
    	{
             out << name; 
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