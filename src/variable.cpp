#include"Variable.h"

Variable::Variable(string name, Type *type) :
        Symbol(name)
{
    this->type = type;
}

Variable::~Variable()
{
    this->type = NULL;
}

void Variable::print(std::ostream &output) const
{
    Symbol::print(output);
    output << " " << *type;
}
