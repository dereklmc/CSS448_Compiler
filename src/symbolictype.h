#ifndef SYMBOLICTYPE_H
#define SYMBOLICTYPE_H

#include "Type.h"
#include "TypeSymbol.h"

class SymbolicType : public Type {

    public:
        SymbolicType(TypeSymbol *);
        ~SymbolicType();
        
        Type* clone() const;
        std::string generateTypeCode() const;
		std::string generateVarDeclCode() const;
		std::string generateOptionalCode() const {return "";}
		std::string printCName() const {return "";}
		bool equals(Type*) const;
		TypeSymbol* getSymbol() const;
    
    protected:
        // overriddent print method from Type.
        void print(std::ostream&) const;

    private:
        TypeSymbol *symbol;

};

#endif
