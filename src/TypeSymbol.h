#ifndef TYPESYMBOL_H
#define TYPESYMBOL_H

#include "symbol.h"
#include "Type.h"
#include "PointerType.h"

/**
 * Represents an identifier for an arbitrary type in pascal.
 */
class TypeSymbol : public Symbol {
    
    public:        
        // Standard ctr, see Symbol.
        TypeSymbol(std::string, Type *&);
        ~TypeSymbol();
        std::string generateCode();
        void print(std::ostream&) const;
    
    private:
        Type *type;
};

#endif
