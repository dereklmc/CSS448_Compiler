#include "Variable.h"

/*******************************************************************************
 *
 * Constructor
 *
 * Creates a variable with a name and a type. Extends the constructor in Symbol.
 */
Variable::Variable(std::string name, Type *&type) :
        Symbol(name)
{
    this->type = type;
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
    if (type != NULL && !type->hasSymbol) {
        delete type;
    }
    type = NULL;
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
