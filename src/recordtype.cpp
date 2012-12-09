#include "RecordType.h"

/*******************************************************************************
 *
 * Standard constructor.
 * Extends the constructor for Type to also take a parentScope. The parentScope
 * is the id of the scope in which the current record is stored. The record then
 * assigns itself a pseudo-scope within the parent scope. This pseudo-scope is
 * used for printing fields.
 */
RecordType::RecordType(int parentScope) : Type()
{
    this->scope = parentScope + 2;
    AR = true;
	optionalCode = "struct";
}

RecordType::~RecordType()
{
    for (int i = 0; i < fields.size(); i++) {
        delete fields[i];
    }
    fields.clear();
}

Type* RecordType::clone() const
{
    RecordType *clone = new RecordType(scope);
    for (int i = 0; i < fields.size(); i++) {
        clone->fields.push_back(new Variable(*fields[i]));
    }
    return clone;
}

/*******************************************************************************
 *
 * Add a new field to the current record.
 */
bool RecordType::addField(Variable *&field)
{
    //std::cout << "RECORD-->ADD FIELD \"" << *field << "\"" << std::endl;
    if (hasField(field->name)) {
        return false;
    }
    fields.push_back(field);
    return true;
}

/*******************************************************************************
 *
 * Checks if the record has a field with a given name. Returns true if
 * it does, otherwise false.
 */
bool RecordType::hasField(const std::string &fieldName) const
{
    getField(fieldName) != NULL;
}

/*******************************************************************************
 *
 * Returns a field with a given name. If the field does not exist, returns null.
 */
Variable* RecordType::getField(const std::string &fieldName) const
{
    for(int i = 0; i < fields.size(); i++) {
        if (fields[i]->name == fieldName) {
            return fields[i];
        }
    }
    return NULL;
}

/*******************************************************************************
 *
 * Overloaded print method from Symbol.
 *
 * Prints name of the record and all fields contained in the record.
 */
void RecordType::print(std::ostream &output) const
{
    output << std::endl;
    for(int i = 0; i < fields.size(); i++) {
        printScopeTabs(output);
        fields[i]->print(output);
        output << std::endl;
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

std::string RecordType::generateVarDeclCode() const
{
	std::string tempString = "";
	return tempString;
}

std::string RecordType::generateTypeCode() const
{
    std::string tempString = "{\n";
	for (int i = 0; i < fields.size(); i++)
	{
		//TODO print tabs
		for(int j = 0; j < scope; j++) {
        		tempString += "    ";
    		}
		tempString += "    ";
		tempString += (fields[i]->generateCode() + ";\n");
	}
	for(int i = 1; i < scope - 1; i++) {
        	tempString += "    ";
    	}
	tempString += "}";
    return tempString;
}
