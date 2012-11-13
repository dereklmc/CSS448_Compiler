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
#include "Procedure.h"
#include "actions.h"
#include "parser.h"
#include "Variable.h"

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

%}

/* definition section */

%start  CompilationUnit
%type   <procedure> ProcedureHeading
%type   <function>  FunctionHeading
%type   <type> Type
%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydispose 
        ydiv ydivide ydo  ydot ydotdot ydownto yelse yend yequal yfalse
        yfor yfunction ygreater ygreaterequal         yif yin yleftbracket
        yleftparen yless ylessequal yminus ymod ymultiply ynew ynil ynot 
        ynotequal ynumber yof  yor yplus yprocedure yprogram yread yreadln  
        yrecord yrepeat yrightbracket yrightparen  ysemicolon yset 
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
                                    //std::string ident($2);
                                    //scopeStack.createScope(ident);
                                    //validBlock = true;
                                    /* TODO : Put Program parameters on stack?? */
                                }
                      Block ydot
                   ;
ProgramParameters  :  yleftparen  IdentList  yrightparen
                   ;
IdentList          :  yident 
                                {
                                    printf("%s ", $1);
                                    //identQueue.push_back($1);
                                }
                   |  IdentList ycomma yident
                                {
                                    printf("%s ", $3);
                                    //identQueue.push_back($3);
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
                                {
                                    /* TODO Search Symbol Table for Type corresponding to yident. */
                                    /* If Type was  */
                                    //Type *type = NULL; // = foundType;
                                    //bool isFound = scopeStack.searchStack(yident, type);
                                    /* Create parameters and add to parameter queue 
                                    if ($3 != NULL)
                                    {
                                        while (!identQueue.empty()) {
                                            Variable var(identQueue.front(), $3);
                                            // TODO Add symbol
                                            identQueue.pop_front();
                                        }
                                    }*/
                                }
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
                                    /*printf("%s ", $1);
                                    Symbol *typeSymbol = NULL;
                                    bool isFound = scopeStack.searchStack(std::string($1), typeSymbol);
                                    Type *type = dynamic_cast<Type*>(typeSymbol);
                                    if (isFound) {
                                        $$ = type;
                                    } else {
                                        $$ = type;
                                    }*/
                                }
                   |  ArrayType
                                {
                                    $$ = NULL;
                                }
                   |  PointerType
                                {
                                    $$ = NULL;
                                }
                   |  RecordType
                                {
                                    $$ = NULL;
                                }
                   |  SetType
                                {
                                    $$ = NULL;
                                }
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
                                createProcedureDecl($1);
                            }
                      Block
                            {
                                exitScope();
                            } 
                   ;
FunctionDecl       :  FunctionHeading  ycolon  yident
                            {
                                createFunctionDecl($3, $1);
                            }
                      ysemicolon  Block
                            {
                                exitScope();
                            }
                   ;
ProcedureHeading   :  yprocedure yident
                                {
                                    Procedure *procedure = new Procedure(std::string($2)); /* Check if name already taken */
                                    $$ = procedure; /* Pass procedure back */
                                }
                   |  yprocedure yident
                                {
                                    printf("%s ", $2);
                                }
                      FormalParameters
                                {
                                    /* Create procedure 
                                    Procedure *procedure = new Procedure(std::string($2)); // NOTE May need to dynamically create?
                                    // Add parameters 
                                    while (!parameterQueue.empty()) { // yacc error ::  yacc: e - line 374 of "grammar.y", $4 is untyped
                                        procedure->addParameter(parameterQueue.front());
                                        parameterQueue.pop_front();
                                    }
                                    $$ = procedure;
                                    */
                                }
                   ;
FunctionHeading    :  yfunction  yident
                                {
                                    createFunction($2, $$);
                                }
                   |  yfunction  yident
                                {
                                    printf("%s ", $2);
                                }
                      FormalParameters
                                {
                                    createFunctionWithParams($2, $$);
                                }
                   ;
FormalParameters   :  yleftparen FormalParamList yrightparen
                   ;
FormalParamList    :  OneFormalParam
                   |  FormalParamList ysemicolon OneFormalParam
                   ;
OneFormalParam     :  yvar IdentList ycolon yident
                                {
                                    createParameter($4);
                                    
                                }
                   |  IdentList ycolon yident
                                {
                                   createParameter($3);
                                    
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

