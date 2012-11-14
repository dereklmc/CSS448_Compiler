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
#include "actions.h"
#include "parser.h"

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

%}

/* definition section */

%start  CompilationUnit
%type   <procedure> ProcedureHeading
%type   <function>  FunctionHeading
%type   <type> Type ArrayType SetType RecordType
%type   <constvalue> ConstFactor ConstExpression
%type   <unaryop> UnaryOperator
%type   <pointertype> PointerType
%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydispose 
        ydiv ydivide ydo  ydot ydotdot ydownto yelse yend yequal yfalse
        yfor yfunction ygreater ygreaterequal         yif yin yleftbracket
        yleftparen yless ylessequal yminus ymod ymultiply ynew ynil ynot 
        ynotequal ynumber yof  yor yplus yprocedure yprogram yread yreadln  
        yrecord yrepeat yrightbracket yrightparen  ysemicolon yset 
        ythen  yto ytrue ytype  yuntil  yvar ywhile ywrite ywriteln yunknown
%token <text> yident ystring ynumber

%%
/* rules section */

/**************************  Pascal program **********************************/

CompilationUnit    :  ProgramModule
                   ;
ProgramModule      :  yprogram yident ProgramParameters ysemicolon
                                {
                                    createProgramScope($2);
                                }
                      Block ydot
                                {
                                    exitScope();
                                }
                   ;
ProgramParameters  :  yleftparen  IdentList  yrightparen
                   ;
IdentList          :  yident 
                                {
                                    addIdent($1);
                                }
                   |  IdentList ycomma yident
                                {
                                    addIdent($3);
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
                                {
                                    //loop throught all pointers
                                    //add to symbol table
                                    checkPointers();
                                }
                   ;
TypeDefList        :  TypeDef  ysemicolon
                   |  TypeDefList TypeDef ysemicolon
                   ;
VariableDeclBlock  :  yvar VariableDeclList
                                {
                                    
                                }
           |  /*** empty ***/
                   ;
VariableDeclList   :  VariableDecl ysemicolon        
           |  VariableDeclList VariableDecl ysemicolon
                   ;  
ConstantDef        :  yident yequal ConstExpression
                                {
                                    createConstant($1, $3);
                                }
                   ;
TypeDef            :  yident yequal  Type
                                {
                                    createTypeSymbol($1, $3);
                                }
                   ;
VariableDecl       :  IdentList  ycolon  Type
                                {
                                    checkPointers();
                                    createVariableList($3);
                                }
                   ;

/***************************  Const/Type Stuff  ******************************/

ConstExpression    :  UnaryOperator ConstFactor
                                {
                                    $2->setOperator($1);
                                    $$ = $2;
                                }
                   |  ConstFactor
                   |  ystring
                                {
                                    createConstStringValue($$, $1);
                                }
                   |  ynil
                                {
                                    createConstNilValue($$);
                                }
                   ;
ConstFactor        :  yident
                                {
                                    createConstSymbolValue($$, $1);
                                }
                   |  ynumber
                                {
                                    createConstNumberValue($$, $1);
                                }
                   |  ytrue
                                {
                                    createConstBoolValue($$, "true");
                                }
                   |  yfalse
                                {
                                    createConstBoolValue($$, "false");
                                }
                   ;
Type               :  yident    {
                                    getTypeOfSymbol($1, $$);
                                }
                   |  ArrayType

                   |  PointerType
                                {
                                    createPointer($1);
                                    $$ = NULL;
                                }
                   |  RecordType
                   |  SetType
                   ;
ArrayType          :  yarray yleftbracket Subrange SubrangeList
                      yrightbracket yof Type
                                {
                                    createArrayType($$, $7);
                                }
                   ;
SubrangeList       :  /*** empty ***/
                   |  SubrangeList ycomma Subrange 
                   ;
Subrange           :  ConstFactor ydotdot ConstFactor
                                {
                                    createConstRange($1, $3);
                                }
                   |  ystring ydotdot  ystring
                                {
                                    createStringRange($1, $3);
                                }
                   ;
RecordType         :  yrecord  FieldListSequence  yend
                                {
                                    createRecordType($$);
                                }
                   ;
SetType            :  yset  yof  Subrange
                                {
                                    createSetType($$);
                                }
                   ;
PointerType        :  ycaret  yident
                                {
                                    printf("%s ", $2);
                                    //Gives symbol to pointer
                                    Symbol* symbol = new Symbol($2);
                                    PointerType* ptr = new PointerType(symbol);
                                    $$ = ptr;
                                }
                   ;
FieldListSequence  :  FieldList  
                   |  FieldListSequence  ysemicolon  FieldList
                   ;
FieldList          :  IdentList  ycolon  Type
                                {
                                    createVariableList($3);
                                }
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
                                    createProcedure($2, $$);
                                }
                   |  yprocedure yident FormalParameters
                                {
                                    createProcedureWithParams($2, $$);
                                }
                   ;
FunctionHeading    :  yfunction  yident
                                {
                                    createFunction($2, $$);
                                }
                   |  yfunction  yident FormalParameters
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

UnaryOperator      :  yplus
                                {
                                    $$ = PLUS;
                                }
                   |  yminus
                                {
                                    $$ = MINUS;
                                }
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

