#include "constvalue.h"

#include <sstream>

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
    if(type == CHAR) {
        std::stringstream temp;
        temp << "'" << value[0] << "'";
        return temp.str();
    }
    return value;
}

ConstValueType ConstValue::getType() const
{
    return type;
}
