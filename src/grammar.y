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
bool handlingInput = false;
bool handlingOutput = false;

Symbol *currentDesignator = NULL;

bool handlingProcFuncCalls = false;
extern YYSTYPE yylval;
extern int lineNumber;

%}

/* definition section */

%start  CompilationUnit
%type   <procedure> ProcedureHeading
%type   <function>  FunctionHeading
%type   <type> Type SetType RecordType AnonType
%type   <pointertype> PointerType
%type   <constvalue> ConstFactor ConstExpression
%type   <unaryop> UnaryOperator
%type   <arraytype> ArrayType

%type   <type> Factor Term TermExpr SimpleExpression Expression Designator FunctionCall
%type   <boolean> AddOperator WhichWay

%token  yand yarray yassign ybegin ycaret ycase ycolon ycomma yconst ydispose
        ydiv ydivide ydo  ydot ydotdot ydownto yelse yend yequal yfalse
        yfor yfunction ygreater ygreaterequal         yif yin yleftbracket
        yleftparen yless ylessequal yminus ymod ymultiply ynew ynil ynot
        ynotequal yof  yor yplus yprocedure yprogram yread yreadln
        yrecord yrepeat yrightbracket yrightparen  ysemicolon yset
        ythen  yto ytrue ytype  yuntil  yvar ywhile ywrite ywriteln yunknown
%token <text> yident ystring yinteger yreal ychar

%%
/* rules section */

/**************************  Pascal program **********************************/

CompilationUnit    :  ProgramModule
                   ;
ProgramModule      :  yprogram yident ProgramParameters ysemicolon
                                {
                                    createProgramScope($2);
                                    std::cout << "class Program {" << std::endl;
                                }
                      Block ydot
                                {
                                    exitScope();
                                    std::cout << "};" << std::endl;
                                    printErrorLog();
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
                                    std::cout << getTabs() << (symbolTable.current)->name << "& call() {" << std::endl;
                                }
                      StatementSequence
                      yend
                                {
                                	std::cout << getTabs() << "\treturn *this;" << std::endl;
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
                                    // TODO ???
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
TypeDef            :  yident yequal Type
                                {
                                    std::cout << getTabs();
                                    createTypeSymbol($1, $3);
                                    std::cout << std::endl;
                                }
                   |  yident yequal PointerType
                   ;
VariableDecl       :  IdentList  ycolon  AnonType
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
           |  ychar
                {
                    createConstValue($$, $1, CHAR);
                }
                   ;
AnonType           :  Type
                   |  ycaret yident
                                {
                                    Symbol *s = NULL;
                                    bool result = symbolTable.searchStack(std::string($2),s);
                                    if (result) {
                                        $$ = new PointerType(s);
                                    } else {
                                        std::cout << "ERROR: \"" << $2 << "\" is undefined!" << std::endl;
                                    }
                                }
                   ;
Type               :  yident    {
                                    getSymbolicType($$, $1);
                                }
                   |  ArrayType
                                {
                                    $$ = dynamic_cast<Type*>($1);
                                }
/*                 |  PointerType  */
                   |  RecordType
                                {
                                    $$ = dynamic_cast<Type*>($1);
                                }
                   |  SetType
                   ;
ArrayType          :  yarray yleftbracket Subrange SubrangeList
                      yrightbracket yof AnonType
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
FieldList          :  IdentList  ycolon  AnonType
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
                      		bool equal = checkTypesEqual($1, $4);
                      		if (!equal) {
                      			std::stringstream ss;
                      			ss << "***Error: Illegal assignment, lefthand and" << 
                      				"righthand sides of the assignment are not equal";
                      		}
                      		std::cout << ";" << std::endl;
                      }
                   ;
ProcedureCall      :  yident
                                {
                                    std::cout << getTabs();
                                    std::cout << $1 << "().call();" << std::endl;
                               		//processProcedureCall($1);
                                }
                   |  yident
                                {
                                    std::cout << getTabs();
                                    std::cout << $1 << "(";
                                }
                      ActualParameters
                                {
                                	//processProcedureCall($1);
                                	std::cout << ").call();" << std::endl;
                                }
                   ;
IfStatement        :  IfStatementBlock
                      EndIf
                   |  IfStatementBlock
                      yelse
                            {
                                exitScope();
                                std::cout << getTabs() << "else {" << std::endl;
                                createLoopCaseScope("else");
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
                                checkConditionalExpressionType($3);
                                std::cout << ") {" << std::endl;
                            }
                      ythen Statement
                   ;
EndIf                 :  /*** empty ***/
                            {
                                exitScope();
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
                                setCaseType($3);
                            }
                      yof
                      CaseList
                            {
                                setCaseType(NULL);
                                exitScope();
                                //std::cout << std::endl << getTabs() << "}" << std::endl;
                            }
                      yend
                   ;
CaseList           :  Case
                            {
                                exitScope();
                                //std::cout << std::endl << getTabs() << "}" <<
                                std::cout << std::endl << getTabs() << "break;" << std::endl;
                            }
                   |  CaseList  ysemicolon
                            {
                                std::cout << std::endl;
                            }
                      Case
                            {
                                exitScope();
                                //std::cout << std::endl << getTabs() << "}" <<
                                std::cout << std::endl << getTabs() << "break;" << std::endl;
                            }
                   ;
Case               :  CaseLabelList  ycolon
                            {
                                printCaseLabel();
                                typeCheckCaseLabel();
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
                                    //delete $1;
                                    //$1 = NULL;
                                    addCaseLabel($1);
                                }
                   |  CaseLabelList  ycomma  ConstExpression
                                {
                                    /* TODO */
                                    //delete $3;
                                    //$3 = NULL;
                                    addCaseLabel($3);
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
                                        std::stringstream ss;
                                        ss << "***ERROR(" << lineNumber << "): Expression is not conditional";
                                        //std::cout << "ERROR: Expression is not conditional" << std::endl;
                                    }
                                    std::cout << ") {" << std::endl;
                                }
                      ydo  Statement
                                {
                                    exitScope();
                                    //std::cout << std::endl << getTabs() << "}" << std::endl;
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
                                    std::cout << ");" << std::endl;
                                }
                      EndRepeat
                   ;
EndRepeat           :    /* empty */
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
                                    bool exists = searchStack($2,var); //Error being printed in searchStack
                                    //Check if yident and Expression have same types
                                    //TODO UNCHECK THIS AFTER CONSTANTS ARE WORKING
                                    if (exists) {
                                    	if(!checkTypesEqual($5, var->type)) {
                                        	//TODO record error
                                        	std::stringstream ss;
                                        	ss << "ERROR: Invalid for loop assignment";
                                        	addError(ss.str());
                                    	}
                                    	if(!checkTypesEqual($8, var->type)) {
                                        	//TODO record error
                                        	std::stringstream ss;
                                        	ss << "ERROR: Incompatible for loop whichway type";
                                        	addError(ss.str());
                                    	}
                                    	std::cout << "; " << $2;
                                    	if ($7)
                                        	std::cout << "++";
                                    	else
                                        	std::cout << "--";
                                    	std::cout << ") {" << std::endl;
                                    }
                                    else {
                                    	stringstream ss;
                                    	ss << "ERROR: Variable does not exist on stack";
                                    	addError(ss.str());
                                    }
                                }
                            ydo  Statement
                                {
                                    exitScope();
                                    //std::cout << std::endl << getTabs() << "}" << std::endl;
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
                                    std::cout << getTabs() << "cin";
                                    handlingOutput = true;
                                }
              DesignatorList  yrightparen
                                {
                                    std::cout << ";" << std::endl;
                                    handlingOutput = false;
                                }
                   |  yreadln
                                {
                                     std::cout << getTabs() << "while (getchar() != '\n')";
                                     std::cout << ";" << std::endl;
                                }
                   |  yreadln  yleftparen
                                {
                                    std::cout << getTabs() << "cin";
                                    handlingOutput = true;
                                }
              DesignatorList  yrightparen
                                {
                                    handlingOutput = false;
                                    std::cout << ";" << std::endl;
                                    std::cout << "cout << \"\\n\";" << std::endl;
                                }
                   |  ywrite  yleftparen
                                {
                                    std::cout << getTabs() << "cout";
                                    handlingInput = true;
                                }
                      ExpList  yrightparen
                                {
                                    std::cout << ";" << std::endl;
                                    handlingInput = false;
                                }
                   |  ywriteln
                                {
                                    std::cout << "cout << \"\\n\"";
                                    std::cout << ";" << std::endl;
                                }
                   |  ywriteln  yleftparen
                                {
                                    std::cout << getTabs() << "cout";
                                    handlingInput = true;
                                }
                      ExpList  yrightparen
                                {
                                    std::cout << " << \"\\n\"";
                                    std::cout << ";" << std::endl;
                                    handlingInput = false;
                                }
                   ;
/***************************  Designator Stuff  ******************************/

DesignatorList     :  Designator
                   |  DesignatorList  ycomma  Designator
                   ;
Designator         :  yident
                            {
                            	Type* potentialReturnType = checkForReturnValue($1);
                            	// Check current scope is a function and if this

                                if (handlingInput)
                                    std::cout << " << ";
                                else if (handlingOutput)
                                    std::cout << " >> ";
                                else if (potentialReturnType == NULL)           
                                	std::cout << $1;
                            	// If not handling return values
                            	if (potentialReturnType == NULL) {
                                	Variable *var = NULL;
                                	if (searchStack<Variable>($1, var) && var != NULL) {
                                    	$$ = var->type;
                                	} else {
                                    	Constant *con = NULL;
                                    	if (searchStack<Constant>($1, con) && con != NULL) {
                                        	$$ = getConstantType(con); // TODO infer constant type.
                                    	} else {
                                        	$$ = NULL;
                                        	std::stringstream ss;
                                        	ss << "***ERROR(" << lineNumber << "): Identifier \"" << $1 << "\" not declared!";
                                        	addError(ss.str());
                                        	//std::cout << "ERROR:: Identifier \"" << $1 << "\" not declared!" << std::endl;
                                    	}
                                	}
                                }
                                else // handling return values
                                {
                                	std::cout << getTabs() << "_returnValue";
                                	$$ = potentialReturnType;
                                }
                            }
                      DesignatorStuff
							{
								
							}
                   ;
DesignatorStuff    :  /*** empty ***/
                   |  DesignatorStuff  theDesignatorStuff
                   ;
theDesignatorStuff :  ydot yident
                   |  yleftbracket ExpList yrightbracket
                   |  ycaret
                            {
                                if (currentDesignator == NULL) {
                                    // return;
                                }
                                Variable *var = dynamic_cast<Variable*>(currentDesignator);
                                if (var == NULL) {
                                    std::cout << "*** Error Cannot dereference \"" << var->name << "\"" << std::endl;
                                    currentDesignator = NULL;
                                } else {
                                    PointerType *pt = dynamic_cast<PointerType*>(var->type);
                                    if (pt == NULL) {
                                        std::cout << "*** Error Cannot dereference \"" << var->name << "\"" << std::endl;
                                        currentDesignator = NULL;
                                    } else {
                                        currentDesignator = pt->getPointee();
                                        std::cout << "[0]" << std::endl;
                                    }
                                }
                            }
                   ;
ActualParameters   :  yleftparen  ExpList  yrightparen
                   ;
ExpList            :  Expression
								{
									if (handlingProcFuncCalls)
										addParameterType($1);
								}
                   |  ExpList  ycomma  Expression
								{
									if (handlingProcFuncCalls)
										addParameterType($3);
								}
                   ;
MemoryStatement    :  ynew  yleftparen  yident
										{
											Variable* sym = NULL;
											bool found = searchStack($3, sym); //Check if variable exists, prints out error within searchStack
											Type* symType = sym->type;
											std::cout << getTabs() << $3 << " = new " <<
												*symType << ";" << std::endl;
										}
					  yrightparen
                   |  ydispose yleftparen  yident
										{
											Variable* sym = NULL;
											bool found = searchStack($3, sym); //Check if variable exists, prints out error within searchStack
											std::cout << getTabs() << "delete " << 
												$3 << ";" << std::endl;
										}
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
                                if (handlingInput)
                                    std::cout << " << ";
                                else if (handlingOutput)
                                    std::cout << " >> ";
                                std::cout << $1;
                                $$ = INTEGER_TYPE;
                            }
                   |  yreal
                            {
                                if (handlingInput)
                                    std::cout << " << ";
                                else if (handlingOutput)
                                    std::cout << " >> ";
                                std::cout << $1;
                                $$ = REAL_TYPE;
                            }
                   |  ytrue
                            {
                                if (handlingInput)
                                    std::cout << " << ";
                                else if (handlingOutput)
                                    std::cout << " >> ";
                                std::cout << "true";
                                $$ = BOOLEAN_TYPE;
                            }
                   |  yfalse
                            {
                                if (handlingInput)
                                    std::cout << " << ";
                                else if (handlingOutput)
                                    std::cout << " >> ";
                                std::cout << "false";
                                $$ = BOOLEAN_TYPE;
                            }
                   |  ynil
                            {
                                if (handlingInput)
                                    std::cout << " << ";
                                else if (handlingOutput)
                                    std::cout << " >> ";
                                std::cout << "NULL";
                                $$ = NULL;
                            }
                   |  ystring
                            {
                                if (handlingInput)
                                    std::cout << " << ";
                                else if (handlingOutput)
                                    std::cout << " >> ";
                                std::cout << "\"" << $1 << "\"";
                                $$ = STRING_TYPE;
                            }
                  |  ychar
                            {
                                if (handlingInput)
                                    std::cout << " << ";
                                else if (handlingOutput)
                                    std::cout << " >> ";
                                std::cout << "\'" << $1 << "\'";
                                $$ = CHAR_TYPE;
                            }
                   |  Designator
                            {
                                if (handlingInput)
                                    std::cout << " << ";
                                else if (handlingOutput)
                                    std::cout << " >> ";
                                $$ = $1;
                            }
                   |  yleftparen
                            {
                                if (handlingInput)
                                    std::cout << " << ";
                                else if (handlingOutput)
                                    std::cout << " >> ";
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
                                if (handlingInput)
                                    std::cout << " << ";
                                else if (handlingOutput)
                                    std::cout << " >> ";
                                std::cout << "!";
                            }
                            Factor
                            {
                                $$ = $3;
                            }
                   |  Setvalue { $$ = NULL; }
                   |  FunctionCall { $$ = $1; }
                   ;
/*  Functions with no parameters have no parens, but you don't need         */
/*  to handle that in FunctionCall because it is handled by Designator.     */
/*  A FunctionCall has at least one parameter in parens, more are           */
/*  separated with commas.                                                  */
FunctionCall       :  yident
                                {
                                    std::cout << getTabs();
									std::cout << $1 << "(";
									handlingProcFuncCalls = true;
                                }
                      ActualParameters
                                {
                                    handlingProcFuncCalls = false;
                                    std::cout << ").call()._returnValue";
                                    $$ = processFunctionCall($1);
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
                                std::cout << getTabs() << "};" << std::endl;
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
                                std::cout << getTabs() << "};" << std::endl;
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

