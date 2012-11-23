#ifndef FUNCTION_H
#define FUNCTION_H

#include "procedure.h"
#include "Type.h"

class Function : public Procedure
{
    public:
        Function(std::string s) : Procedure(s){};
        ~Function()
        {
            delete returnType;
            returnType = NULL;
        }
        void setType(Type *t)    {returnType = t;}
    private:
        Type *returnType;
};

#endif
