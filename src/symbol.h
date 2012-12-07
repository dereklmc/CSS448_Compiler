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
            return name == rhs.name;
        }
        
        virtual void print(std::ostream& out) const
        {
             out << name; 
        }

		std::string getName()
		{
			return name;
		}

	    bool hasName(std::string n)
	    {
	        if (name == n) {
		        return true;
	        } else {
		        return false;
	        }
	    }
	    
	    virtual std::string generateCode() const { return name; }
	    
	    virtual Symbol* clone() const { std::cerr << "*** Symbol#clone() called! Should be pure virtual ***" << std::endl; return NULL; }
};

#endif
