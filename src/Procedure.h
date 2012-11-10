#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "symbol.h"
#include "Parameter.h"

#include<vector>

class Procedure : public Symbol {

    public:
        Procedure(string);
        bool addField(const Parameter&);

    private:
        std::vector<Parameter> fields;

};

#endif
