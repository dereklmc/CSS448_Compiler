#ifndef PARSER_H
#define PARSER_H

#include "procedure.h"
#include "function.h"
#include "Type.h"
#include "constvalue.h"
#include "PointerType.h"
#include "arraytype.h"

typedef union {
    const char *text;
    Procedure *procedure;
    Function *function;
    Type *type;
    ConstValue *constvalue;
    UnaryOperator unaryop;
    ArrayType *arraytype;
    PointerType *pointertype;
    bool boolean;
} YYSTYPE;

extern YYSTYPE yylval;

#endif
