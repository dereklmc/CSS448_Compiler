#ifndef CONSTANT_H
#define CONSTANT_H

#include "symbol.h"
#include "constvalue.h"

class Constant : public Symbol
{
	
    private:
         ConstValue *value;
         
    public:
        Constant(std::string, ConstValue *);
        
        ~Constant();
        
        std::string generateCode() const;
        
        void print(std::ostream&) const;
};

#endif
