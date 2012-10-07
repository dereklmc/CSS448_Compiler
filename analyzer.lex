%{

#include "tokenconsts.h"
#include <iostream>
#include <iomanip>

static char* text;
static const int subtractFromToken = 257;

static const char* tokenNames[] = {
	"yand",
	"yarray",
	"yassign",
	"ybegin",
	"ycaret",
	"ycase",
	"ycolon",
	"ycomma",
	"yconst",
	"ydispose",
	"ydiv",
	"ydivide",
	"ydo",
	"ydot",
	"ydotdot",
	"ydownto",
	"yelse",
	"yend",
	"yequal",
	"yfalse",
	"yfor",
	"yfunction",
	"ygreater",
	"ygreaterequal",
	"yident",
	"yif",
	"yin",
	"yleftbracket",
	"yleftparen",
	"yless",
	"ylessequal",
	"yminus",
	"ymod",
	"ymultiply",
	"ynew",
	"ynil",
	"ynot",
	"ynotequal",
	"ynumber",
	"yof",
	"yor",
	"yplus",
	"yprocedure",
	"yprogram",
	"yread",
	"yreadln",
	"yrecord",
	"yrepeat",
	"yrightbracket",
	"yrightparen",
	"ysemicolon",
	"yset",
	"ystring",
	"ythen",
	"yto",
	"ytrue",
	"ytype",
	"yuntil",
	"yvar",
	"ywhile",
	"ywrite",
	"ywriteln",
	"yunknown"
};

static const int nextToken = -1;

%}

%option case-insensitive

letter			[a-zA-Z]
digit 			[0-9]

%s STRING

%%
	/** Identifiers **/
and			{ return yand; }
array       		{ return yarray; }
begin			{ return ybegin; }
case 			{ return ycase; }
const 			{ return yconst; }
dispose			{ return ydispose; }
div 			{ return ydiv; }
do 			{ return ydo; }
downto 			{ return ydownto; }
else 			{ return yelse; }
end 			{ return yend; }
false 			{ return yfalse; }
for 			{ return yfor; }
function    		{ return yfunction; }
if  			{ return yif; }
in  			{ return yin; }
mod 			{ return ymod; }
new			{ return ynew; }
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
set			{ return yset; }
then			{ return ythen; }
to			{ return yto; }
true			{ return ytrue; }
type			{ return ytype; }
until			{ return yuntil; }
var			{ return yvar; }
while			{ return ywhile; }
write			{ return ywrite; }
writeln			{ return ywriteln; }
	/** Operands **/
<INITIAL>[\"]	{ yymore(); BEGIN(STRING); return nextToken; }
<STRING>[^"]*	{ yymore(); return nextToken; }
<STRING>["]	{ text = yytext; BEGIN(0); return ystring; }
<STRING><<EOF>>	{ text = yytext; BEGIN(0); return ystring; }
":="			{ return yassign; }
\^			{ return ycaret; }
:			{ return ycolon; }
,			{ return ycomma; }
\/			{ return ydivide; }
"."	 		{ return ydot; }
".." 			{ return ydotdot; }
"=" 			{ return yequal; }
">"   			{ return ygreater; }
">="  			{ return ygreaterequal; }
"["  			{ return yleftbracket; }
"("  			{ return yleftparen; }
"<"   			{ return yless; }
"<="  			{ return ylessequal; }
"-"   			{ return yminus; }
"*"  			{ return ymultiply; }
"<>"			{ return ynotequal; }
"+"			{ return yplus; }
"]"			{ return yrightbracket; }
")"			{ return yrightparen; }
";"			{ return ysemicolon; }
	/** Misc */
{letter}({letter}|[0-9])* 			{ text = yytext; return yident; }
{digit}*|{digit}+(.{digit}+)?(e[+-]?{digit}+)?		{ text = yytext; return ynumber; }
[[:space:]]			/** Ignore */
.						{ text = yytext; return yunknown; }

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
	if ( (tName == "ynumber") || (tName == "yident") || (tName == "yunknown") || (tName == "ystring") ){
		std::cout << text;
	}
	std::cout << "\n";	
}

int main(void)
{
	while (1)
	{
		int tokenId = yylex();
		if (tokenId == 0) {
			break;
		} else if (tokenId > 0) {
			displayToken(tokenId);
		}
	}
	return 0;
}

