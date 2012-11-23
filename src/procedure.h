#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "symbol.h"
#include "parameter.h"

#include<vector>

/**
 * Represents an identifier for a procedure in pascal.
 * A procedure has a name and a set of parameters.
 */
class Procedure : public Symbol {

    public:
        // ctr inherited from Symbol.
        Procedure(std::string);
        virtual ~Procedure();
        // Add a new parameter that must be given when calling the procedure.
        bool addParameter(Parameter*);
        // Checks if param with given name is in the current procedure.
        bool hasParameter(Parameter*) const;
        std::vector<Parameter*> getParameters() const;
		virtual void print(std::ostream& out) const;

    private:
        // parameters required when calling the procedure.
        std::vector<Parameter*> parameters;
};

#endif
