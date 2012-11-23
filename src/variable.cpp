#include "Variable.h"

Variable::Variable()
{
    type = NULL;
}

/*******************************************************************************
 *
 * Constructor
 *
 * Creates a variable with a name and a type. Extends the constructor in Symbol.
 */
Variable::Variable(std::string name, Type *type) :
        Symbol(name)
{
    this->type = type;
}

Variable::Variable(const Variable &other)
{
    this->type = NULL;
    this->clone(other);
}

/*******************************************************************************
 *
 * Destructor.
 *
 * Releases reference to type without deallocating type. Because the symbol
 * table stores type, the symbol table is responsible for deallocating type.
 */
Variable::~Variable()
{
    delete type;
    type = NULL;
}

Variable& Variable::operator=(const Variable &rhs)
{
    if (this != &rhs) {
        this->clone(rhs);
    }
    return *this;
}

void Variable::clone(const Variable &rhs)
{
    if (this->type != NULL) {
        delete this->type;
        this->type = NULL;
    }
    this->name = rhs.name;
    this->type = rhs.type->clone();
}

/*******************************************************************************
 *
 * Overloaded print method from Symbol.
 *
 * Prints name of the variable and name of the variable's type.
 */
void Variable::print(std::ostream &output) const
{
    Symbol::print(output);
    output << " " << *type;
}
