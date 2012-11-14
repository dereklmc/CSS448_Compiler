#include "PointerType.h"

/*******************************************************************************
 *
 * Constructor
 *
 * Creates a pointer with a name that points to a given ident. Extends the 
 * constructor in Symbol.
 */
PointerType::PointerType(Symbol *pointee) : Type()
{
    this->pointee = pointee;
}

Symbol* PointerType::getPointee() const
{
    return pointee;
}
/*******************************************************************************
 *
 * Destructor.
 *
 * Releases reference to the pointee without deallocating type. Because the 
 * symbol table stores pointee, the symbol table is responsible for deallocating
 * pointee.
 */
PointerType::~PointerType()
{
    this->pointee = NULL;
}

/*******************************************************************************
 *
 * Overloaded print method from Symbol.
 *
 * Prints name of the pointer and name of what the pointer is pointing to.
 */
void PointerType::print(std::ostream &output) const
{
    output << " ^ " << pointee->name;
}

