#ifndef POINTERTYPE_H
#define POINTERTYPE_H

#include "Type.h"

#include <iostream>

/**
 * Represents an identifier for a pointer in Pascal.
 * A pointer is a name and a reference to a symbol in the symbol table.
 */
class PointerType : public Type {

    public:
        // ctr, creates a variable with a name that points to a given ident.
        PointerType( Symbol*);
        ~PointerType();
        // Defined to release PointerType's reference to Symbol.
        // Since pointee is also in the symbol table, the ST will deallocate 
        // pointee.
        Symbol* getPointee() const;
        Type* clone() const;

    private:
        // The ident that the pointer points to.
        Symbol *pointee;
        
    protected:
        // overriden print method from Symbol. Prints name and pointee's name
        void print(std::ostream&) const;
        std::string generateCode() const;

};

#endif
