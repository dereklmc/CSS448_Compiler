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
#include "sep.h"

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;
bool handlingProcFuncCalls = false;
bool designatorTab = true;
extern YYSTYPE yylval;
extern int lineNumber;

std::stack<std::string> seps;

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
                                }
                      Block ydot
                                {
                                    exitScope();
                                    endProgram($2);
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
                                    startBlock();
                                }
                      StatementSequence
                      yend
                                {
                                	endBlock();
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
                                    createAR();
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
                                    createConstant($1, $3);
                                    std::cout << std::endl;
                                }
                   ;
TypeDef            :  yident yequal Type
                                {
                                    if($3->AR)
                                        addAR($1, $3);
                                    else{
                                        createTypeSymbol($1, $3);
                                        std::cout << std::endl;
                                    }
                                }
                   ;
VariableDecl       :  IdentList  ycolon  AnonType
                                {
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
                   |  ychar
                                {
                                    createConstValue($$, $1, CHAR);
                                }
                   ;
AnonType           :  Type
                                {
                                    checkPointers();
                                    // check Records
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
                                {
                                    $$ = dynamic_cast<Type*>($1);
                                }
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
                   |  StatementSequence  ysemicolon
		      Statement
                   ;
Statement          :  Assignment
                   |  ProcedureCall
                   |  IfStatement
                   |  CaseStatement
                   |  WhileStatement
                   |  RepeatStatement
                   |  ForStatement
                   |  {std::cout << getTabs();}	IOStatement
                   |  MemoryStatement
                   |  ybegin StatementSequence yend
                   |  /*** empty ***/
                   ;
				   ;
Assignment         :  Designator
                      yassign 
								{ 
									std::cout << " = "; 
									designatorTab = false;
								}
                      Expression
                      {
                            /*********** ASSIGNMENT DEBUG ************/
                            /*
                            std::cout << "==*== ASSIGNMENT TYPES ::: ";
                            std::cout << "<";
                            std::cout << std::flush << $1 << std::flush;
                            std::cout << ">=\"";
                            std::cout << std::flush << *($1) << std::flush;
                            std::cout << "\"";
                            std::cout << " ::== ";
                            std::cout << "<";
                            std::cout << std::flush << $4 << std::flush;
                            std::cout << ">=\"";
                            std::cout << std::flush << *($4) << std::flush;
                            std::cout << "\"";
                            std::cout << " ==*==" << std::endl;
                            /**/
                            /*********** ASSIGNMENT DEBUG ************/
							designatorTab = true;
                      		bool equal = checkTypesEqual($4, $1);
                      		if (!equal) {
                      			std::stringstream ss;
                      			ss << "***Error(line: " << lineNumber 
									<< "): Illegal assignment, lefthand and " << 
               						"righthand sides of the assignment are not equal";
								addError(ss.str());
                      		}
                      		std::cout << ";" << std::endl;
                      }
                   ;
ProcedureCall      :  yident
                                {
                                    std::cout << getTabs();
                                    std::cout << $1 << "(this).call();" << std::endl;
                               		processProcedureCall($1);
                                }
                   |  yident
                                {
                                    std::cout << getTabs();
                                    std::cout << $1 << "(this,";
                                }
                      ActualParameters
                                {
                                	processProcedureCall($1);
                                	std::cout << ").call();" << std::endl;
                                }
                   ;
IfStatement        :  IfStatementBlock
                      EndIf
                   |  IfStatementBlock
                      yelse
                            {
                                exitScope();
				std::cout << std::endl;
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
								designatorTab = false;
                            }
                      Expression
                            {
								designatorTab = true;
                                checkConditionalExpressionType($3);
                                std::cout << ") {" << std::endl;
                            }
                      ythen Statement
                   ;
EndIf                 :  /*** empty ***/
                            {
                                exitScope();
				std::cout << std::endl;
                            }
                      ;
CaseStatement      :  ycase
                               {
                                std::cout << getTabs() << "switch(";
                                createLoopCaseScope("case");
								designatorTab = false;
                            }
                      Expression
                            {
								designatorTab = true;
                                std::cout << ") {" << std::endl;
                                setCaseType($3);
                            }
                      yof
                      CaseList
                            {
                                setCaseType(NULL);
                                exitScope();
				std::cout << std::endl;
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
									designatorTab = false;
                                }
                      Expression
                                {
									designatorTab = true;
                                    if (!BOOLEAN_TYPE->equals($3)) {
                                        // TODO, record error message
                                        std::stringstream ss;
                                        ss << "***ERROR(" << lineNumber 
											<< "): Expression is not conditional";
										addError(ss.str());
                                    }
                                    std::cout << ") {" << std::endl;
                                }
                      ydo  Statement
                                {
                                    exitScope();
				    std::cout << std::endl;
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
				    				std::cout << " while(";
									designatorTab = false;
                                }
                      Expression
                                {
									designatorTab = true;
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
									designatorTab = false;
                                }
                            yassign  Expression
                                {
                                    std::cout << "; " << $2 << " ";
                                }
                            WhichWay  Expression
                                {
									designatorTab = true;
                                    Variable *var = NULL;
                                    bool exists = searchStack($2,var); //Error being printed in searchStack
                                    //Check if yident and Expression have same types
                                    //TODO UNCHECK THIS AFTER CONSTANTS ARE WORKING
                                    if (exists) {
                                    	if(!checkTypesEqual($5, var->type)) {
                                        	//TODO record error
                                        	std::stringstream ss;
                                        	ss << "***ERROR(line: " << lineNumber 
												<< "): Invalid for loop assignment";
                                        	addError(ss.str());
                                    	}
                                    	if(!checkTypesEqual($8, var->type)) {
                                        	//TODO record error
                                        	std::stringstream ss;
                                        	ss << "***ERROR(line: " << lineNumber 
												<< ": Incompatible for loop whichway type";
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
                                    	ss << "***ERROR(line: " << lineNumber 
											<< "): Variable does not exist on stack";
                                    	addError(ss.str());
                                    }
                                }
                            ydo  Statement
                                {
                                    exitControlScope();
			 	    std::cout << std::endl;
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
                                    std::cout << "cin >> ";
									designatorTab = false;
									seps.push(INPUT);
                                }
                      DesignatorList  yrightparen
                                {
                                    std::cout << ";" << std::endl;
									designatorTab = true;
									seps.pop();
                                }
                   |  yreadln
                                {
                                     std::cout << "while (getchar() != '\n')";
                                     std::cout << ";" << std::endl;
                                }
                   |  yreadln  yleftparen
                                {
                                    std::cout << "cin >> ";
									designatorTab = false;
									seps.push(INPUT);
                                }
                      DesignatorList  yrightparen
                                {
									designatorTab = true;
                                    std::cout << ";" << std::endl;
                                    std::cout << "cout << \"\\n\";" << std::endl;
                                    seps.pop();
                                }
                   |  ywrite  yleftparen
                                {
                                    std::cout << "cout << ";
									designatorTab = false;
                                    seps.push(OUTPUT);
                                }
                      ExpList  yrightparen
                                {
									designatorTab = true;
                                    std::cout << ";" << std::endl;
                                    seps.pop();
                                }
                   |  ywriteln
                                {
                                    std::cout << "cout << \"\\n\"";
                                    std::cout << ";" << std::endl;
                                }
                   |  ywriteln  yleftparen
                                {
                                    std::cout << "cout << ";
									designatorTab = false;
									seps.push(OUTPUT);
									
                                }
                      ExpList  yrightparen
                                {
									designatorTab = true;
                                    std::cout << " << \"\\n\"";
                                    std::cout << ";" << std::endl;
                                    seps.pop();
                                }
                   ;
/***************************  Designator Stuff  ******************************/

DesignatorList     :  Designator
                   |  DesignatorList  ycomma
                            {
                                std::cout << seps.top();
                            }
                      Designator
                   ;
Designator         :  yident
                            {
                            	Type* potentialReturnType = checkForReturnValue($1);
                            	// Check current scope is a function and if this
								if (designatorTab) {
									std::cout << getTabs();
								}
								
                                if (potentialReturnType == NULL) {
                                    Symbol *foundDesignator = NULL;
                                    int distance = symbolTable.findSymbol($1,foundDesignator);
                                    if (distance < 0) {
                                        // TODO print error;
                                    }
                                    for (int i = 0; i < distance; i++) {
                                        std::cout << "parent->";
                                    }
                                	std::cout << $1 << std::flush;
                                    designators.push(foundDesignator);
                            	}
                                
                            }
                      DesignatorStuff
                            {
                            	Type* potentialReturnType = NULL;
                            	potentialReturnType = checkForReturnValue($1);
                            	// If not handling return values
                            	if (potentialReturnType == NULL) {
                                	Variable *var = dynamic_cast<Variable*>(designators.top());
                                	Constant *con = dynamic_cast<Constant*>(designators.top());
                                	TypeSymbol *ts = dynamic_cast<TypeSymbol*>(designators.top());
                                	if (var != NULL) {
                                    	$$ = var->type;
                                	} else if (con != NULL) {
										Type* t = getConstantType(con);
                                    	$$ = t; // TODO infer constant type.
                                	} else if (ts != NULL) {
                                    	$$ = ts->getMyType();
                                	} else {
                                    	$$ = NULL;
                                    	std::stringstream ss;
                                    	ss << "***ERROR(" << lineNumber << "): Identifier \"" << $1 << "\" not declared!";
                                    	addError(ss.str());
                                	}
                                }
                                else // handling return values
                                {
                                	std::cout << getTabs() << "_returnValue";
                                	$$ = potentialReturnType;
                                }
								if (!designators.empty())
                                	designators.pop();
                            }
                   ;
DesignatorStuff    :  /*** empty ***/
                   |  DesignatorStuff  theDesignatorStuff
                   ;
theDesignatorStuff :  ydot yident
                            {
                                accessField($2);
                            }
                   |  yleftbracket 
							{
								designatorTab = false;
								seps.push(ARRAY);
								std::cout << "[";
							    accessArray();
							}
                      ExpList 
							{
							    accessedArrayRanges.clear();
							    std::cout << "]";
								designatorTab = true;
								seps.pop();
							}
                      yrightbracket
                   |  ycaret
                            {
                                dereferenceDesignator();
                            }
                   ;
ActualParameters   :  yleftparen
							{
								designatorTab = false;
								handlingProcFuncCalls = true;
								seps.push(COMMA);
							}
					  ExpList  
							{
							    seps.pop();
								handlingProcFuncCalls = false;
								designatorTab = true;
							}
					  yrightparen
                   ;
ExpList            :  Expression
						    {
							    if (handlingProcFuncCalls) {
								    addParameterType($1);
								}
								if (accessedArrayRanges.empty() && seps.top() == ARRAY) {
								    accessArray();
								} 
							    if (!accessedArrayRanges.empty()) {
							        Range *range = accessedArrayRanges.front();
							        accessedArrayRanges.pop_front();
							        std::cout << " - " << range->getStart();
							    }
						    }
                   |  ExpList
                            {
                                std::cout << seps.top();
                            }
                      ycomma
                      Expression
							{
								if (handlingProcFuncCalls) {
									addParameterType($4);
								}
								if (accessedArrayRanges.empty() && seps.top() == ARRAY) {
								    accessArray();
								} 
							    if (!accessedArrayRanges.empty()) {
							        Range *range = accessedArrayRanges.front();
							        accessedArrayRanges.pop_front();
							        std::cout << " - " << range->getStart();
							    }
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
                            if (checkTypesEqual($3,$1)) {
                            	$$ = BOOLEAN_TYPE;
                            //TODO - put this back in when Constants are implemented
                            }
                            else
                            {
								std::stringstream ss;
                                ss << "***ERROR(line: " << lineNumber << "): Invalid relation";
								addError(ss.str());
                                $$ = NULL;
                            }
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
                                std::cout << " && ";

                            }
                            Factor
							{
								if (checkTypesEqual($1,$4))
									$$ = BOOLEAN_TYPE;
								else
									$$ = NULL;
							}
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
                                $$ = NIL_TYPE;
                            }
                   |  ystring
                            {
                                std::cout << "\"" << $1 << "\"";
                                $$ = STRING_TYPE;
                            }
                  |  ychar
                            {
                                std::cout << "\'" << $1 << "\'";
                                $$ = CHAR_TYPE;
                            }
                   |  Designator
                            {
                                $$ = $1;
                            }
                   |  yleftparen
                            {
                                std::cout << "(" << std::flush;
                            }
                              Expression
                              yrightparen
                            {
                                std::cout << ")" << std::flush;
                                $$ = $3;
                            }
                   |  ynot
                            {
                                std::cout << "!" << std::flush;
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
                                    std::cout << $1 << "(this,";
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
                                createProcedureDecl($1);
                            }
                      Block
                            {
                                exitScope();
                                std::cout << ";" << std::endl;
                            }
                   ;
FunctionDecl       :  FunctionHeading  ycolon  yident
                            {
                                createFunctionDecl($3, $1);
                            }
                      ysemicolon  Block
                            {
                                exitScope();
                                std::cout << ";" << std::endl;
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

