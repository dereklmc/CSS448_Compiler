#ifndef PROCEDURE_H
#define PROCEDURE_H

#include"symbol.h"

#include<vector>

class Procedure : public Symbol {

    public:
        Procedure(string);
        bool addField(const Parameter&);

    private:
        vector<Parameter> fields;

};

#endif
