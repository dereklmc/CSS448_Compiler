#ifndef STDTYPE_H
#define STDTYPE_H

#include "Type.h"

#include <vector>

class StdType : public Type
{
    
	public:
	    StdType(std::string name);
    
        std::string getName() const;
    
	private:
	    std::string name;
	    

	    void print(std::ostream& out) const;
};

#endif
