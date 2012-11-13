#ifndef CONST_VALUE
#define CONST_VALUE

#include <string>

enum ConstValueType
{
    SYMBOL,
    NUMBER,
    BOOLEAN,
    STRING,
    NIL
};

enum UnaryOperator {
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
        ConstValue(std::string value, ConstValueType type) {
            this->value = value;
            this->type = type;
            this->op = NONE;
        }
        
        void setOperator(UnaryOperator op) {
            this->op = op;
        }
    
    private:
        std::string value;
        ConstValueType type;
        UnaryOperator op;

};

#endif
