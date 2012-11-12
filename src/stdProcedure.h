#ifndef STDPROCEDURE_H
#define STDPROCEDURE_H
#include "Procedure.h"

class StdProcedure : public Procedure
{
    friend std::ostream& operator<<(std::ostream& out, const StdProcedure p)
    {
        p.print(out);
    }
    
    public:
        static const StdProcedure WRITE;
        static const StdProcedure WRITELN;
        static const StdProcedure READ;
        static const StdProcedure READLN;
        static const StdProcedure DISPOSE;
    
    private:
        StdProcedure(string name) : Procedure(name) {};

        void print(std::ostream& out)
        {
            Procedure::print(out);
        }
};

StdProcedure::WRITE = StdProcedure("write");
StdProcedure::WRITELN = StdProcedure("writeln");
StdProcedure::READ = StdProcedure("read");
StdProcedure::READLN = StdProcedure("readln");
StdProcedure::DISPOSE = StdProcedure("dispose");

#endif