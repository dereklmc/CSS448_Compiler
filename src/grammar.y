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

extern YYSTYPE yylval;

%}

/* definition section */

%start  CompilationUnit
%type   <procedure> ProcedureHeading
%type   <function>  FunctionHeading
%type   <type> Type SetType RecordType PointerType
%type   <constvalue> ConstFactor ConstExpression
%type   <unaryop> UnaryOperator
%type   <arraytype> ArrayType

%type   <type> Factor Term TermExpr SimpleExpression Expression Designator

%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydispose 
        ydiv ydivide ydo  ydot ydotdot ydownto yelse yend yequal yfalse
        yfor yfunction ygreater ygreaterequal         yif yin yleftbracket
        yleftparen yless ylessequal yminus ymod ymultiply ynew ynil ynot 
        ynotequal yof  yor yplus yprocedure yprogram yread yreadln  
        yrecord yrepeat yrightbracket yrightparen  ysemicolon yset 
        ythen  yto ytrue ytype  yuntil  yvar ywhile ywrite ywriteln yunknown
%token <text> yident ystring yinteger yreal

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

Block              :  Declarations
                      ybegin
                            {
                                std::cout << "void call() {" << std::endl;
                            }
                      StatementSequence
                      yend
                            {
                                std::cout << "}" << std::endl;
                            }
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
VariableDeclBlock  :  yvar
					 VariableDeclList
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
                                    createVariables($3);
                                }
                   ;

/***************************  Const/Type Stuff  ******************************/

ConstExpression    : UnaryOperator yident
                                {
									createConstValue($$, $2, SYMBOL);
                                    $$->setOperator($1);
                                }
				   |  UnaryOperator yinteger
								{
									createConstValue($$, $2, INTEGER);
                                    $$->setOperator($1);
								}
				   |  UnaryOperator yreal
								{
									createConstValue($$, $2, REAL);
                                    $$->setOperator($1);
								}
                   |  ConstFactor
                   |  ystring
                                {
                                    createConstValue($$, $1, STRING);
                                }
                   |  ynil
                                {
                                    createConstValue($$, "nil", NIL);
                                }
                   ;
ConstFactor        :  yident
                                {
                                    createConstValue($$, $1, SYMBOL);
                                }
                   |  yinteger
                                {
                                    createConstValue($$, $1, INTEGER);
                                }
                   |  yreal
                                {
                                    createConstValue($$, $1, REAL);
                                }
                   |  ytrue
                                {
                                    createConstValue($$, "true", BOOLEAN);
                                }
                   |  yfalse
                                {
                                    createConstValue($$, "false", BOOLEAN);
                                }
                   ;
Type               :  yident    {
									
                                    getSymbolicType($$, $1);
                                }
                   |  ArrayType
                                {
                                    $$ = dynamic_cast<Type*>($1);
                                }
                   |  PointerType
                   |  RecordType
					 			{
									$$ = dynamic_cast<Type*>($1);
								}
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
                                    createPointer($$, $2);
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
Assignment         :  Designator
                      yassign { std::cout << " = "; }
                      Expression
                      {
                      		std::cout << ";";
                      }
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
IfStatement        :  IfStatementBlock
                      EndIf
                   |  IfStatementBlock
                      yelse
                            {
                                std::cout << "} else {" << std::endl;	
                            }
                      Statement
                      EndIf
                   ;
IfStatementBlock   :  yif 
                            {
                                std::cout << "if (";	
                            }
					  Expression 
					        {
                                if (!BOOLEAN_TYPE->equals($3)) {
                                    // TODO, record error message
                                    std::cout << "ERROR: Expression is not conditional" << std::endl;
						        }
						        std::cout << ") {" << std::endl;	
					        }  
					  ythen Statement
                   ;
EndIf          	   :  /*** empty ***/
					        {
						        std::cout << std::endl << "}" << std::endl;	
					        }
           		   ;
CaseStatement      :  ycase  Expression  yof  CaseList  yend
                   ;
CaseList           :  Case
                   |  CaseList  ysemicolon  Case
                   ;
Case               :  CaseLabelList  ycolon  Statement
                   ;
CaseLabelList      :  ConstExpression
                                {
                                    /* TODO */
                                    delete $1;
                                    $1 = NULL;
                                }
                   |  CaseLabelList  ycomma  ConstExpression
                                {
                                    /* TODO */
                                    delete $3;
                                    $3 = NULL;
                                }
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
								std::cout << $1;
							}
                      DesignatorStuff
							{
								Variable *var = NULL;
								if (searchStack<Variable>($1, var) && var != NULL) {
									$$ = var->type;
								} else {
									Constant *con = NULL;
									if (searchStack<Constant>($1, con) && con != NULL) {
										$$ = NULL; // TODO infer constant type.
									} else {
									    $$ = NULL;
									   std::cout << "ERROR:: Identifier \"" << $1 << "\" not declared!" << std::endl;
									}
								}
							}
                   ;
DesignatorStuff    :  /*** empty ***/
                   |  DesignatorStuff  theDesignatorStuff
                   ;
theDesignatorStuff :  ydot yident
                   |  yleftbracket ExpList yrightbracket
                   |  ycaret
                   ;
ActualParameters   :  yleftparen  ExpList  yrightparen
                   ;
ExpList            :  Expression
                   |  ExpList  ycomma  Expression
                   ;
MemoryStatement    :  ynew  yleftparen  yident  yrightparen
                   |  ydispose yleftparen  yident
                      yrightparen
                   ;

/***************************  Expression Stuff  ******************************/

Expression         :  SimpleExpression
						{ 
							$$ = $1; 
						}
                   |  SimpleExpression  Relation  SimpleExpression
						{ 
							$$ = BOOLEAN_TYPE;
						}
                   ;
SimpleExpression   :  TermExpr
						{ 
							$$ = $1;
						}
                   |  UnaryOperator  TermExpr
						{ 
							$$ = $2; 
						}
                   ;
TermExpr           :  Term  
						{ 
							$$ = $1; 
						}
                   |  TermExpr  AddOperator  Term
						{
							$$ = getMultAddSubType($1,$3);			
						}
                   ;
Term               :  Factor  
							{
								$$ = $1;
							}
                   |  Term  ymultiply 
							{ 
								std::cout << "*"; 
							}	
							Factor 
							{
								$$ = getMultAddSubType($1,$4);
							}
                   |  Term  ydivide 
							{ 
								std::cout << "/ (double)"; 
							} 
							Factor 
							{
								$$ = getDivideType($1,$4);
							} 
                   |  Term  ydiv 
							{ 
								std::cout << "/"; 
							} 
							Factor  
							{
								$$ = getDivModType($1,$4);
							}
                   |  Term  ymod 
							{ 
								std::cout << "%"; 
							} 
							Factor 
							{
								$$ = getDivModType($1,$4);
							} 
                   |  Term  yand 
							{ 
								std::cout << "&&"; 
								$$ = NULL; 
							} 
							Factor
                   ;
Factor             :  yinteger 
							{ 
								std::cout << $1; 
								$$ = INTEGER_TYPE; 
							}
                   |  yreal 
							{ 
								std::cout << $1; 
								$$ = REAL_TYPE; 
							}
                   |  ytrue 
							{ 
								std::cout << "true"; 
								$$ = BOOLEAN_TYPE;  
							}
                   |  yfalse 
							{ 
								std::cout << "false"; 
								$$ = BOOLEAN_TYPE;  
							}
                   |  ynil 
							{ 
								std::cout << "NULL"; 
								$$ = NULL;  
							}
                   |  ystring 
							{ 
								std::cout << $1; 
								$$ = STRING_TYPE;  
							}
                   |  Designator 
							{
								$$ = NULL;  
							}
                   |  yleftparen 
							{ 
								std::cout << "("; 
							}
                      		Expression 
                      		yrightparen 
							{ 
								std::cout << ")"; 
								$$ = $3;
							}
                   |  ynot  
							{ 
								std::cout << "!"; 
							} 
							Factor 
							{ 
								$$ = $3; 
							}
                   |  Setvalue { $$ = NULL; }
                   |  FunctionCall { $$ = NULL; }
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
					  			{
									//Nina's WIP - no touchy!
									Symbol *fSymbol = NULL;
									bool exists = searchStack($1, fSymbol);
									Function *functionClass;
									if (exists) {
										//Attempt to cast as function
										functionClass = dynamic_cast<Function*>(fSymbol);
										if (functionClass == NULL) {
											std::cout << "ERROR: Symbol " << $1 
												<< " is not a function definition" << std::endl;
										}
									}
									else {
										/* TODO - record error */
										std::cout << "ERROR: Symbol " << $1 
											<< " has not been declared" << std::endl;
									}

									// Check the parameters to see if their types match
									std::vector<Parameter*> params = functionClass->getParameters();
									// 	
								}
                   ;
Setvalue           :  yleftbracket ElementList  yrightbracket
                   |  yleftbracket yrightbracket
                   ;
ElementList        :  Element  
                   |  ElementList  ycomma  Element
                   ;
Element            :  ConstExpression
                                {
                                    /* TODO */
                                    delete $1;
                                    $1 = NULL;
                                }
                   |  ConstExpression  ydotdot  ConstExpression
                                {
                                    /* TODO */
                                    delete $1;
                                    $1 = NULL;
                                    delete $3;
                                    $3 = NULL;
                                }
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
                                std::cout << "}" << std::endl;
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
                                    createProcedure($2, $$);
                                }
                   ;
FunctionHeading    :  yfunction  yident
                                {
                                    createFunction($2, $$);
                                }
                   |  yfunction  yident FormalParameters
                                {
                                    createFunction($2, $$);
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
AddOperator        :  yplus { std::cout << "+"; }
                   |  yminus { std::cout << "-"; }
                   |  yor
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

