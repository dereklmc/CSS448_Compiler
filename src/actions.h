#ifndef ACTIONS_H
#define ACTIONS_H

#include "y.tab.h"
#include "Procedure.h"
#include "function.h"
#include "Type.h"
#include "stack.h"

#include <deque>

extern Stack symbolTable;

template <class T>
bool searchStack(const char*, T*&);

void createParameter(const char*);

void createFunction(const char*, Function*&);

void createFunctionWithParams(const char*, Function*&);

void createFunctionDecl(const char*, Function*&);

void createProcedureDecl(Procedure*);

void exitScope();

#endif
