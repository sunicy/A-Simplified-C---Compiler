#ifndef _GTREE_H_
#define _GTREE_H_

enum nodetype {
	NT_UNKNOWN, /* not specified yet */
	NT_INT, NT_FLOAT, NT_ID, NT_SEMI, NT_COMMA, NT_ASSIGNOP, NT_RELOP,
	NT_PLUS, NT_MINUS, NT_STAR, NT_DIV, NT_AND, NT_OR, NT_DOT, NT_NOT,
	NT_TYPE, NT_LP, NT_RP, NT_LB, NT_RB, NT_LC, NT_RC, NT_STRUCT,
	NT_RETURN, NT_IF, NT_ELSE, NT_WHILE, /* end of TOKENs */
	NT_Program, NT_ExtDefList, NT_ExtDef, NT_ExtDecList, NT_Specifier,
	NT_StructSpecifier, NT_OptTag, NT_Tag, NT_VarDec, NT_FunDec,
	NT_VarList, NT_ParamDec, NT_CompSt, NT_StmtList, NT_Stmt, NT_DefList,
	NT_Def, NT_DecList, NT_Dec, NT_Exp, NT_Args /* end of Non-terminals */
};

enum reducetype {
	RT_TERMINAL, 
	/* basic */
	RT_Program_ExtDefList, 
	RT_ExtDefList_ExtDef_ExtDefList, RT_ExtDefList_NULL, 
	RT_ExtDef_Specifier_ExtDecList_SEMI, RT_ExtDef_Specifier_SEMI, RT_ExtDef_Specifier_FunDec_CompSt, 
	RT_ExtDecList_VarDec, RT_ExtDecList_VarDec_COMMA_ExtDecList, 

	/* Specifiers */
	RT_Specifier_TYPE, RT_Specifier_StructSpecifier, 
	RT_StructSpecifier_STRUCT_OptTag_LC_DefList_RC, RT_StructSpecifier_STRUCT_Tag, 
	RT_OptTag_ID, RT_OptTag_NULL, 
	RT_Tag_ID, 

	/* Declarators */
	RT_VarDec_ID, RT_VarDec_VarDec_LB_INT_RB, 
	RT_FunDec_ID_LP_VarList_RP, RT_FunDec_ID_LP_RP, 
	RT_VarList_ParamDec_COMMA_VarList, RT_VarList_ParamDec, 
	RT_ParamDec_Specifier_VarDec, 

	/* Statements */
	RT_CompSt_LC_DefList_StmtList_RC, 
	RT_StmtList_Stmt_StmtList, RT_StmtList_NULL, 
	RT_Stmt_Exp_SEMI, RT_Stmt_CompSt, RT_Stmt_RETURN_Exp_SEMI, RT_Stmt_IF_LP_Exp_RP_Stmt, 
	RT_Stmt_IF_LP_Exp_RP_Stmt_ELSE_Stmt, RT_Stmt_WHILE_LP_Exp_RP_Stmt, RT_Stmt_SEMI, 

	/* Local definitions */
	RT_DefList_Def_DefList, RT_DefList_NULL, 
	RT_Def_Specifier_DecList_SEMI, 
	RT_DecList_Dec, RT_DecList_Dec_COMMA_DecList, 
	RT_Dec_VarDec, RT_Dec_VarDec_ASSIGNOP_Exp, 

	/* Expressions */
	RT_Exp_Exp_ASSIGNOP_Exp, RT_Exp_Exp_AND_Exp, RT_Exp_Exp_OR_Exp, RT_Exp_Exp_RELOP_Exp, RT_Exp_Exp_PLUS_Exp, RT_Exp_Exp_MINUS_Exp, RT_Exp_Exp_STAR_Exp, 
	RT_Exp_Exp_DIV_Exp, RT_Exp_LP_Exp_RP, RT_Exp_MINUS_Exp, RT_Exp_NOT_Exp, RT_Exp_ID_LP_Args_RP, RT_Exp_ID_LP_RP, RT_Exp_Exp_LB_Exp_RB, RT_Exp_Exp_DOT_ID, 
	RT_Exp_ID, RT_Exp_INT, RT_Exp_FLOAT, 
	RT_Args_Exp_COMMA_Args, RT_Args_Exp,

	RT_ExtDef_Specifier_FunDec_SEMI
};

/* relationship op! (5 - rel) = !(rel) */
enum relop {R_LT, R_GT, R_NE, R_EQ, R_LE, R_GE};
#define reverse_relop(r) (5 - (r))

/* What's the expected type, BOOL or INT ?
	used in Intermediate Code to determine
	whether a value-expr or a bool-stmt */
/* 	
	ET_VEXPR: a numeric value expr expected.(X,Y in X=Y; X RELOP Y)
	ET_BOOL: a BOOL expr expected.(NOT(X); X AND/OR Y; IF(X); WHILE(X))
*/
enum expr_type {ET_NUM, ET_BOOL}; 

enum var_complexity {
	VC_BASIC, VC_ADDR, VC_REL
};	/* is it a basic var, address or rel? mostly determined in ExpID*/

#define NT_FIRST_TOKEN NT_INT;
#define NT_LAST_TOKEN NT_WHILE
#define NT_FIRST_NON_TERMINAL NT_Program

/* below are for "lvalue" , 01, 10, 11*/
#define LV_UNKNOWN 0
#define LV_LVALUE 1
#define LV_RVALUE 2
#define LV_BIVALUE 3

union gnode_val {
	int		val_int;
	char*	val_str;
	float	val_float;
	enum relop val_rel;
	void*	val_void; /* nothing */
}; /* value if exists */

struct gnode {
	union gnode_val val; 		/* Of course it's the value! */
	enum nodetype type; 		/* specify what type the node is */
	char* type_name; 			/* er, just the name of the type */
	struct gnode *child, *sibling;
	int lineno; 				/* ...- -# */

	/* :: below are used for semantic analysis */
	enum reducetype rtype; 		/* rule used when reducing */
	/* syn: synchronized property; inh: inherited property */
	int lvalue;					/* syn: LV_XXX */ 
	union {
		struct tnode* vtype;	/* syn: value type */
		struct tfield_lst* field;/* syn: field-list type */ 
		struct tnode* return_type;/* used for Fun->CompSt*/
	};
	char* name;					/* syn: name if has an ID name */
	unsigned char struct_def;	/* inh: whether in struct define(non-0) */

	enum expr_type exptype;	/* inh/syn: whether a value expr */
	struct im_node *im_prev;	/* prev im_node(let's add after it!)*/
	struct im_node *lbl_true, *lbl_false; /* inh/syn true/F label */
	union {
		struct {
			struct im_operand *base_addr;/* current offset variable id */
		} comptype; /* complex type [array struct] */

		struct {
			union {
				struct im_operand* rvop;	/* result var op */
				int ival;		/* actual value */
				float fval;
			};
			int numeric;		/* if literal numeric, 0 for no */
		} inttype;
		struct {
			struct im_node* rel_node;/* ifgoto stmt */
		} cond; /* condition stmt */
	};
	enum var_complexity vcmplx;	/* is it a basic var, address or rel? */
};

typedef struct gnode* ptr_gnode;

/* allocate memory for new node */ 
ptr_gnode gt_new_node(char* type_name, int lineno, union gnode_val val,
						enum nodetype type, enum reducetype rtype);

/* append the current gnode to one gnode as a succ sibling */
/* CAUTION: not INSERT! Original succ sibling will be lost! */
ptr_gnode gt_append_sibling(ptr_gnode prev_sibling, ptr_gnode cur_node);

/* append the current gnode to one gnode as a Child */
ptr_gnode gt_append_child(ptr_gnode parent, ptr_gnode cur_node);

#endif
