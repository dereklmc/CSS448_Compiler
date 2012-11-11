#ifndef TYPE_H
#define TYPE_H

#include "symbol.h"

/**
 * Represents an identifier for an arbitrary type in pascal. This is an
 * abstract base class for all types in pascal, including:
 * * Records
 * * Pointers
 * * Arrays
 * * Sets
 *
 * Currently, type is just a symbol with another name. Type may have specific
 * methods in later phases of the compiler.
 */
class Type : public Symbol {
    
    public:
        // Standard ctr, see Symbol.
        Type(string);
};

#endif
