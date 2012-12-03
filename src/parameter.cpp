#include "parameter.h"

/*******************************************************************************
 *
 * Constructor
 *
 * Creates a parameter with a name and a type that is either passed by reference
 * or by value. Extends the constructor in Variable.
 */
Parameter::Parameter(std::string name, Type *&type, bool byReference) :
        Variable(name, type)
{
    this->byReference = byReference;
}

Variable* Parameter::getVariable() const
{
    return new Variable(name, type->clone());
}

bool Parameter::operator==(const Parameter &other) const
{
    return other.name == name;
}

std::string Parameter::generateCode() const
{
    return (type->generateVarDeclCode() + " " + (byReference ? "&" : "") + " " + name);
}

/*******************************************************************************
 *
 * Overloaded print from Variable.
 * Extends Variable::print to also print whether the parameter is passed by
 * reference or by value.
 */
void Parameter::print(std::ostream &output) const
{
    if (byReference) {
        output << "var ";
    }
    var.print(output);
}
