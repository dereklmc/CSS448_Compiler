#ifndef VARIABLE_H
#define VARIABLE_H

#include "symbol.h"
#include "Type.h"

#include <iostream>

/**
 * Represents an identifier for a variable and a field in a record.
 * Has both a name and type.
 */
class Variable : public Symbol {

    public:
        Variable();
        // ctr, creates a variable with a name and a type.
        Variable(std::string, Type*);
        Variable(const Variable&);
        // Defined to release Variable's reference to Type.
        // Since type is also in the symbol table, the ST will deallocate type.
        virtual ~Variable();
		std::string generateCode() const;
        
        Variable& operator=(const Variable&);
        
        // overriden print method from Symbol. Prints name and type's name.
        virtual void print(std::ostream&) const;
        
    protected: 
        virtual void clone(const Variable&);
        Type *type; // The type associated with this variable.
};

#endif
