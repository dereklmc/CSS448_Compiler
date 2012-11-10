#ifndef PARAMETER_H
#define PARAMETER_H

#include "Variable.h"

class Parameter : public Variable {

    public:
        Parameter(string, Type*, bool);

    private:
        bool byReference;
        void print(std::ostream&) const;

};

#endif
