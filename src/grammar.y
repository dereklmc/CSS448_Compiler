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
%type	<boolean> AddOperator WhichWay

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
                                std::cout << getTabs() << "void call() {" << std::endl;
                            }
                      StatementSequence
                      yend
                            {
                                std::cout << getTabs() << "}" << std::endl;
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
									std::cout << getTabs();
                                    createConstant($1, $3);
									std::cout << std::endl;
                                }
                   ;
TypeDef            :  yident yequal  Type
                                {
									std::cout << getTabs();
                                    createTypeSymbol($1, $3);
									std::cout << std::endl;
                                }
                   ;
VariableDecl       :  IdentList  ycolon  Type
                                {
                                    checkPointers();
									std::cout << getTabs();
                                    createVariables($3);
									std::cout << std::endl;
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
									std::cout << getTabs();
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
									std::cout << getTabs();
                                    printf("%s ", $1);
                                }
                   |  yident 
                                {
									std::cout << getTabs();
                                    printf("%s ", $1);
                                }
                      ActualParameters
								{
									std::cout << std::endl;
								}
                   ;
IfStatement        :  IfStatementBlock
                      EndIf
                   |  IfStatementBlock
                      yelse
                            {
                                std::cout << getTabs() << "} else {" << std::endl;	
                            }
                      Statement
                      EndIf
                   ;
IfStatementBlock   :  yif 
                            {
								
                                std::cout << getTabs() << "if (";	
								createLoopCaseScope("if");
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
								exitScope();
						        std::cout << std::endl << getTabs() << "}" << std::endl;	
					        }
           		   ;
CaseStatement      :  ycase  
					   		{
								std::cout << getTabs() << "switch(";
								createLoopCaseScope("case");
							}
					  Expression  
							{
								std::cout << ") {" << std::endl;
							}
					  yof  
					  		
					  CaseList  
							{
								exitScope();
								std::cout << std::endl << getTabs() << "}" << std::endl;
							}
					  yend
                   ;
CaseList           :  Case
							{
								exitScope();
								std::cout << std::endl << getTabs() << "}" << 
											std::endl << getTabs() << "break;" << std::endl;
							}
                   |  CaseList  ysemicolon 
							{
								std::cout << std::endl;
							}
					  Case
							{
								exitScope();
								std::cout << std::endl << getTabs() << "}" <<
										std::endl << getTabs() << "break;" << std::endl;
							}
                   ;
Case               :  CaseLabelList  ycolon  
							{
								//Will want to print out caselabellist after "case"
								std::cout << getTabs() << "case placeholdercase" << ": " << std::endl << getTabs() << "{" << std::endl;						
								createLoopCaseScope("Inner case");
							}
					  Statement
							{
								std::cout << std::endl;
							}
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
WhileStatement     :  ywhile
								{
									std::cout << getTabs() << "while (";
									createLoopCaseScope("while");
								}
					  Expression  
								{
									if (!BOOLEAN_TYPE->equals($3)) {
										// TODO, record error message
										std::cout << "ERROR: Expression is not conditional" << std::endl;
									}	
									std::cout << ") {" << std::endl;
								}
					  ydo  Statement
								{
									exitScope();
									std::cout << std::endl << getTabs() << "}" << std::endl;
								}
                   ;
RepeatStatement    :  yrepeat  
								{
									std::cout << getTabs() << "do {" << std::endl;
									createLoopCaseScope("repeat");
								}
					  StatementSequence  yuntil  
								{
									exitScope();
									std::cout << getTabs() << "} while(";
								}
					  Expression
								{
									// The ");" is printed on a new line because of the lexxer
									std::cout << ");" << std::endl;
								}
					  EndRepeat
                   ;

EndRepeat		   :	/* empty */
				   ;
ForStatement       :  yfor  
					  yident  
                                {
									std::cout << getTabs() << "for ("; 
									createLoopCaseScope("for");
									printf("%s ", $2);
									std::cout << "= ";
                                }
                            yassign  Expression  
								{	
									std::cout << "; " << $2 << " ";
								}							
							WhichWay  Expression
								{
									Variable *var = NULL;
									searchStack($2,var); //Error being printed in searchStack
									//Check if yident and Expression have same types
									//TODO UNCHECK THIS AFTER CONSTANTS ARE WORKING
									//if(!checkTypesEqual(var->type, $5)) {
										//TODO record error
										//std::cout << "ERROR: Invalid assignment" << std::endl; 	
									//}
									//if(!checkTypesEqual(var->type, $8)) {
										//TODO record error
										//std::cout << "ERROR: Incompatible whichway type" << std::endl; 	
									//}
									std::cout << "; " << $2;
									if ($7)
										std::cout << "++";
									else
										std::cout << "--";
									std::cout << ") {" << std::endl;
								}
                            ydo  Statement
								{
									exitScope();
									std::cout << std::endl << getTabs() << "}" << std::endl;
								}
                   ;
WhichWay           :  yto  
								{
									// WhichWay will be true if it is "to" and false if "downto"
									std::cout << "< ";
									$$ = true;
								}
					  |  ydownto
								{								
									std::cout << "> ";
									$$ = false;
								}
                   ;
IOStatement        :  yread  yleftparen  
								{
									std::cout << getTabs() << "cin >> ";
								}
					  DesignatorList  yrightparen
								{std::cout << ";" << std::endl;}
                   |  yreadln  
								{ std::cout << ";" << std::endl; }
                   |  yreadln  yleftparen 
								{
									std::cout << getTabs() << "cin >> ";
								}
					  DesignatorList  yrightparen
								{std::cout << ";" << std::endl;}
                   |  ywrite  yleftparen  
								{
									std::cout << getTabs() << "cout << ";
								}
					  ExpList  yrightparen
								{std::cout << ";" << std::endl;}
                   |  ywriteln  
								{std::cout << ";" << std::endl;}
                   |  ywriteln  yleftparen  
								{
									std::cout << getTabs() << "cout << ";
								}
					  ExpList  yrightparen
								{std::cout << ";" << std::endl;}
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
							//if (checkTypesEqual($1,$3))
							$$ = BOOLEAN_TYPE;
							//TODO - put this back in when Constants are implemented
							//else
							//{
								//std::cout << "ERROR: Invalid relation" << std::endl;
								//$$ = NULL;
							//}
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
							if ($2) {
								$$ = BOOLEAN_TYPE;
							}
							else {
								$$ = getMultAddSubType($1,$3);	
							}		
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
								$$ = $1;  
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
									std::cout << getTabs();
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
								std::cout << getTabs();
                                createProcedureDecl($1);
                            }
                      Block
                            {
                                exitScope();
                                std::cout << getTabs() << "}" << std::endl;
                            } 
                   ;
FunctionDecl       :  FunctionHeading  ycolon  yident
                            {
								std::cout << getTabs();
                                createFunctionDecl($3, $1);
                            }
                      ysemicolon  Block
                            {
                                exitScope();
                            }
                   ;
ProcedureHeading   :  yprocedure yident
                                {
									std::cout << getTabs();
                                    createProcedure($2, $$);
                                }
                   |  yprocedure yident FormalParameters
                                {
									std::cout << getTabs();
                                    createProcedure($2, $$);
                                }
                   ;
FunctionHeading    :  yfunction  yident
                                {
									std::cout << getTabs();
                                    createFunction($2, $$);
                                }
                   |  yfunction  yident FormalParameters
                                {
									std::cout << getTabs();
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
									std::cout << getTabs();
                                    createParameter($4);
                                }
                   |  IdentList ycolon yident
                                {
									std::cout << getTabs();
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
AddOperator        :  yplus { std::cout << "+"; $$ = false;}
                   |  yminus { std::cout << "-"; $$ = false;}
                   |  yor     { std::cout << " || "; $$ = true;}
                   ;
Relation           :  yequal  { std::cout << " == "; }
		      | ynotequal { std::cout << " != "; }
 		      | yless { std::cout << " < "; }
		      | ygreater  { std::cout << " > "; }
                      |  ylessequal { std::cout << " <= "; }
		      | ygreaterequal { std::cout << " >= "; }
		      | yin
                   ;

%%

/* program section */

void yyerror(const char *s) {
   cout << s << endl;
}

extern int yylex();

