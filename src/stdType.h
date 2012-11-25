#ifndef STDTYPE_H
#define STDTYPE_H

#include "Type.h"

#include <vector>

class StdType : public Type
{
    
	public:
	    StdType(std::string pname, std::string cname);
    
        std::string getName() const;
        std::string printName() {return cname;};
        Type* clone() const;
    	std::string generateTypeCode() const;
		std::string generateVarDeclCode() const;
		std::string generateOptionalCode() const {return "";};
		std::string printCName() const {return cname;};
	private:
	    std::string pname;
		std::string cname;
	    void print(std::ostream& out) const;
};

#endif
