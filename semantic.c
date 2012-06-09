/* Created on Apr. 27th, 2012 by Sunicy Tao */

/* This file aims to do the semantic analysis,
	with symbol table and grammer tree

	the module is prefixed with SN_
*/

#include <stdlib.h>
#include <stdio.h>
#include "semantic.h"
#include "stable.h"
#include "gtree.h"
#include "std.h"

/* DEBUGING */
//#define DEBUG


void print_null(char* x, ...)
{
}

/* a dozen of children ~ hoho */
#define child1(p) (p->child)
#define child2(p) (child1(p)->sibling)
#define child3(p) (child2(p)->sibling)
#define child4(p) (child3(p)->sibling)
#define child5(p) (child4(p)->sibling)
#define child6(p) (child5(p)->sibling)
#define child7(p) (child6(p)->sibling)

/* BASIC TYPE tnodes */
static struct tnode* tnode_int/* = new_tnode_basic(TK_INT)*/;
static struct tnode* tnode_float/* = new_tnode_basic(TK_FLOAT)*/;
static struct tnode* tnode_error/* = new_tnode(TK_ERROR)*/;

static int layer = 0;

/* a bunch of defines for defining var/fun */
#define semantic_err(errno) \
	( \
		print_err((errno),cur->lineno,(errno##_MSG)), \
		cur->vtype = tnode_error \
	) \

#define define_new_var(name,type,errno,errmsg) \
	if (find_snode_cur_scope((name),SK_VAR)) \
	{ \
		print_err((errno),cur->lineno,(errmsg)); \
		cur->vtype = tnode_error; \
	} \
	else \
		add_snode((name), SK_VAR, (type))



/* does semantic analysis recursively */
/* REMEMBER: always create snode when the top-level entry is finished */
static void sn_parse(struct gnode* cur, struct gnode* p)
{
	struct snode* n;
	struct tfield_lst* f;
	layer++;
	//printd("# [rt:%d]%s LAYER: %d [Line %d]\n", cur->rtype, cur->type_name, layer, cur->lineno);
	switch (cur->rtype)
	{
		case RT_Program_ExtDefList:
			
			new_scope(); /* well, we should have at least ONE scope~ */
			/* define READ and WRITE functions */
				add_snode("read" , SK_VAR, 
					new_tnode_func(new_func(tnode_int, NULL, FUNC_DEFINED)));
				add_snode("write" , SK_VAR, 
					new_tnode_func(new_func(tnode_int, new_fieldlst_link(tnode_int, "x", NULL), FUNC_DEFINED)));
			/* end of definition */
			sn_parse(child1(cur), cur);
			/* check functions that declared only */
			n = cur_scope();
			while (n = n->down)
				if (n->kind == SK_VAR && n->type->kind == TK_FUNC &&
					n->type->function->defined == FUNC_UNDEFINED)
					printf("Error type %d: Function '%s' cannot be resolved\n", SNERR_FUNC_DEC_UNDEFINED, n->name), set_error_status();
			del_current_scope();
			break;
		case RT_ExtDefList_ExtDef_ExtDefList:
			sn_parse(child1(cur), cur);
			sn_parse(child2(cur), cur);
			break;
		case RT_ExtDefList_NULL:
			break;
		case RT_ExtDef_Specifier_ExtDecList_SEMI:
			sn_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype;
			sn_parse(child2(cur), cur);
			break;
		case RT_ExtDef_Specifier_SEMI:
			sn_parse(child1(cur), cur);
			/*cur->vtype = child1(cur)->vtype;*/
			break;
		case RT_ExtDef_Specifier_FunDec_CompSt:
			new_scope(); /* have fun yourself! */
			sn_parse(child1(cur), cur);
			cur->return_type = child1(cur)->vtype;
			sn_parse(child2(cur), cur);
			if (child2(cur)->name != NULL && child2(cur)->vtype != tnode_error &&
						child1(cur)->vtype != tnode_error) /* if succ */
				if (n = find_snode(child2(cur)->name, SK_VAR)) /* multi defined */
				{
					if (n->type->function->defined)
						semantic_err(SNERR_FUNC_MULTIDEFINED);
					else if (typecmp(n->type->function->return_type, child1(cur)->return_type) != 0 ||
						fieldcmp(n->type->function->param, child2(cur)->field) != 0)
					{
						/* delete the old wrong declaration and create a new one */
						del_func(n->type->function);
						add_snode_parent(child2(cur)->name, SK_VAR, 
							new_tnode_func(new_func(child1(cur)->return_type, child2(cur)->field, FUNC_DEFINED)));
						semantic_err(SNERR_FUNC_DEC_DEF_MISMATCH);
					}
					else
						n->type->function->defined = FUNC_DEFINED;
				}
				else
				/* build up the snode! function isn't defined in CURRENT SCOPE!*/
					add_snode_parent(child2(cur)->name, SK_VAR, 
						new_tnode_func(new_func(child1(cur)->return_type, child2(cur)->field, FUNC_DEFINED)));
			sn_parse(child3(cur), cur);
			del_current_scope(); /* fun's over */

			break;
		case RT_ExtDef_Specifier_FunDec_SEMI:
			new_scope(); /* have fun yourself! */
			sn_parse(child1(cur), cur);
			cur->return_type = child1(cur)->vtype;
			sn_parse(child2(cur), cur);
			if (child2(cur)->name != NULL && child2(cur)->vtype != tnode_error &&
						child1(cur)->vtype != tnode_error) /* if succ */
				if (n = find_snode(child2(cur)->name, SK_VAR)) 
				{
					if (typecmp(n->type->function->return_type, child1(cur)->return_type) != 0 ||
						fieldcmp(n->type->function->param, child2(cur)->field) != 0)
						semantic_err(SNERR_FUNC_DEC_DEF_MISMATCH);
					else
						n->type->function->defined = FUNC_DEFINED;
				}
				else
				/* build up the snode! function isn't defined in CURRENT SCOPE!*/
					add_snode_parent(child2(cur)->name, SK_VAR, 
						new_tnode_func(new_func(child1(cur)->return_type, child2(cur)->field, FUNC_UNDEFINED)));
			del_current_scope(); /* fun's over */

			break;
		case RT_ExtDecList_VarDec:
			cur->vtype = p->vtype; /* inherits from its parent */
			sn_parse(child1(cur), cur);
			printd("\tdefine: var-name=%s\n", child1(cur)->name);
			define_new_var(child1(cur)->name,cur->vtype,SNERR_VAR_MULTIDEFINED,"Variable multidefined");

			break;
		case RT_ExtDecList_VarDec_COMMA_ExtDecList:
			cur->vtype = p->vtype;
			sn_parse(child1(cur), cur);
			printd("\tdefine: var-name=%s\n", child1(cur)->name);
			define_new_var(child1(cur)->name,cur->vtype,SNERR_VAR_MULTIDEFINED,"Variable multidefined");
			sn_parse(child3(cur), cur);
			
			break;
		case RT_Specifier_TYPE: /* CAUTION: it's a terminal node */
			if (strcmp(child1(cur)->val.val_str, "int") == 0)
				cur->vtype = tnode_int;
			else if (strcmp(child1(cur)->val.val_str, "float") == 0)
				cur->vtype = tnode_float;

			break;
		case RT_Specifier_StructSpecifier:
			sn_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype; /* inherits from StructSpecifier */
	
			break;
		case RT_StructSpecifier_STRUCT_OptTag_LC_DefList_RC:
			sn_parse(child2(cur), cur);
			new_scope(); /* start new fun */
			cur->struct_def = 1; /* now we are in struct definition, 
									it should be inherited in DefList */
			sn_parse(child4(cur), cur);
			f = create_fieldlst_from_cur_scope();

			del_current_scope(); /* fun's over */
			cur->vtype = new_tnode_struct(f);
			if (child2(cur)->name != NULL) /* if the struct is tagged, ok, add it as snode! */
			{
				if (find_snode_cur_scope(child2(cur)->name, SK_DEF))
				{
					print_err(SNERR_STRUCT_MULTIDEFINED, cur->lineno, "Struct name conflicts");
					cur->vtype = tnode_error;
				}
				else
					add_snode(child2(cur)->name, SK_DEF, cur->vtype);
			}

			break;
		case RT_StructSpecifier_STRUCT_Tag:
			sn_parse(child2(cur), cur);
			if (n = find_snode(child2(cur)->name, SK_DEF)) /* if exists */
				cur->vtype = n->type;
			else /* oops, let's have error output! */
			{
				print_err(SNERR_STRUCT_UNDEFINED, cur->lineno, "Struct undefined");
				cur->vtype = tnode_error;
			}

			break;
		case RT_OptTag_ID:
			cur->name = child1(cur)->val.val_str; /* get ID! */
			break;
		case RT_OptTag_NULL:
			cur->name = NULL; /* oops, no ID */
			break;
		case RT_Tag_ID:
			cur->name = child1(cur)->val.val_str; /* get ID! */
			break;
		case RT_VarDec_ID:
			cur->name = child1(cur)->val.val_str;
			cur->vtype = p->vtype;
			break;
		case RT_VarDec_VarDec_LB_INT_RB:
			cur->vtype = new_tnode_arr_by_tnode(p->vtype, child3(cur)->val.val_int);
			sn_parse(child1(cur), cur);
			cur->name = child1(cur)->name;
			cur->vtype = child1(cur)->vtype;

			break;
		case RT_FunDec_ID_LP_VarList_RP:
			cur->name = child1(cur)->val.val_str;
			sn_parse(child3(cur), cur);
			cur->field = child3(cur)->field; /* syn: field-lst */
			f = cur->field;

			break;
		case RT_FunDec_ID_LP_RP:
			cur->name = child1(cur)->val.val_str;
			cur->field = NULL; /* well, no list at all */
			break;
		case RT_VarList_ParamDec_COMMA_VarList:
			sn_parse(child1(cur), cur);
			sn_parse(child3(cur), cur);
			/* TODO: Error detection[vtype OK?] */
			cur->field = new_fieldlst_link(child1(cur)->vtype, child1(cur)->name, child3(cur)->field);
			
			break;
		case RT_VarList_ParamDec:
			sn_parse(child1(cur), cur);
			/* TODO : judge it! */
			//if (child1(cur)->vtype->kind != TK_ERROR)
				cur->field = new_fieldlst_link(child1(cur)->vtype, child1(cur)->name, NULL); /* tail */
			/*else
				cur->vtype = tnode_error;*/
			break;
		case RT_ParamDec_Specifier_VarDec:
			/*TODO Error detection*/
			sn_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype;
			sn_parse(child2(cur), cur);
			cur->name = child2(cur)->name;
			cur->vtype = child2(cur)->vtype;
			if (find_snode_cur_scope(cur->name, SK_VAR))
			{
				print_err(SNERR_VAR_MULTIDEFINED, cur->lineno, "Variable multidefined");
				cur->vtype = tnode_error;
			}
			else
				add_snode(cur->name, SK_VAR, cur->vtype);

			break;
		case RT_CompSt_LC_DefList_StmtList_RC:
			cur->return_type = p->return_type; /* have to know the return type! */
			if (p->rtype != RT_ExtDef_Specifier_FunDec_CompSt) /* CAUTION! function hdr has the same scope of CompSt */
				new_scope(); /* yes! another scope */
			sn_parse(child2(cur), cur);
			sn_parse(child3(cur), cur);
			if (p->rtype != RT_ExtDef_Specifier_FunDec_CompSt) /* CAUTION! function hdr has the same scope of CompSt */
				del_current_scope(); /* oops, nevermind */

			break;
		case RT_StmtList_Stmt_StmtList:
			cur->return_type = p->return_type; /* have to know the return type! */
			sn_parse(child1(cur), cur);
			sn_parse(child2(cur), cur);
			break;
		case RT_StmtList_NULL:
			break;
		case RT_Stmt_Exp_SEMI:
			cur->return_type = p->return_type; /* have to know the return type! */
			sn_parse(child1(cur), cur);
			break;
		case RT_Stmt_CompSt:
			cur->return_type = p->return_type;
			sn_parse(child1(cur), cur);
			break;
		case RT_Stmt_RETURN_Exp_SEMI:
			cur->return_type = p->return_type;
			sn_parse(child2(cur), cur);
			if (typecmp(cur->return_type, child2(cur)->vtype)) /* if types unmatched */
				semantic_err(SNERR_RETURN_TYPE_UNMATCH);
			break;
		case RT_Stmt_IF_LP_Exp_RP_Stmt:
			cur->return_type = p->return_type;
			sn_parse(child3(cur), cur);
			sn_parse(child5(cur), cur);
			if (typecmp(tnode_int, child3(cur)->vtype)) /* if condition type isn't int */
				semantic_err(SNERR_COND_WRONG_TYPE);
			break;
		case RT_Stmt_IF_LP_Exp_RP_Stmt_ELSE_Stmt:
			cur->return_type = p->return_type;
			sn_parse(child3(cur), cur);
			sn_parse(child5(cur), cur);
			sn_parse(child7(cur), cur);
			if (typecmp(tnode_int, child3(cur)->vtype)) /* if condition type isn't int */
				semantic_err(SNERR_COND_WRONG_TYPE);
			break;
		case RT_Stmt_WHILE_LP_Exp_RP_Stmt:
			cur->return_type = p->return_type;
			sn_parse(child3(cur), cur);
			sn_parse(child5(cur), cur);
			if (typecmp(tnode_int, child3(cur)->vtype)) /* if condition type isn't int */
				semantic_err(SNERR_COND_WRONG_TYPE);
			break;
		case RT_DefList_Def_DefList:
			cur->struct_def = p->struct_def;
			sn_parse(child1(cur), cur);
			sn_parse(child2(cur), cur);
			break;
		case RT_DefList_NULL:
			break;
		case RT_Def_Specifier_DecList_SEMI:
			cur->struct_def = p->struct_def;
			sn_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype;
			sn_parse(child2(cur), cur);
			break;
		case RT_DecList_Dec:
			cur->struct_def = p->struct_def;
			cur->vtype = p->vtype;
			sn_parse(child1(cur), cur);
			cur->name = child1(cur)->name;
			if (child1(cur)->vtype != tnode_error)
				if (find_snode_cur_scope(cur->name, SK_VAR) != NULL)
				{
					if (cur->struct_def)
						semantic_err(SNERR_INVALID_DEF_IN_STRUCT);
					else
						semantic_err(SNERR_VAR_MULTIDEFINED);
				}
				else 
					add_snode(cur->name, SK_VAR, child1(cur)->vtype);
			break;
		case RT_DecList_Dec_COMMA_DecList:
			cur->struct_def = p->struct_def;
			cur->vtype = p->vtype;
			sn_parse(child1(cur), cur);
			cur->name = child1(cur)->name;
			/* define the var */
			if (child1(cur)->vtype->kind != TK_ERROR)
				if (find_snode_cur_scope(cur->name, SK_VAR))
				{
					if (cur->struct_def)
						semantic_err(SNERR_INVALID_DEF_IN_STRUCT);
					else
						semantic_err(SNERR_VAR_MULTIDEFINED);
				}
				else
					add_snode(cur->name, SK_VAR, child1(cur)->vtype);
			sn_parse(child3(cur), cur);

			break;
		case RT_Dec_VarDec:
			cur->vtype = p->vtype;
			sn_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype;
			cur->name = child1(cur)->name;
			break;
		case RT_Dec_VarDec_ASSIGNOP_Exp:
			cur->struct_def = p->struct_def;
			if (cur->struct_def) /* forbidden in struct declaration */
			{
				semantic_err(SNERR_INVALID_DEF_IN_STRUCT);
				break;
			}
			cur->vtype = p->vtype;
			sn_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype;
			cur->name = child1(cur)->name;
			sn_parse(child3(cur), cur);
			if (typecmp(cur->vtype, child3(cur)->vtype))
				semantic_err(SNERR_ASSIGN_TYPE_UNMATCH);
			break;
		case RT_Exp_Exp_ASSIGNOP_Exp:
			sn_parse(child1(cur), cur);
			sn_parse(child3(cur), cur);
			/* ignore current error */
			if (child1(cur)->vtype->kind == TK_ERROR ||
				child3(cur)->vtype->kind == TK_ERROR)
			{
				printd("X=Y error\n");
				cur->vtype = tnode_error;
				break;
			}

			/* left-value? */
			if (!(child1(cur)->lvalue & LV_LVALUE)) /* CAUTION: can an exp be only left-valued? */
				semantic_err(SNERR_RVALUE_BEFORE_ASSIGNOP);
			/* type unmatched? */
			if (typecmp(child1(cur)->vtype, child3(cur)->vtype))
				semantic_err(SNERR_ASSIGN_TYPE_UNMATCH);

			/* if nothing wrong, set cur */
			if (cur->vtype != tnode_error)
			{
				cur->lvalue = child1(cur)->lvalue;
				cur->vtype = child1(cur)->vtype;
			}

			break;
#define Exp_Exp_Op_Exp \
	sn_parse(child1(cur), cur); \
	sn_parse(child3(cur), cur); \
	/* type unmatched? */ \
	if (child1(cur)->vtype->kind > LAST_BASIC_TYPE || \
		child3(cur)->vtype->kind > LAST_BASIC_TYPE ||\
		child3(cur)->vtype->kind != child1(cur)->vtype->kind) \
	{ \
		semantic_err(SNERR_TYPE_UNMATCH); \
		break; \
	} \
	else \
		(cur->lvalue = LV_RVALUE,  \
		cur->vtype = child1(cur)->vtype);

		case RT_Exp_Exp_AND_Exp:
			Exp_Exp_Op_Exp
			break;
		case RT_Exp_Exp_OR_Exp:
			Exp_Exp_Op_Exp
			break;
		case RT_Exp_Exp_RELOP_Exp:
			Exp_Exp_Op_Exp
			break;
		case RT_Exp_Exp_PLUS_Exp:
			Exp_Exp_Op_Exp
			break;
		case RT_Exp_Exp_MINUS_Exp:
			Exp_Exp_Op_Exp
			break;
		case RT_Exp_Exp_STAR_Exp:
			Exp_Exp_Op_Exp
			break;
		case RT_Exp_Exp_DIV_Exp:
			Exp_Exp_Op_Exp
			break;
		case RT_Exp_LP_Exp_RP:
			sn_parse(child2(cur), cur);
			cur->lvalue = child2(cur)->lvalue;
			cur->vtype = child2(cur)->vtype;
			break;
		case RT_Exp_MINUS_Exp:
			sn_parse(child2(cur), cur);
			cur->lvalue = LV_RVALUE;
			cur->vtype = child2(cur)->vtype;
			break;
		case RT_Exp_NOT_Exp:
			sn_parse(child2(cur), cur);
			cur->lvalue = LV_RVALUE;
			cur->vtype = child2(cur)->vtype;
			break;
		case RT_Exp_ID_LP_Args_RP:
			/* get ID and check it */
			cur->name = child1(cur)->val.val_str;
			if (!(n = find_snode(cur->name, SK_VAR)))
				semantic_err(SNERR_FUNC_UNDEFINED);
			else if (n->type->kind != TK_FUNC)
				semantic_err(SNERR_USE_VAR_AS_FUNC);
			else
			{
				/* pass the function type on! */
				cur->field = n->type->function->param;
				sn_parse(child3(cur), cur);
				/* args invalid! */
				if (child3(cur)->vtype->kind == TK_ERROR)
					semantic_err(SNERR_FUNC_ARG_UNMATCH);
				else
				{
					/* it's all correct */
					cur->lvalue = LV_RVALUE;
					cur->vtype = n->type->function->return_type;
				}
			}
			break;
		case RT_Exp_ID_LP_RP:
			/* get ID and check it */
			cur->name = child1(cur)->val.val_str;
			if (!(n = find_snode(cur->name, SK_VAR)))
				semantic_err(SNERR_FUNC_UNDEFINED);
			else if (n->type->kind != TK_FUNC)
				semantic_err(SNERR_USE_VAR_AS_FUNC);
			else
			{
				/* it's all correct */
				cur->lvalue = LV_RVALUE;
				cur->vtype = n->type->function->return_type;
			}
			break;
		case RT_Exp_Exp_LB_Exp_RB:
			sn_parse(child1(cur), cur);
			sn_parse(child3(cur), cur);
			cur->vtype = child1(cur)->vtype;
			if (cur->vtype->kind != TK_ERROR) /* if error, ignore */
				if (cur->vtype->kind != TK_ARRAY)
					semantic_err(SNERR_ARRAY_TYPE_REQUIRED);
				else if (child3(cur)->vtype->kind != TK_INT)
					semantic_err(SNERR_INVALID_ARRAY_INDEX);
				else
				{
					/* all is well */
					cur->vtype = child1(cur)->vtype->array.type; /* set cur type as elem of the array */
					/* only non-array type can be an lvalue */
					if (cur->vtype->kind == TK_ARRAY)
						cur->lvalue = LV_RVALUE;
					else
						cur->lvalue = LV_LVALUE;
				}
			break;
		case RT_Exp_Exp_DOT_ID:
			sn_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype;
			if (cur->vtype->kind != TK_ERROR) /* if error, ignore */
				if (cur->vtype->kind != TK_STRUCT)
					semantic_err(SNERR_USE_VAR_AS_STRUCT);
				if ((f = find_field(cur->vtype->field_lst, child3(cur)->val.val_str)) == NULL)
					semantic_err(SNERR_INVALID_STRUCT_SCOPE);
				else
				{
					/* OK */
					cur->vtype = f->type;
					cur->lvalue = (cur->vtype->kind == TK_ARRAY) ? LV_RVALUE : LV_LVALUE;
				}
			break;
		case RT_Exp_ID:
			/* Almost nonsense */
			cur->name = cur->val.val_str;
			cur->lvalue = LV_LVALUE; /* if exists */
			if(n = find_snode(cur->name, SK_VAR)) /* if found in stable */
				cur->vtype = n->type;
			else
				semantic_err(SNERR_VAR_UNDEFINED);
			break;
		case RT_Exp_INT:
			cur->vtype = tnode_int;
			cur->lvalue = LV_RVALUE;
			break;
		case RT_Exp_FLOAT:
			cur->vtype = tnode_float;
			cur->lvalue = LV_RVALUE;
			break;
		case RT_Args_Exp_COMMA_Args:
			sn_parse(child1(cur), cur);
			cur->field = p->field; /* get the field, compare them one by one */
			if (cur->field == NULL) /* Oops, too many argments */
				semantic_err(SNERR_FUNC_ARG_UNMATCH);
			else if (typecmp(cur->field->type, child1(cur)->vtype))
				/* wrong type */
				semantic_err(SNERR_FUNC_ARG_UNMATCH);
			else
			{
				/* It's OK, pass on to args */
				cur->field = cur->field->next;
				sn_parse(child3(cur), cur);
				cur->field = p->field; /* restore it! otherwise it could be null */
			}
			break;
		case RT_Args_Exp:
			sn_parse(child1(cur), cur);
			cur->field = p->field; /* get the field, compare them one by one */
			if (cur->field == NULL) /* Oops, too many argments */
				semantic_err(SNERR_FUNC_ARG_UNMATCH);
			if (cur->field->next != NULL) /* too few argments */
				semantic_err(SNERR_FUNC_ARG_UNMATCH);
			else if (typecmp(cur->field->type, child1(cur)->vtype))
				/* wrong type */
				semantic_err(SNERR_FUNC_ARG_UNMATCH);
			break;
	}
	layer--;
}

/* public: start it! */
int semantic_parse()
{
	tnode_int = new_tnode_basic(TK_INT);
	tnode_float = new_tnode_basic(TK_FLOAT);
	tnode_error = new_tnode(TK_ERROR);
	sn_parse(gtroot, NULL);
}
