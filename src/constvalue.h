#ifndef CONST_VALUE
#define CONST_VALUE

#include <string>
#include <iostream>

enum ConstValueType
{
    SYMBOL,
    INTEGER,
    REAL,
    BOOLEAN,
    STRING,
    CHAR,
    NIL
};

enum UnaryOperator
{
    PLUS,
    MINUS,
    NONE
};

class ConstValue
{
    friend std::ostream& operator<<(std::ostream& out, const ConstValue &cvalue)
    {
        out << cvalue.value;
        return out;
    }

    public:
        ConstValue(std::string value, ConstValueType type);
        void setOperator(UnaryOperator op);
        std::string generateCode() const;
        ConstValueType getType() const;
    
    private:
        std::string value;
        ConstValueType type;
        UnaryOperator op;

};

#endif
