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
        PointerType(Symbol*);
        ~PointerType();
        // Defined to release PointerType's reference to Symbol.
        // Since pointee is also in the symbol table, the ST will deallocate 
        // pointee.
        Symbol* getPointee() const;
        void setPointee(Symbol*);
        Type* clone() const;
        std::string printCName() const {return "";};

    private:
        // The ident that the pointer points to.
        Symbol *pointee;
        
    protected:
        // overriden print method from Symbol. Prints name and pointee's name
        void print(std::ostream&) const;
        std::string generateTypeCode() const;
		std::string generateVarDeclCode() const;
		std::string generateOptionalCode() const {return "";};

};

#endif
