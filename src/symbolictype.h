#ifndef SYMBOLICTYPE_H
#define SYMBOLICTYPE_H

#include "Type.h"
#include "TypeSymbol.h"

class SymbolicType : public Type {

    public:
        SymbolicType(TypeSymbol *);
        ~SymbolicType();
        
        Type* clone() const;
        std::string generateCode() const;
    
    protected:
        // overriddent print method from Type.
        void print(std::ostream&) const;

    private:
        TypeSymbol *symbol;

};

#endif
