#ifndef FUNCTION_H
#define FUNCTION_H

#include "Procedure.h"
#include "Type.h"

class Function : public Procedure
{
    public:
        Function(std::string s) : Procedure(s){};
        void setType(Type *t)    {returnType = t;}
    private:
        Type *returnType;
};

#endif
