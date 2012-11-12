#ifndef STDPROCEDURE_H
#define STDPROCEDURE_H
#include "Procedure.h"

class StdProcedure : public Procedure
{
    private:
        StdProcedure WRITE;
        StdProcedure WRITELN;
        StdProcedure READ;
        StdProcedure READLN;
        StdProcedure DISPOSE;

        void print(std::ostream& out)
        {
            //TODO
        }
    public:
        friend std::ostream& operator<<(std::ostream& out, const StdProcedure p)
        {
            p.print(out);
        }
};

#endif
