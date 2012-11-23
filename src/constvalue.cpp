#include "constvalue.h"

ConstValue::ConstValue(std::string value, ConstValueType type)
{
    this->value = value;
    this->type = type;
    this->op = NONE;
}

void ConstValue::setOperator(UnaryOperator op)
{
    this->op = op;
}

std::string ConstValue::generateCode() const
{
    return value;
}
