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
        Symbol() { }
        Symbol(std::string name);
        virtual ~Symbol() { }
        
        bool operator==(const Symbol& rhs);
        
		std::string getName();
	    bool hasName(std::string n) const;
	    
        virtual void print(std::ostream& out) const;
	    virtual std::string generateCode() const;
};

#endif
