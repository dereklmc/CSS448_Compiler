#ifndef PARAMETER_H
#define PARAMETER_H

#include "variable.h"

/**
 * Represents an identifier for a parameter of a procedure or function.
 * A parameter is a variable because it has a name and a type.
 * A parameter can also be passed by reference or by value.
 */
class Parameter : public Variable
{

    public:
        // ctr, creates a parameter with a name and a type that is passed by ref
        // or by value.
        Parameter(std::string, Type *&, bool);
        Parameter(const Parameter&);
        
        bool operator==(const Parameter&) const;
        std::string getDesignator() const;
        //std::string generateCode() const;
        std::string generateDeclaration() const;
        std::string generateFunctorParam() const;
        std::string generateInit() const;

        // records how the parameter is passed:
        // true if by reference
        // false if by value.
        bool byReference;
    private:
        // Overloaded print from Symbol.
        void print(std::ostream&) const;

};

#endif
