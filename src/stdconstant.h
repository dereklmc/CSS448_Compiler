#ifndef STDCONSTANT_H
#define STDCONSTANT_H
#include "constant.h"
using namespace std;
class StdConstant : public Constant<bool>
{
    public:
        friend std::ostream& operator<<(std::ostream& out, const StdConstant s)
        {
            s.print(out);
        }
	
        // Remember to use: const StdConstant StdConstant::TRUE("true", true)
        // to add to scope 0
        static const StdConstant TRUE;
        static const StdConstant FALSE;
        StdConstant(std::string name, bool value) : Constant(name, value) {};
    protected:
	    void print(std::ostream& out) const
	    {
		    Symbol::print(out);
	    };
    //private:
	    //stdConstant(std::string, bool);
};

#endif
