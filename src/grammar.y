%{
/******************************************************************************
 * File: grammar.y
 * Authors: Evelina Arthursson, Derek McLean, Hugo Ribeiro
 *      *original content: Carol Zander
 * This is a grammar file that defines the grammar rules for a Pascal-like
 * language.
 * 10/16 - It currently only displays the text of unique identifiers (not
 *         keywords or symbols).
 */

/* declarations section */
#include "stack.h"
#include "Procedure.h"

#include <stdio.h>
#include <iostream>
#include <vector>
#include <deque>

extern Stack scopeStack;
using namespace std;

deque<Parameter> parameterQueue;
deque<string> identQueue;

%}

%union {
    const char *text;
    Procedure *procedure;
    deque<Parameter> parameterQueue;
    deque<string> identQueue;
}

/* definition section */

%start  CompilationUnit
%type   <procedure> ProcedureHeading
%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydispose 
        ydiv ydivide ydo  ydot ydotdot ydownto yelse yend yequal yfalse
        yfor yfunction ygreater ygreaterequal         yif yin yleftbracket
        yleftparen yless ylessequal yminus ymod ymultiply ynew ynil ynot 
        ynotequal ynumber yof  yor yplus yprocedure yprogram yread yreadln  
        yrecord yrepeat yrightbracket yrightparen  ysemicoalon yset 
        ythen  yto ytrue ytype  yuntil  yvar ywhile ywrite ywriteln yunknown
%token <text> yident ystring

%%
/* rules section */

/**************************  Pascal program **********************************/

CompilationUnit    :  ProgramModule
                   ;
ProgramModule      :  yprogram yident
                                {
                                    printf("%s ", $2);
                                }
                      ProgramParameters ysemicolon
                                { 
                                    /* Enter Program Scope */
                                    scopeStack.createScope(string($2));
                                    /* TODO : Put Program parameters on stack?? */
                                }
                      Block ydot
                   ;
ProgramParameters  :  yleftparen  IdentList  yrightparen
                   ;
IdentList          :  yident 
                                {
                                    printf("%s ", $1);
                                    identQueue.push_back($1);
                                }
                   |  IdentList ycomma yident
                                {
                                    printf("%s ", $3);
                                    identQueue.push_back($1);
                                }
                   ;

/**************************  Declarations section ***************************/

Block              :  Declarations  ybegin  StatementSequence  yend
                   ;
Declarations       :  ConstantDefBlock
                      TypeDefBlock
                      VariableDeclBlock
                      SubprogDeclList
                   ;
ConstantDefBlock   :  yconst ConstDefList
           |  /*** empty ***/
                   ;
ConstDefList       :  ConstantDef ysemicolon
                   |  ConstDefList ConstantDef ysemicolon
                   ;
TypeDefBlock       :  /*** empty ***/
                   |  ytype  TypeDefList
                   ;
TypeDefList        :  TypeDef  ysemicolon
                   |  TypeDefList TypeDef ysemicolon
                   ;
VariableDeclBlock  :  yvar VariableDeclList
           |  /*** empty ***/
                   ;
VariableDeclList   :  VariableDecl ysemicolon
           |  VariableDeclList VariableDecl ysemicolon
                   ;  
ConstantDef        :  yident
                                {
                                    printf("%s ", $1);
                                }
                      yequal  ConstExpression
                   ;
TypeDef            :  yident
                                {
                                    printf("%s ", $1);
                                }
                      yequal  Type
                   ;
VariableDecl       :  IdentList  ycolon  Type
                   ;

/***************************  Const/Type Stuff  ******************************/

ConstExpression    :  UnaryOperator ConstFactor
           |  ConstFactor
                   |  ystring
           |  ynil
                   ;
ConstFactor        :  yident    {
                                    printf("%s ", $1);
                                }
                   |  ynumber
                   |  ytrue
                   |  yfalse
                   ;
Type               :  yident    {
                                    printf("%s ", $1);
                                }
                   |  ArrayType
                   |  PointerType
                   |  RecordType
                   |  SetType
                   ;
ArrayType          :  yarray yleftbracket Subrange SubrangeList
                      yrightbracket  yof Type
                   ;
SubrangeList       :  /*** empty ***/
                   |  SubrangeList ycomma Subrange 
                   ;
Subrange           :  ConstFactor ydotdot ConstFactor
                   |  ystring ydotdot  ystring
                   ;
RecordType         :  yrecord  FieldListSequence  yend
                   ;
SetType            :  yset  yof  Subrange
                   ;
PointerType        :  ycaret  yident
                                {
                                    printf("%s ", $2);
                                }
                   ;
FieldListSequence  :  FieldList  
                   |  FieldListSequence  ysemicolon  FieldList
                   ;
FieldList          :  IdentList  ycolon  Type
                   ;

/***************************  Statements  ************************************/

StatementSequence  :  Statement  
                   |  StatementSequence  ysemicolon  Statement
                   ;
Statement          :  Assignment
                   |  ProcedureCall
                      {
                          //Handle procedue calls
                      }
                   |  IfStatement
                   |  CaseStatement
                   |  WhileStatement
                   |  RepeatStatement
                   |  ForStatement
                   |  IOStatement
                   |  MemoryStatement
                   |  ybegin StatementSequence yend
                   |  /*** empty ***/
                   ;
Assignment         :  Designator yassign Expression
                   ;
ProcedureCall      :  yident 
                                {
                                    printf("%s ", $1);
                                }
                   |  yident 
                                {
                                    printf("%s ", $1);
                                }
                      ActualParameters
                   ;
IfStatement        :  yif  Expression  ythen  Statement EndIf
           |  yif  Expression  ythen  Statement yelse Statement  EndIf
                   ;  
EndIf          :  /*** empty ***/
           ;
CaseStatement      :  ycase  Expression  yof  CaseList  yend
                   ;
CaseList           :  Case
                   |  CaseList  ysemicolon  Case
                   ;
Case               :  CaseLabelList  ycolon  Statement
                   ;
CaseLabelList      :  ConstExpression
                   |  CaseLabelList  ycomma  ConstExpression
                   ;
WhileStatement     :  ywhile  Expression  ydo  Statement
                   ;
RepeatStatement    :  yrepeat  StatementSequence  yuntil  Expression
                   ;
ForStatement       :  yfor  yident  
                                {
                                    printf("%s ", $2);
                                }
                            yassign  Expression  WhichWay  Expression
                            ydo  Statement
                   ;
WhichWay           :  yto  |  ydownto
                   ;
IOStatement        :  yread  yleftparen  DesignatorList  yrightparen
                   |  yreadln  
                   |  yreadln  yleftparen DesignatorList  yrightparen
                   |  ywrite  yleftparen  ExpList  yrightparen
                   |  ywriteln  
                   |  ywriteln  yleftparen  ExpList  yrightparen
                   ;

/***************************  Designator Stuff  ******************************/

DesignatorList     :  Designator  
                   |  DesignatorList  ycomma  Designator
                   ;
Designator         :  yident  
                                {
                                   printf("%s ", $1);
                                }
                      DesignatorStuff
                   ;
DesignatorStuff    :  /*** empty ***/
                   |  DesignatorStuff  theDesignatorStuff
                   ;
theDesignatorStuff :  ydot yident
                                {
                                    printf("%s ", $2);
                                } 
                   |  yleftbracket ExpList yrightbracket
                   |  ycaret
                   ;
ActualParameters   :  yleftparen  ExpList  yrightparen
                   ;
ExpList            :  Expression
                   |  ExpList  ycomma  Expression
                   ;
MemoryStatement    :  ynew  yleftparen  yident
                                {
                                    printf("%s ", $3);
                                }
                      yrightparen
                   |  ydispose yleftparen  yident
                                {
                                    printf("%s ", $3);
                                }
                      yrightparen
                   ;

/***************************  Expression Stuff  ******************************/

Expression         :  SimpleExpression
                   |  SimpleExpression  Relation  SimpleExpression
                   ;
SimpleExpression   :  TermExpr
                   |  UnaryOperator  TermExpr
                   ;
TermExpr           :  Term  
                   |  TermExpr  AddOperator  Term
                   ;
Term               :  Factor  
                   |  Term  MultOperator  Factor
                   ;
Factor             :  ynumber
                   |  ytrue
                   |  yfalse
                   |  ynil
                   |  ystring
                   |  Designator
                   |  yleftparen  Expression  yrightparen
                   |  ynot Factor
                   |  Setvalue
                   |  FunctionCall
                   ;
/*  Functions with no parameters have no parens, but you don't need         */
/*  to handle that in FunctionCall because it is handled by Designator.     */
/*  A FunctionCall has at least one parameter in parens, more are           */
/*  separated with commas.                                                  */
FunctionCall       :  yident 
                                {
                                    printf("%s ", $1);
                                }
                      ActualParameters
                   ;
Setvalue           :  yleftbracket ElementList  yrightbracket
                   |  yleftbracket yrightbracket
                   ;
ElementList        :  Element  
                   |  ElementList  ycomma  Element
                   ;
Element            :  ConstExpression  
                   |  ConstExpression  ydotdot  ConstExpression
                   ;

/***************************  Subprogram Stuff  ******************************/

SubprogDeclList    :  /*** empty ***/
                   |  SubprogDeclList ProcedureDecl ysemicolon
                   |  SubprogDeclList FunctionDecl ysemicolon
                   ;
ProcedureDecl      :  ProcedureHeading  ysemicolon
                            {
                                /* TODO: put in actions.cpp */
                                /* Put procedure in parent scope */
                                scopeStack.current->add(&$1);
                                /* Enter Procedure Scope */
                                scopeStack.createScope($1.name);
                                /* Put procedure params on symbol stack. */
                                std::vector<Procedure> toPutOnStack = $1.getParameters();
                                for (int i = 0; i < toPutOnStack.size(); i++) {
                                    current->add(toPutOnStack[i]);
                                }
                            }
                      Block
                            {
                                /* TODO: put in actions.cpp */
                                /* Exit Procedure scope */
                                StackFrame *scope = stackScope.leaveScope();
                                /* Print exited scope. */
                                cout << scope;
                                /* Mem management */
                                delete scope;
                                scope = NULL;
                            }
                   ;
FunctionDecl       :  FunctionHeading  ycolon  yident
                                {
                                    printf("%s ", $3);
                                }
                      ysemicolon  Block
                   ;
ProcedureHeading   :  yprocedure yident {
                                {
                                    printf("%s ", $2);
                                    /* create procedure */
                                    Procedure procedure(string($2));
                                    /* Pass procedure back */
                                    $$ = procedure;
                                }
                   |  yprocedure yident {
                                {
                                    printf("%s ", $2);
                                }
                      FormalParameters
                                {
                                    /* Create procedure */
                                    Procedure procedure(string($2)); // NOTE May need to dynamically create?
                                    /* Add parameters */
                                    while (!procedureQueue.empty()) { // yacc error ::  yacc: e - line 374 of "grammar.y", $4 is untyped
                                        procedure.addParameter(procedureQueue.front());
                                        procedureQueue.pop_front();
                                    }
                                }
                   ;
FunctionHeading    :  yfunction  yident
                                {
                                    printf("%s ", $2);
                                }
                   |  yfunction  yident
                                {
                                    printf("%s ", $2);
                                }
                      FormalParameters
                   ;
FormalParameters   :  yleftparen FormalParamList yrightparen
                                {
                                    $$ = $2; /* TODO Play with removing */
                                }
                   ;
FormalParamList    :  OneFormalParam 
                                {
                                    //$$.push_back($1);
                                }
                   |  FormalParamList ysemicolon OneFormalParam
                                {
                                    //$$.push_back($3);
                                }
                   ;
OneFormalParam     :  yvar  IdentList  ycolon  yident
                                {
                                    printf("%s ", $4);
                                    /* Search Symbol Table for Type corresponding to yident. */
                                    Type *type = NULL; // = foundType;
                                    bool isFound = scopeStack.searchStack(yident, type);
                                    /* Create parameters and add to parameter queue */
                                    if (isFound)
                                    {
                                        while (!identQueue.empty()) {
                                            Parameter param(identQueue.front(), type, true);
                                            parameterQueue.push_back(param);
                                            identQueue.pop_front();
                                        }
                                    }
                                }
                   |  IdentList  ycolon  yident
                                {
                                    printf("%s ", $3);
                                    /* TODO Search Symbol Table for Type corresponding to yident. */
                                    Type *type = NULL; // = foundType;
                                
                                    /* Create parameters and add to parameter queue */
                                    while (!identQueue.empty()) {
                                        Parameter param(identQueue.front(), type, false);
                                        parameterQueue.push_back(param);
                                        identQueue.pop_front();
                                    }
                                }
                   ;

/***************************  More Operators  ********************************/

UnaryOperator      :  yplus | yminus
                   ;
MultOperator       :  ymultiply | ydivide | ydiv | ymod | yand
                   ;
AddOperator        :  yplus | yminus | yor
                   ;
Relation           :  yequal  | ynotequal | yless | ygreater
                   |  ylessequal | ygreaterequal | yin
                   ;

%%

/* program section */

void yyerror(const char *s) {
   cout << s << endl;
}

extern int yylex();

