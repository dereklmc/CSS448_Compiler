#ifndef VARIABLE_H
#define VARIABLE_H

#include "symbol.h"
#include "Type.h"

#include <iostream>

class Variable : public Symbol {

    public:
        Variable(string, Type*);
        virtual ~Variable();

    private:
        Type *type;
        
    protected:
        void print(std::ostream&) const;

};

#endif
