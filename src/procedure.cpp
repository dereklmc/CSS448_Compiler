#include"Procedure.h"

Procedure::Procedure(string name) :
        Symbol(name)
{
}

bool Procedure::addField(const Parameter &param)
{
    fields.push_back(param);
}
