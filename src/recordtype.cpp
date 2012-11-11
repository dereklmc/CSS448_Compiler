#include "RecordType.h"

/*******************************************************************************
 *
 * Standard constructor.
 * Extends the constructor for Type to also take a parentScope. The parentScope
 * is the id of the scope in which the current record is stored. The record then
 * assigns itself a pseudo-scope within the parent scope. This pseudo-scope is
 * used for printing fields.
 */
RecordType::RecordType(string name, int parentScope) :
        Type(name)
{
    this->scope = parentScope + 1;
}

/*******************************************************************************
 *
 * Add a new field to the current record.
 */
bool RecordType::addField(const Variable &field)
{
    
    if (hasField(field.name)) {
        return false;
    }
    fields.push_back(field);
    return true;
}

/*******************************************************************************
 *
 * Add a new field to the current record.
 */
bool RecordType::hasField(const string &fieldName) const
{
    
    std::vector<Variable>::const_iterator it;
    for(it = fields.begin(); it != fields.end(); ++it) {
        if (it->name == fieldName) {
            return true;
        }
    }
    false;
}

/*******************************************************************************
 *
 * Overloaded print method from Symbol.
 *
 * Prints name of the record and all fields contained in the record.
 */
void RecordType::print(std::ostream &output) const
{
    Type::print(output);
    output << endl;
    
    std::vector<Variable>::const_iterator it;
    for(it = fields.begin(); it != fields.end(); ++it) {
        std::cout << *it << endl;
    }
}

/*******************************************************************************
 *
 * Helper method for print.
 * Prints to given output stream a number of tabs equal to the record's 
 * psuedo-scope.
 */
void RecordType::printScopeTabs(std::ostream &output) const
{    
    for(int i = 0; i < scope; i++) {
        std::cout << "\t";
    }
}
