#include "constant.h"

Constant::Constant(std::string name, ConstValue *value) : Symbol(name)
{
    this->value = value;
}

Constant::~Constant() {
    delete value;
    value = NULL;
}

std::string Constant::generateCode() const
{
    return "#define " + name + " " + value->generateCode();
}

void Constant::print(std::ostream& out) const
{
	Symbol::print(out);
	out << " " << *value;
}
