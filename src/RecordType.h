#ifndef RECORDTYPE_H
#define RECORDTYPE_H

#include "Type.h"
#include "Variable.h"

#include <iostream>
#include <vector>

/**
 * Represents an identifier for type that is a record.
 * A record has a name and a set of fields. Additionally, a record is assigned
 * a pseudo-scope that is used for printing. The list of fields for a record may
 * be empty.
 */
class RecordType : public Type {

    public:
        // ctr that creates a record with a name and a given pseudo-scope
        RecordType(int);
        ~RecordType();
        Type* clone() const;
        // Add a new field to the current record.
        bool addField(Variable*&);
        // Checks if field with given name is in the current record.
        bool hasField(const std::string&) const;
		std::string generateOptionalCode() const {return "struct";};
		std::string printCName() const {return "";};
    protected:
        // overriddent print method from Type. Prints name and fields.
        void print(std::ostream&) const;

    private:
        // The set of fields within a record.
        std::vector<Variable*> fields;
        // The pseudo-scope level used for printing purposes.
        int scope;
        // Helper method for #print, prints tabs indicated by scope.
        void printScopeTabs(std::ostream&) const;
        std::string generateTypeCode() const;
		std::string generateVarDeclCode() const;
};

#endif
