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

	friend std::ostream& operator<<(std::ostream& out, const Variable 
s)
	{
		s.print(out);
		return out;
	}

    public:
        // ctr, creates a variable with a name and a type.
        Variable(std::string, Type*&);
        // Defined to release Variable's reference to Type.
        // Since type is also in the symbol table, the ST will deallocate type.
        virtual ~Variable();
        // overriden print method from Symbol. Prints name and type's name.
        virtual void print(std::ostream&) const;
		
    private:
        Type *type; // The type associated with this variable.

};

#endif
