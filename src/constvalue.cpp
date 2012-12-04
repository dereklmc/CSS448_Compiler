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

ConstValueType ConstValue::getType() const
{
    return type;
    /*
    switch(type)
    {
        case BOOLEAN:
            return "BOOLEAN";
        case SYMBOL:
            return "SYMBOL";
        case INTEGER:
            return "INTEGER";
        case REAL:
            return "REAL";
        case STRING:
            return "STRING";
        case CHAR:
            return "CHAR";
        case NIL:
            return "NIL";
    }*/
}
