#include"Parameter.h"

Parameter::Parameter(string name, Type *type, bool byReference) :
        Variable(name, type)
{
    this->byReference = byReference;
}

void Parameter::print(std::ostream &output) const
{
    if (byReference) {
        output << "var ";
    }
    Variable::print(output);
}
