%{
#include <stdio.h>
#include "lexical.c"
#include "gtree.h"
#include "std.h"

int error_type = 0;
static union gnode_val gval;

#define FORCE_OUTPUT 1

#define pm(x) printf("%s ( %d )\n", x, yylineno)

/* assert gtree */
#define _AG \
			if (error_type != 0) \
				break
/* _ANS is short for New a Nonterminal Symbol */
#define _NNS(symbol,rtype) \
	gt_new_node(#symbol, yyloc.first_line, gval, NT_##symbol,rtype)/*, printf("%s at Line %d\n", #symbol, yylineno)*/

/* _ATS is short for New a Terminal Symbol */
#define _NTS(symbol) \
	/*printf("SYMBOL: %s; VAL: %d\n", #symbol, gval.val_int), */gt_new_node(#symbol, yylineno, gval, NT_##symbol, RT_TERMINAL)

/* _AS is short for Append a Sibling */
#define _AS(p, cur) \
	gt_append_sibling(p, cur)

/* _AC is short for Append a Child */
#define _AC(p, cur) \
	gt_append_child(p, cur)

#define PRINT_TREE

%}

%locations

%union {
	int type_int;
	float type_float;
	char * type_str;
	int type_rel;
	struct gnode* gnode;
}

%type <gnode> Exp Args Dec DecList Def DefList Stmt CompSt ExtDef ExtDefList Program ExtDecList Specifier StructSpecifier
%type <gnode> OptTag Tag VarDec FunDec VarList ParamDec StmtList
%type <type_rel> RELOP

%nonassoc LOWEST
%token COMMA SEMI
%token LC RC
%token <type_int>INT <type_float>FLOAT <type_str>ID
%nonassoc SIMPLE_IF_STMT
%nonassoc ELSE
%token STRUCT RETURN IF WHILE
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left DIV STAR
%right NOT 
%nonassoc MINUS_NUM
%right <type_str>TYPE
%left LP RP LB RB DOT

%%

Program		:	ExtDefList 	{
								_AG;
								$$ =_NNS(Program,RT_Program_ExtDefList); 
								_AC($$,$1);
								if (!error_type)
								{
									#ifdef DEBUG
										gt_display($$,0);
									#endif
									gtroot = $$;
								}
								else
									gtroot = NULL;
							}
			|	ExtDefList error 	{
										$$ = NULL;
										printf("Error type %d at line %d: fatal error, is this the end of program?\n", error_type = 2, yylineno);
									}
			;
ExtDefList	:	ExtDef ExtDefList {_AG;$$ = _NNS(ExtDefList,RT_ExtDefList_ExtDef_ExtDefList); _AS(_AC($$,$1),$2);}
			|	{_AG;$$ = _NNS(ExtDefList,RT_ExtDefList_NULL);/*_AC($$,NULL);*/}
			;
ExtDef		:	Specifier ExtDecList SEMI {_AG;$$ = _NNS(ExtDef,RT_ExtDef_Specifier_ExtDecList_SEMI); _AS(_AS(_AC($$,$1),$2),_NTS(SEMI));}
			|	Specifier SEMI {_AG;_AG;$$ = _NNS(ExtDef,RT_ExtDef_Specifier_SEMI); _AS(_AC($$,$1),_NTS(SEMI));}
			|	Specifier FunDec CompSt {_AG;$$ = _NNS(ExtDef,RT_ExtDef_Specifier_FunDec_CompSt); _AS(_AS(_AC($$,$1),$2),$3);}
			|	Specifier FunDec SEMI {_AG;$$ = _NNS(ExtDef,RT_ExtDef_Specifier_FunDec_SEMI); _AS(_AS(_AC($$,$1),$2),_NTS(SEMI));}
			|	Specifier ExtDecList error {$$ = NULL; printf("Error type %d at line %d: ';' expected\n", error_type = 2, yylineno);}
			;
ExtDecList	:	VarDec {_AG;$$ = _NNS(ExtDecList,RT_ExtDecList_VarDec); _AC($$,$1);}
			|	VarDec COMMA ExtDecList {_AG;$$ =  _NNS(ExtDecList,RT_ExtDecList_VarDec_COMMA_ExtDecList); _AS(_AS(_AC($$,$1),_NTS(COMMA)),$3);}
			;

Specifier	:	TYPE	{
							_AG;$$ = _NNS(Specifier,RT_Specifier_TYPE); 
							gval.val_str = $1;
							_AC($$,_NTS(TYPE));
						}
			|	StructSpecifier {_AG;$$ = _NNS(Specifier,RT_Specifier_StructSpecifier); _AC($$,$1);}
			;
StructSpecifier:STRUCT OptTag LC DefList RC {_AG;$$ = _NNS(StructSpecifier,RT_StructSpecifier_STRUCT_OptTag_LC_DefList_RC); _AS(_AS(_AS(_AS(_AC($$,_NTS(STRUCT)),$2),_NTS(LC)),$4),_NTS(RC));}
			|	STRUCT Tag {_AG;$$ = _NNS(StructSpecifier,RT_StructSpecifier_STRUCT_Tag); _AS(_AC($$,_NTS(STRUCT)),$2);}
			;
OptTag		:	ID	{
						_AG;$$ = _NNS(OptTag,RT_OptTag_ID);
						gval.val_str = $1;
						_AC($$,_NTS(ID));
					}
			|	{_AG;$$ = _NNS(OptTag,RT_OptTag_NULL);_AC($$,NULL);}
			;
Tag			:	ID	{
						_AG;$$ = _NNS(Tag,RT_Tag_ID);
						gval.val_str = $1;
						_AC($$,_NTS(ID));
					}
			;

VarDec		:	ID 	{
						_AG;$$ = _NNS(VarDec,RT_VarDec_ID);
						gval.val_str = $1;
						_AC($$,_NTS(ID));
					}
			|	VarDec LB INT RB {
									_AG;$$ = _NNS(VarDec,RT_VarDec_VarDec_LB_INT_RB);
									gval.val_int = $3;
									_AS(_AS(_AS(_AC($$,$1),_NTS(LB)),_NTS(INT)),_NTS(RB));
								}
			;
FunDec		:	ID LP VarList RP {
									_AG;$$ = _NNS(FunDec,RT_FunDec_ID_LP_VarList_RP);
									gval.val_str = $1;
									_AS(_AS(_AS(_AC($$,_NTS(ID)),_NTS(LP)), $3),_NTS(RP));
								}
			|	ID LP RP	{
								_AG;$$ = _NNS(FunDec,RT_FunDec_ID_LP_RP);
								gval.val_str = $1;
								_AS(_AS(_AC($$,_NTS(ID)),_NTS(LP)),_NTS(RP));
							}
			;
VarList		:	ParamDec COMMA VarList {_AG;$$ = _NNS(VarList,RT_VarList_ParamDec_COMMA_VarList); _AS(_AS(_AC($$,$1),_NTS(COMMA)),$3);}
			|	ParamDec {_AG;$$ = _NNS(VarList,RT_VarList_ParamDec); _AC($$,$1);}
			;
ParamDec	:	Specifier VarDec {_AG;$$ = _NNS(ParamDec,RT_ParamDec_Specifier_VarDec); _AS(_AC($$,$1),$2);}
			;


CompSt		:	LC DefList StmtList RC {_AG;$$ = _NNS(CompSt,RT_CompSt_LC_DefList_StmtList_RC); _AS(_AS(_AS(_AC($$,_NTS(LC)),$2),$3),_NTS(RC));}
			;
StmtList	:	Stmt StmtList {_AG;$$ = _NNS(StmtList,RT_StmtList_Stmt_StmtList); _AS(_AC($$,$1),$2);}
			|	{_AG;$$ = _NNS(StmtList,RT_StmtList_NULL); /*_AC($$,NULL);*/}
			;
Stmt		:	Exp SEMI {_AG;$$ =_NNS(Stmt,RT_Stmt_Exp_SEMI); _AS(_AC($$,$1),_NTS(SEMI));}
			|	CompSt {_AG;$$ = _NNS(Stmt,RT_Stmt_CompSt); _AC($$,$1);}
			|	RETURN Exp SEMI {_AG;$$ = _NNS(Stmt,RT_Stmt_RETURN_Exp_SEMI); _AS(_AS(_AC($$,_NTS(RETURN)),$2),_NTS(SEMI));}
			|	IF LP Exp RP Stmt %prec SIMPLE_IF_STMT {_AG;$$ = _NNS(Stmt,RT_Stmt_IF_LP_Exp_RP_Stmt); _AS(_AS(_AS(_AS(_AC($$,_NTS(IF)),_NTS(LP)),$3),_NTS(RP)),$5);}
			|	IF LP Exp RP Stmt ELSE Stmt {_AG;$$ = _NNS(Stmt,RT_Stmt_IF_LP_Exp_RP_Stmt_ELSE_Stmt); _AS(_AS(_AS(_AS(_AS(_AS(_AC($$,_NTS(IF)),_NTS(LP)),$3),_NTS(RP)),$5),_NTS(ELSE)),$7);}
			|	WHILE LP Exp RP Stmt {_AG;$$ = _NNS(Stmt,RT_Stmt_WHILE_LP_Exp_RP_Stmt); _AS(_AS(_AS(_AS(_AC($$,_NTS(WHILE)),_NTS(LP)),$3),_NTS(RP)),$5);}
			|	SEMI {_AG;$$ = _NNS(Stmt,RT_Stmt_SEMI); _AC($$,_NTS(SEMI));}
			|	error SEMI {$$ = NULL;printf("Error type %d at line %d: Syntax error.\n", error_type = 2, yylineno);}
			|	Exp error {$$ = NULL;printf("Error type %d at line %d: (';' expected ?) unexpected '%s'.\n", error_type = 2, yylineno, yytext);yyerrok;} %prec LOWEST
			;

DefList		:	Def DefList {_AG;$$ = _NNS(DefList,RT_DefList_Def_DefList); _AS(_AC($$,$1),$2);}
			|	{_AG;$$ = _NNS(DefList,RT_DefList_NULL);/*_AC($$,NULL);*/}
			;
Def			:	Specifier DecList SEMI {_AG;$$ = _NNS(Def,RT_Def_Specifier_DecList_SEMI); _AS(_AS(_AC($$,$1),$2),_NTS(SEMI));}
			|	Specifier DecList error {$$ = NULL; printf("Error type %d at line %d: ';' expected\n", error_type = 2, yylineno);}
			;
DecList		:	Dec {_AG;$$ = _NNS(DecList,RT_DecList_Dec); _AC($$,$1);}
			|	Dec COMMA DecList {_AG;$$ = _NNS(DecList,RT_DecList_Dec_COMMA_DecList); _AS(_AS(_AC($$,$1),_NTS(COMMA)),$3);}
			;
Dec			:	VarDec {_AG;$$ = _NNS(Dec,RT_Dec_VarDec); _AC($$,$1);}
			|	VarDec ASSIGNOP Exp {_AG;$$ = _NNS(Dec,RT_Dec_VarDec_ASSIGNOP_Exp); _AS(_AS(_AC($$,$1),_NTS(ASSIGNOP)),$3);}
			;

Exp			:	Exp ASSIGNOP Exp {_AG;$$ = _NNS(Exp,RT_Exp_Exp_ASSIGNOP_Exp); _AS(_AS(_AC($$,$1),_NTS(ASSIGNOP)),$3);}
			|	Exp AND Exp {_AG;$$ = _NNS(Exp,RT_Exp_Exp_AND_Exp); _AS(_AS(_AC($$,$1),_NTS(AND)),$3);}
			|	Exp OR Exp {_AG;$$ = _NNS(Exp,RT_Exp_Exp_OR_Exp); _AS(_AS(_AC($$,$1),_NTS(OR)),$3);}
			|	Exp RELOP Exp {_AG;$$ = _NNS(Exp,RT_Exp_Exp_RELOP_Exp); gval.val_rel = $2; _AS(_AS(_AC($$,$1),_NTS(RELOP)),$3);}
			|	Exp PLUS Exp {_AG;$$ = _NNS(Exp,RT_Exp_Exp_PLUS_Exp); _AS(_AS(_AC($$,$1),_NTS(PLUS)),$3);}
			|	Exp MINUS Exp  {_AG;$$ = _NNS(Exp,RT_Exp_Exp_MINUS_Exp); _AS(_AS(_AC($$,$1),_NTS(MINUS)),$3);}
			| 	Exp STAR Exp {_AG;$$ = _NNS(Exp,RT_Exp_Exp_STAR_Exp); _AS(_AS(_AC($$,$1),_NTS(STAR)),$3);}
			|	Exp DIV Exp {_AG;$$ = _NNS(Exp,RT_Exp_Exp_DIV_Exp); _AS(_AS(_AC($$,$1),_NTS(DIV)),$3);}
			|	LP Exp RP {_AG;$$ = _NNS(Exp,RT_Exp_LP_Exp_RP); _AS(_AS(_AC($$,_NTS(LP)),$2),_NTS(RP));}
			|	MINUS Exp {_AG;$$ = _NNS(Exp,RT_Exp_MINUS_Exp); _AS(_AC($$,_NTS(MINUS)),$2);} %prec MINUS_NUM
			|	NOT Exp {_AG;$$ = _NNS(Exp,RT_Exp_NOT_Exp); _AS(_AC($$,_NTS(NOT)),$2);}
			|	ID LP Args RP	{ 
									_AG;$$ = _NNS(Exp,RT_Exp_ID_LP_Args_RP);
									gval.val_str = $1;
									_AS(_AS(_AS(_AC($$,_NTS(ID)),_NTS(LP)),$3),_NTS(RP));
								}
			|	ID LP RP 	{
								_AG;$$ = _NNS(Exp,RT_Exp_ID_LP_RP);
								gval.val_str = $1;
								_AS(_AS(_AC($$,_NTS(ID)),_NTS(LP)),_NTS(RP));
							}
			|	Exp LB Exp RB {_AG;$$ = _NNS(Exp,RT_Exp_Exp_LB_Exp_RB); _AS(_AS(_AS(_AC($$,$1),_NTS(LB)),$3),_NTS(RB));}
			|	Exp DOT ID	{
								_AG;$$ = _NNS(Exp,RT_Exp_Exp_DOT_ID);
								gval.val_str = $3;
								_AS(_AS(_AC($$,$1),_NTS(DOT)),_NTS(ID));
							}
			|	ID {
					gval.val_str = $1;
					_AG;$$ = _NNS(Exp,RT_Exp_ID);
					_AC($$,_NTS(ID));
				}
			|	INT {
					gval.val_int = $1;
					_AG;$$ = _NNS(Exp,RT_Exp_INT);
					_AC($$,_NTS(INT));
				}
			|	FLOAT {
					gval.val_float = $1;
					_AG;$$ = _NNS(Exp,RT_Exp_FLOAT);
					_AC($$,_NTS(FLOAT));
				}
			|	Exp LB error {
					$$ = NULL;
					printf("Error type %d at line %d: parentheses error\n", error_type = 2, yylineno);
				}
			;
Args		:	Exp COMMA Args {_AG;$$ = _NNS(Args,RT_Args_Exp_COMMA_Args); _AS(_AS(_AC($$,$1),_NTS(COMMA)),$3);}
			|	Exp	{_AG;$$ = _NNS(Args,RT_Args_Exp); _AC($$,$1);}
			;
%%

yyerror(char* msg)
{
}
