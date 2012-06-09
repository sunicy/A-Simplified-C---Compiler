%{
#include <string.h>
#include "syntax.tab.h"
#include "gtree.h"

extern int error_type;
int yycolumn = 1;

#define YY_USER_ACTION yylloc.first_line=yylloc.last_line=yylineno; \
	yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1;\
	yycolumn += yyleng;

int strccount(char* s, char x)
{
	int count = 0;
	for (;*s; s++)
		if (s[0] == x)
			count++;
	return count;
}
%}

oct_number		0[0-7]+
hex_number		0x[0-9a-fA-F]+
exp_number		[0-9]+(\.[0-9]+)?[Ee][+-]?[0-9]+
float_number	[0-9]+\.|[0-9]*\.[0-9]+
int_number		[0-9]+
digit	[0-9]
letter	[_a-zA-Z]
whitespace [ 	] 

%option noyywrap
%option yylineno

%%

{exp_number}|{float_number}		{
									sscanf(yytext, "%f", &yylval.type_float);
									return FLOAT;
								}
{oct_number}	{
					sscanf(yytext + 1, "%o", &yylval.type_int);
					return INT;
				}
{hex_number}	{
					sscanf(yytext + 2, "%x", &yylval.type_int);
					return INT;
				}
{int_number}	{
					yylval.type_int = atoi(yytext);
					return INT;
				}
","		{return COMMA;}
";"		{return SEMI;}
"="		{return ASSIGNOP;}
"+"		{return PLUS;}
"-"		{return MINUS;}
"*"		{return STAR;}
"/"		{return DIV;}
"&&"	{return AND;}
"||"	{return OR;}
"."		{return DOT;}
"!"		{return NOT;}
"("		{return LP;}
")"		{return RP;}
"["		{return LB;}
"]"		{return RB;}
"{"		{return LC;}
"}"		{return RC;}
struct	{return STRUCT;}
return	{return RETURN;}
if		{return IF;}
else	{return ELSE;}
while	{return WHILE;}
int|float						{
									char* p = (char*)malloc(strlen(yytext) + 1);
									strcpy(p, yytext);
									yylval.type_str = p;
									return TYPE;
								}
">"|"<"|">="|"<="|"=="|"!="		{
									if (yytext[0] == '>')
									{
										if (yytext[1] == '=')
											yylval.type_rel = R_GE;
										else
											yylval.type_rel = R_GT;
									}
									else if (yytext[0] == '<')
									{
										if (yytext[1] == '=')
											yylval.type_rel = R_LE;
										else
											yylval.type_rel = R_LT;
									}
									else if (yytext[0] == '!')
										yylval.type_rel = R_NE;
									else
										yylval.type_rel = R_EQ;
									return RELOP;
								}
{letter}({letter}|{digit})*		{
									char* p = (char*)malloc(strlen(yytext) + 1);
									strcpy(p, yytext);
									yylval.type_str = p;
									return ID;
								}
\n								{yycolumn = 1;}
{whitespace}					{}
.								{printf("Error type %d at line %d: Mysterious character '%s'.\n", error_type = 1, yylineno, yytext);}
"/*"(.|\n)*"*/"					{
									if (strstr(yytext + 2, "/*") > 0)
										printf("Error type %d at line %d: comment error!\n", error_type = 3, yylineno - strccount(yytext, '\n'));
								}
	

%%


