#ifndef TYPE_H
#define TYPE_H

#include "symbol.h"

/**
 * Represents an arbitrary type in pascal. This is an
 * abstract base class for all types in pascal, including:
 * * Records
 * * Pointers
 * * Arrays
 * * Sets
 *
 * Currently, type is just a symbol with another name. Type may have specific
 * methods in later phases of the compiler.
 */
class Type 
{
    friend std::ostream& operator<<(std::ostream& out, const Type &t)
    {
        t.print(out);
        return out;
    }
    
    public:
        virtual ~Type()
        {
        }
        
        virtual Type* clone() const =0;
        virtual std::string generateTypeCode() const =0;
		virtual std::string generateVarDeclCode() const=0;
		virtual std::string generateOptionalCode() const=0;
		virtual std::string printCName() const=0;
		virtual bool equals(Type*) const { return false; }
        bool typeDefed;

    protected:
        virtual void print(std::ostream& out) const =0;
		std::string optionalCode;
                
};

#endif
