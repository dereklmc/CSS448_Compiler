#include"Parameter.h"

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

Parameter::~Parameter()
{
    std::cout << "IN PARAM DESTRUCTOR" << std::endl;
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
    Variable::print(output);
}
