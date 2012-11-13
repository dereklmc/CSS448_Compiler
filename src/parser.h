#ifndef PARSER_H
#define PARSER_H

#include "Procedure.h"
#include "function.h"
#include "Type.h"

typedef union {
    const char *text;
    Procedure *procedure;
    Function *function;
    Type *type;
} YYSTYPE;

#endif
