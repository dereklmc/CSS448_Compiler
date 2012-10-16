%{

#include "tokenconsts.h"
#include "analyzer.h"

#include <stdio.h>
#include <string.h>

#include <iostream>	// For cout	      
#include <iomanip>	// For setw and left

char* tokenText;	// Holds yytext for display

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
<INITIAL>[\"]		{ BEGIN(STRING); }
<STRING>[^"]*		{ yymore(); }
<STRING>["]			{
						yyless(yyleng-1);
						tokenText = new char[yyleng];
						strcpy(tokenText, yytext);
						yyinput(); 
						BEGIN(0);
						return ystring;
					}
<STRING><<EOF>>		{
						tokenText = new char[yyleng];
						strcpy(tokenText, yytext);
						BEGIN(0);
						return ystring;
					}

<INITIAL>[\']			{ BEGIN(STRING2); }
<STRING2>[^']*			{ yymore(); }
<STRING2>[']			{
							yyless(yyleng-1);
							tokenText = new char[yyleng];
							strcpy(tokenText, yytext);
							yyinput();
							BEGIN(0);
							return ystring;
						}
<STRING2><<EOF>>		{
							tokenText = new char[yyleng];
							strcpy(tokenText, yytext);
							BEGIN(0);
							return ystring;
						}

	/** Comments **/
<INITIAL>[\(][\*]	{ printf("/*"); BEGIN(COMMENT); }
<COMMENT>[^\*\)]*	{ ECHO; }
<COMMENT>[\*]/[\)]	{
				printf("*/");
				yyinput();
				BEGIN(0);
			}
<COMMENT>[\*]/[^\)]	{ ECHO;	}
<COMMENT><<EOF>>	{ printf("*/"); BEGIN(0); }

<INITIAL>[{]            { printf("/*"); BEGIN(COMMENT2);}
<COMMENT2>[^}]*         { ECHO; }
<COMMENT2>[}] 		{ printf("*/"); BEGIN(0); }
<COMMENT2><<EOF>>       { printf("*/"); BEGIN(0); }
	/** Operands **/	
":="			{ return yassign; }
\^			{ return ycaret; }
:			{ return ycolon; }
","			{ return ycomma; }
\/			{ return ydivide; }
"."	 		{ return ydot; }
".." 			{ return ydotdot; }
"=" 			{ return yequal; }
">"   			{ return ygreater; }
">="  			{ return ygreaterequal; }
<INITIAL>"["  		{ return yleftbracket; }
<INITIAL>"("		{ return yleftparen; }
<INITIAL>"<"   			{ return yless; }
"<="  			{ return ylessequal; }
"-"   			{ return yminus; }
"*"  			{ return ymultiply; }
"<>"			{ return ynotequal; }
"+"			{ return yplus; }
"]"			{ return yrightbracket; }
<INITIAL>")"		{ return yrightparen; }
";"			{ return ysemicolon; }
	/** Misc */
\n					{ ECHO; }
{letter}({letter}|[0-9])* 				{
											tokenText = new char[yyleng];
											strcpy(tokenText, yytext);
											return yident;
										}
{digit}+(\.{digit}+)?(e[+-]?{digit}+)?	{
											tokenText = new char[yyleng];
											strcpy(tokenText, yytext);
											return ynumber;
										}
[[:space:]]				/** Ignore */
<INITIAL>.								{
											tokenText = new char[yyleng];
											strcpy(tokenText, yytext);
											return yunknown;
										}

%%

int getNewTokenId(int oldTokenId)
{
	int newTokenId = oldTokenId - subtractFromToken;
	return newTokenId;
}

void displayToken(int tokenId)
{
	std::string tName;
	tName = tokenNames[getNewTokenId(tokenId)];

	std::cout << std::left << std::setw(8) <<  tokenId;
	std::cout << std::left << std::setw(15) << tokenNames[tokenId-257];
	
	if ( (tName == "ynumber") || (tName == "yident") ||
		 (tName == "yunknown") || (tName == "ystring") )
	{
		std::cout << tokenText;
	}
	std::cout << "\n";	
}

int yywrap()
{
	return 1;
}
