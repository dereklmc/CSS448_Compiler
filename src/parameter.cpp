#include "parameter.h"
#include "symbolictype.h"

#include <sstream>

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

/*******************************************************************************
 *
 * Constructor
 */
Parameter::Parameter(const Parameter &other) :
        Variable(other.name, other.type->clone())
{
    this->byReference = other.byReference;
}

bool Parameter::operator==(const Parameter &other) const
{
    return other.name == name;
}

std::string Parameter::generateFunctorParam() const
{
    std::stringstream ss;
    ss << type->generateVarDeclCode() << " ";
    if (byReference) {
        ss << "& ";
    }
    ss << name;
    return ss.str();
}

std::string Parameter::getDesignator() const
{
    std::stringstream ss;
    if (byReference) {
        ss << "(*";
    }
    ss << name;
    if (byReference) {
        ss << ")";
    }
    return ss.str();
}

std::string Parameter::generateDeclaration() const
{
    std::stringstream ss;
    ss << type->generateVarDeclCode() << " ";
    if (byReference) {
        ss << "* ";
    }
    ss << name;
    return ss.str();
}

std::string Parameter::generateInit() const
{
    std::stringstream ss;
    ss << name << " = ";
    if (byReference) {
        ss << "&";
    }
    ss << name;
    return ss.str();
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
    output << name;
}
