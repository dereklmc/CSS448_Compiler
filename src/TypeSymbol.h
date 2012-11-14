#ifndef TYPESYMBOL_H
#define TYPESYMBOL_H

#include "symbol.h"
#include "Type.h"

/**
 * Represents an identifier for an arbitrary type in pascal.
 */
class TypeSymbol : public Symbol {
    
    public:
        Type *type;
        
        // Standard ctr, see Symbol.
        TypeSymbol(std::string, Type *);
        ~TypeSymbol();
    
    protected:
        
        void print(std::ostream&) const;
};

#endif
