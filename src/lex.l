%{
/******************************************************************************
 * File: lex.l
 * Authors:  Evelina Arthursson, Derek Mclean, Hugo Ribeiro
 * This file is dependent on external files "tokenconsts.h" and "analyzer.h"
 * This flex file parses through a given file and looks for tokens
 * related to Pascal grammar. When it finds a keyword or symbol, it
 * returns the appropriate int associated with it. If it encounters
 * a comment it will convert the comment symbols to C comment symbols
 * and echo the content of the comment.
 * If the tokenizer encounters a newline it will echo that newline.
 *
 * char* tokenText is referred to by grammar.y (the yacc file) in
 * order to output the identifier text.
 */

#include "y.tab.h"
#include "parser.h"

#include <stdio.h>
#include <string.h>

#include <iostream>	// For cout
#include <iomanip>	// For setw and left

void saveText();

%}

%option case-insensitive

letter			[a-zA-Z]
digit 			[0-9]

%s STRING
%s STRING2
%s COMMENT
%s COMMENT2

%%
	/** Identifiers **/
and				{ return yand; }
array			{ return yarray; }
begin			{ return ybegin; }
case 			{ return ycase; }
const 			{ return yconst; }
dispose			{ return ydispose; }
div 			{ return ydiv; }
do 		        { return ydo; }
downto 			{ return ydownto; }
else 			{ return yelse; }
end 			{ return yend; }
false 			{ return yfalse; }
for 			{ return yfor; }
function    	{ return yfunction; }
if  			{ return yif; }
in  			{ return yin; }
mod 			{ return ymod; }
new				{ return ynew; }
nil 			{ return ynil; }
not 			{ return ynot; }
of  			{ return yof; }
or  			{ return yor; }
procedure		{ return yprocedure; }
program			{ return yprogram; }
read			{ return yread; }
readln			{ return yreadln; }
record			{ return yrecord; }
repeat			{ return yrepeat; }
set				{ return yset; }
then			{ return ythen; }
to				{ return yto; }
true			{ return ytrue; }
type			{ return ytype; }
until			{ return yuntil; }
var				{ return yvar; }
while			{ return ywhile; }
write			{ return ywrite; }
writeln			{ return ywriteln; }
	/** Strings **/
<INITIAL>[\"]		    { BEGIN(STRING); }
<STRING>[^"]*		    { yymore(); }
<STRING>["]			    {
						    yyless(yyleng-1);
                            yylval.text = strdup(yytext);
						    yyinput();
						    BEGIN(0);
						    return ystring;
					    }
<STRING><<EOF>>		    {
                            yylval.text = strdup(yytext);
						    BEGIN(0);
						    return ystring;
					    }

<INITIAL>[\']			{ BEGIN(STRING2); }
<STRING2>[^']*			{ yymore(); }
<STRING2>[']			{
							yyless(yyleng-1);
                            yylval.text = strdup(yytext);
							yyinput();
							BEGIN(0);
							return ystring;
						}
<STRING2><<EOF>>		{
                            yylval.text = strdup(yytext);
							BEGIN(0);
							return ystring;
						}

	/** Comments **/
<INITIAL>[\(][\*]	    { printf("/*"); BEGIN(COMMENT); }
<COMMENT>[^\*\)]*	    { ECHO; }
<COMMENT>[\*]/[\)]	    {
				            printf("*/");
				            yyinput();
				            BEGIN(0);
        			    }
<COMMENT>[\*]/[^\)]	    { ECHO; }
<COMMENT><<EOF>>	    { printf("*/"); BEGIN(0); }

<INITIAL>[{]            { printf("/*"); BEGIN(COMMENT2);}
<COMMENT2>[^}]*         { ECHO; }
<COMMENT2>[}]           { printf("*/"); BEGIN(0); }
<COMMENT2><<EOF>>       { printf("*/"); BEGIN(0); }
	/** Operands **/
":="			{ return yassign; }
\^			    { return ycaret; }
:			    { return ycolon; }
","			    { return ycomma; }
\/			    { return ydivide; }
"."	 		    { return ydot; }
".." 			{ return ydotdot; }
"=" 			{ return yequal; }
>   			{ return ygreater; }
>=  			{ return ygreaterequal; }
<INITIAL>"["    { return yleftbracket; }
<INITIAL>"("    { return yleftparen; }
<INITIAL><      { return yless; }
"<="  			{ return ylessequal; }
"-"   			{ return yminus; }
"*"  			{ return ymultiply; }
"<>"  			{ return ynotequal; }
"+"			    { return yplus; }
"]"			    { return yrightbracket; }
<INITIAL>")"    { return yrightparen; }
";"			    { return ysemicolon; }
	/** Misc */
\n					{ ECHO; }
{letter}({letter}|[0-9])* 				{
                                            yylval.text = strdup(yytext);
											return yident;
										}
{digit}+                                {
                                            yylval.text = strdup(yytext);
											return yinteger;
										}
{digit}+(\.{digit}+)?(e[+-]?{digit}+)?	{
                                            yylval.text = strdup(yytext);
											return yreal;
										}
[[:space:]]				/** Ignore */
<INITIAL>.								{
                                            yylval.text = strdup(yytext);
											return yunknown;
										}

%%


int yywrap()
{
	return 1;
}
