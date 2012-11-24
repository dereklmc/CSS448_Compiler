#ifndef STDTYPE_H
#define STDTYPE_H

#include "Type.h"

#include <vector>

class StdType : public Type
{
    
	public:
	    StdType(std::string pname, std::string cname);
    
        std::string getName() const;
        
        Type* clone() const;
    	std::string generateCode() const;
	private:
	    std::string pname;
		std::string cname;
	    void print(std::ostream& out) const;
};

#endif
