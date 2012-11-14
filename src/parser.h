#ifndef PARSER_H
#define PARSER_H

#include "Procedure.h"
#include "function.h"
#include "Type.h"
#include "constvalue.h"
#include "PointerType.h"

typedef union {
    const char *text;
    Procedure *procedure;
    Function *function;
    Type *type;
    ConstValue *constvalue;
    UnaryOperator unaryop;
} YYSTYPE;

#endif
