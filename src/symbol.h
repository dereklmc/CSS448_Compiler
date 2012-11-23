#ifndef SYMBOL_H
#define SYMBOL_H
#include <string>
#include <iostream>
class Symbol
{
    friend std::ostream& operator<<(std::ostream& out, const Symbol &s)
    {
        s.print(out);
        return out;
    }

    public:
        std::string name;
        Symbol()
        {
        }
        
        Symbol(std::string name)
        {
            this->name = name;
        }
        
        virtual ~Symbol() {  }
        
        virtual bool operator==(const Symbol& rhs)
        {
            if(name==rhs.name) {
                return true;
            } else {
                return false;
            }
        }
        
        virtual void print(std::ostream& out) const
        {
             out << name; 
        }

	    bool hasName(std::string n)
	    {
	        if (name == n) {
		        return true;
	        } else {
		        return false;
	        }
	    }
};

#endif
