#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "symbol.h"
#include "Parameter.h"

#include<vector>

/**
 * Represents an identifier for a procedure in pascal.
 * A procedure has a name and a set of parameters.
 */
class Procedure : public Symbol {
	
	friend std::ostream& operator<<(std::ostream& out, const Procedure s)
    {
        s.print(out);
    }

    public:
        // ctr inherited from Symbol.
        Procedure(string);
        // Add a new parameter that must be given when calling the procedure.
        bool addParameter(const Parameter&);
	protected:
		void print(std::ostream& out) const;
        {
             Symbol::print(out);
        }

    private:
        // parameters required when calling the procedure.
        std::vector<Parameter> parameters;
};

#endif
