#include "PointerType.h"
#include "TypeSymbol.h"
#include "symbolictype.h"
#include "stdtype.h"
#include "PointerType.h"
#include "RecordType.h"
#include <sstream>
using namespace std;
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
    pointer = true;
}

Symbol* PointerType::getPointee() const
{
    return pointee;
}

void PointerType::setPointee(Symbol *pointee)
{
    delete this->pointee;
    this->pointee = NULL;
    this->pointee = pointee;
}
std::string PointerType::generateOptionalCode() const{
    return pointee->getName();
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
    //delete pointee;
    pointee = NULL;
}

/*******************************************************************************
 *
 * Overloaded print method from Symbol.
 *
 * Prints name of the pointer and name of what the pointer is pointing to.
 */
void PointerType::print(std::ostream &output) const
{
    output << "^ " << pointee->name;
}

Type* PointerType::clone() const
{
    PointerType *clone = new PointerType(pointee);
    return clone;
}

std::string PointerType::generateTypeCode() const
{
    std::string tempString = "*";
    return tempString;
}

std::string PointerType::generateVarDeclCode() const
{
	TypeSymbol* t = dynamic_cast<TypeSymbol*>(pointee);
	std::string tempString = "";
	if (t != NULL) {
		StdType* test = dynamic_cast<StdType*>(t->getMyType());	

		//RecordType* test3 = dynamic_cast<RecordType*>(t->getMyType());
		if (test != NULL) {
			std::stringstream ss;		
			ss << t->generateCode() << "*";
			tempString = ss.str();
		}
		else {
			std::stringstream ss;
			ss << t->getName() << "*";
			tempString = ss.str();
			
		}
	}
    return tempString;
}
