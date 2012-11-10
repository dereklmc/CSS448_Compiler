#ifndef STDTYPE_H
#define STDTYPE_H

#include "symbol.h"

public StdType : public Symbol
{
public:
	static const StdType INTEGER;
	static const StdType BOOL;
	static const StdType REAL;
	static const StdType CHAR;

	StdType() { name = ""; }
	StdType( String name ) { this.name = name; }

private:

}

#endif
