/* Created on May. 24th, 2012 by Sunicy Tao */

/* This file aims to generate basic intermediate code,
	based on "semantic.c" with symbol table and grammer tree

	This module repeats what "semantic.c" has done before, 
	and basically depends on the assumption that "NO SEMANTIC
	ERROR" exists.

	the module is prefixed with im_
*/

#include <stdlib.h>
#include <stdio.h>
#include "std.h"
#include "semantic.h"
#include "interm.h"
#include "stable.h"
#include "gtree.h"
#include "imnode.h"

/* DEBUGING */
//#define DEBUG

#ifdef DEBUG
	#define printd printf
#else
	#define printd print_null
#endif

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
static struct im_operand* iop_zero; /* #0 */
static struct im_operand* iop_one;  /* #1 */

static int layer = 0;

/* a bunch of defines for defining var/fun */

#define define_new_var(name,type,errno,errmsg) \
	add_snode((name), SK_VAR, (type))

#define set_im_prev(dst,src) \
		((dst)->im_prev = (src)->im_prev)
#define bool_expr() (cur->exptype = ET_BOOL)
#define num_expr() (cur->exptype = ET_NUM)
#define set_expr_type_as_parent() (cur->exptype = p->exptype)

#define cur_ival cur->inttype.ival
#define exp1_ival child1(cur)->inttype.ival
#define exp2_ival child2(cur)->inttype.ival
#define exp3_ival child3(cur)->inttype.ival
#define cur_numeric cur->inttype.numeric
#define exp1_numeric child1(cur)->inttype.numeric
#define exp2_numeric child2(cur)->inttype.numeric
#define exp3_numeric child3(cur)->inttype.numeric
#define exp_op(p) (((p)->vcmplx == VC_ADDR) ? (new_star_operand((p)->comptype.base_addr->vid)) : \
		(((p)->inttype.numeric) ? new_int_const_operand((p)->inttype.ival) : (p)->inttype.rvop))

/* judge relationship expression */
int judge_rel_expr(int x, int y, enum relop r)
{
	switch (r)
	{
		case R_GT: return x > y;
		case R_LT: return x < y;
		case R_LE: return x <= y;
		case R_GE: return x >= y;
		case R_EQ: return x == y;
		case R_NE: return x != y;
	}
	return FALSE;
}

void do_assign(struct gnode* lc, struct gnode *rc)
{
	struct im_operand *rop, *lop;
	struct snode* n;
	/* assign left op! */
	if (lc->vtype->kind > LAST_BASIC_TYPE) /* struct! */
	{
		//TODO
	}
	else
	{
		/* get right-operand */
		rop = exp_op(rc);
		lop = exp_op(lc);
		im_ins_tail(new_imnode_assign(lop, rop));
	}
}

/* assess and convert expr_type at the very end */
void assess_and_convert_expr_type(struct gnode* p, struct gnode* cur)
{
	struct im_node* n;
	struct im_operand* rop;
	if (cur->exptype == p->exptype)
		return ; /* already OK! */
	if (p->exptype == ET_NUM) /* bool => num! */
	{
		/* TODO : not consider it yet!!!!! 
		IF ?? GOTO labelX
		tY := 0
		LABEL label X:
		tY = 1
		*/
		/*rop = new_var_operand(new_vid());
		im_ins_tail(new_imnode_ifgoto(cur->inttype.rvop, cur->val.val_rel, iop_zero, false_lid));
		im_ins_tail(new_imnode_assign(rop, iop_zero));
		im_ins_tail(false_lid);
		im_ins_tail(new_imnode_assign(rop, iop_one));
		cur->inttype.rvop = rop; yes, rop is now the result! */
		
	}
	else	/* num => bool! */
	{
		/* 
		IF Exp == 0 GOTO labelFalse
		...
		LABEL labelFalse:

		or

		IF Exp != 0 GOTO labelTrue
		...
		LABEL labelTrue

		or

		IF Exp == 0 GOTO labelTrue
		GOTO labelFalse
		*/
		if (!cur->inttype.numeric)
		{
			if (!p->lbl_false) /* if only true is available! */
				im_ins_tail(new_imnode_ifgoto(cur->inttype.rvop, iop_zero, R_NE, p->lbl_true));
			else if (!p->lbl_true)/* well, only false one is available*/
				im_ins_tail(new_imnode_ifgoto(cur->inttype.rvop, iop_zero, R_EQ, p->lbl_false));
			else /* both are available! */
			{
				im_ins_tail(new_imnode_ifgoto(cur->inttype.rvop, iop_zero, R_NE, p->lbl_true));
				im_ins_tail(new_imnode_goto(p->lbl_false));
			}
		}
		else /* determine now! */
		{
			if ((cur->inttype.ival == 0) ^ (p->lbl_true != NULL)) /* always goto! */
				im_ins_tail(new_imnode_goto(((cur->inttype.ival) ? p->lbl_true : p->lbl_false)));
		}
		set_expr_type_as_parent();/* now it's alright! */
	}
}

/* decide whether to exchange the property from num to bool */
void expr_num_to_bool(struct gnode* cur, struct gnode* p)
{
	if (p->exptype == ET_BOOL && cur->vtype == tnode_int) /* we need a bool! */
	{
		if (cur_numeric) /* well, a numeric! */
		{
			if (cur_ival)
			{
				if (cur->lbl_true)
					im_ins_tail(new_imnode_goto(cur->lbl_true));
			}
			else
			{
				if (cur->lbl_false)
					im_ins_tail(new_imnode_goto(cur->lbl_false));
			}
		}
		else
		{
			/* an expression */
			/*
				if t1 == 0 goto false
				goto true
			*/
			if (cur->lbl_false && cur->lbl_true)
			{
				im_ins_tail(new_imnode_ifgoto(exp_op(cur), iop_zero, R_NE, cur->lbl_true));
				im_ins_tail(new_imnode_goto(cur->lbl_false));
			}
			else if (cur->lbl_false)
				im_ins_tail(new_imnode_ifgoto(exp_op(cur), iop_zero, R_EQ, cur->lbl_false));
			else if (cur->lbl_true)
				im_ins_tail(new_imnode_ifgoto(exp_op(cur), iop_zero, R_NE, cur->lbl_true));
		}
	}
}

/* does semantic analysis recursively */
/* REMEMBER: always create snode when the top-level entry is finished */
static void im_parse(struct gnode* cur, struct gnode* p)
{
	struct snode* n;
	struct tfield_lst* f;
	struct im_node *in, *in2;
	struct im_operand *top, *rop, *lop; /* tmp operand */
	int rvid, tvid;
	layer++;
	printd("# [rt:%d]%s LAYER: %d [Line %d]\n", cur->rtype, cur->type_name, layer, cur->lineno);

	/* initialize the exp */
	if (cur->type == NT_Exp) /* inherites from the parent! */
	{
		cur->exptype = p->exptype;
	}

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
			im_parse(child1(cur), cur);
			del_current_scope();
			break;
		case RT_ExtDefList_ExtDef_ExtDefList:
			
			im_parse(child1(cur), cur);
			im_parse(child2(cur), cur);
			break;
		case RT_ExtDefList_NULL:
			break;
		case RT_ExtDef_Specifier_ExtDecList_SEMI:
			
			im_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype;
			im_parse(child2(cur), cur);
			break;
		case RT_ExtDef_Specifier_SEMI:
			
			im_parse(child1(cur), cur);
			/*cur->vtype = child1(cur)->vtype;*/
			break;
		case RT_ExtDef_Specifier_FunDec_CompSt:
			
			new_scope(); /* have fun yourself! */
			im_parse(child1(cur), cur);
			cur->return_type = child1(cur)->vtype;
			im_ins_tail(new_imnode_func_label(child2(cur)->name));
			im_parse(child2(cur), cur);
			if (child2(cur)->name != NULL)
				if (n = find_snode(child2(cur)->name, SK_VAR)) /* if already declared */
					n->type->function->defined = FUNC_DEFINED;
				else
					/* build up the snode! function isn't defined in CURRENT SCOPE!*/
					add_snode_parent(child2(cur)->name, SK_VAR, 
						new_tnode_func(new_func(child1(cur)->return_type, child2(cur)->field, FUNC_DEFINED)));
			im_parse(child3(cur), cur);
			del_current_scope(); /* fun's over */

			break;
		case RT_ExtDef_Specifier_FunDec_SEMI:
			
			new_scope(); /* have fun yourself! */
			im_parse(child1(cur), cur);
			cur->return_type = child1(cur)->vtype;
			im_parse(child2(cur), cur);
			if (child2(cur)->name != NULL)
				if (n = find_snode(child2(cur)->name, SK_VAR)) 
					n->type->function->defined = FUNC_DEFINED;
				else
					/* build up the snode! function isn't defined in CURRENT SCOPE!*/
					add_snode_parent(child2(cur)->name, SK_VAR, 
						new_tnode_func(new_func(child1(cur)->return_type, child2(cur)->field, FUNC_UNDEFINED)));
			del_current_scope(); /* fun's over */

			break;
		case RT_ExtDecList_VarDec:
			
			cur->vtype = p->vtype; /* inherits from its parent */
			im_parse(child1(cur), cur);
			printd("\tdefine: var-name=%s\n", child1(cur)->name);
			set_snode_vid(add_snode(child1(cur)->name, SK_VAR, cur->vtype),
						new_vid(),  /* allocate a new vid */
						VT_CONCRETE); /* it's a concrete variable */

			break;
		case RT_ExtDecList_VarDec_COMMA_ExtDecList:
			
			cur->vtype = p->vtype;
			im_parse(child1(cur), cur);
			printd("\tdefine: var-name=%s\n", child1(cur)->name);
			set_snode_vid(add_snode(child1(cur)->name, SK_VAR, cur->vtype),
						new_vid(),  /* allocate a new vid */
						VT_CONCRETE); /* it's a concrete variable */
			im_parse(child3(cur), cur);
			
			break;
		case RT_Specifier_TYPE: /* CAUTION: it's a terminal node */
			
			if (strcmp(child1(cur)->val.val_str, "int") == 0)
				cur->vtype = tnode_int;
			else if (strcmp(child1(cur)->val.val_str, "float") == 0)
				cur->vtype = tnode_float;

			break;
		case RT_Specifier_StructSpecifier:
			
			im_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype; /* inherits from StructSpecifier */
	
			break;
		case RT_StructSpecifier_STRUCT_OptTag_LC_DefList_RC:
			
			im_parse(child2(cur), cur);
			new_scope(); /* start new fun */
			cur->struct_def = 1; /* now we are in struct definition, 
									it should be inherited in DefList */
			im_parse(child4(cur), cur);
			f = create_fieldlst_from_cur_scope();

			del_current_scope(); /* fun's over */
			cur->vtype = new_tnode_struct(f);
			if (child2(cur)->name != NULL) /* if the struct is tagged, ok, add it as snode! */
				add_snode(child2(cur)->name, SK_DEF, cur->vtype);

			break;
		case RT_StructSpecifier_STRUCT_Tag:
			
			im_parse(child2(cur), cur);
			n = find_snode(child2(cur)->name, SK_DEF);
			cur->vtype = n->type;

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
			im_parse(child1(cur), cur);
			cur->name = child1(cur)->name;
			cur->vtype = child1(cur)->vtype;

			break;
		case RT_FunDec_ID_LP_VarList_RP:
			
			cur->name = child1(cur)->val.val_str;
			im_parse(child3(cur), cur);
			cur->field = child3(cur)->field; /* syn: field-lst */
			f = cur->field;

			break;
		case RT_FunDec_ID_LP_RP:
			
			cur->name = child1(cur)->val.val_str;
			cur->field = NULL; /* well, no list at all */
			break;
		case RT_VarList_ParamDec_COMMA_VarList:
		
			im_parse(child1(cur), cur);
			im_parse(child3(cur), cur);
			cur->field = new_fieldlst_link(child1(cur)->vtype, child1(cur)->name, child3(cur)->field);
			
			break;
		case RT_VarList_ParamDec:
			
			im_parse(child1(cur), cur);
			/* TODO : judge it! */
			cur->field = new_fieldlst_link(child1(cur)->vtype, child1(cur)->name, NULL); /* tail */
			break;
		case RT_ParamDec_Specifier_VarDec:
			
			im_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype;
			im_parse(child2(cur), cur);
			cur->name = child2(cur)->name;
			cur->vtype = child2(cur)->vtype;
			set_snode_vid(n = add_snode(cur->name, SK_VAR, cur->vtype),  /* create an snode */
							new_pvid(),  /* allocate a new parameter vid */
							/* it's a concrete variable or a ptr? basic=>concrete*/
							(cur->vtype->kind <= LAST_BASIC_TYPE) ? VT_CONCRETE : VT_PTR); 
			if (cur->vtype->kind > LAST_BASIC_TYPE) /* complex */
				im_ins_tail(new_imnode_param(new_var_operand(get_snode_vid(n))));
			else /* basic */
				im_ins_tail(new_imnode_param(new_var_operand(get_snode_vid(n))));
			
			break;
		case RT_CompSt_LC_DefList_StmtList_RC:
			
			cur->return_type = p->return_type; /* have to know the return type! */
			if (p->rtype != RT_ExtDef_Specifier_FunDec_CompSt) /* CAUTION! function hdr has the same scope of CompSt */
				new_scope(); /* yes! another scope */
			im_parse(child2(cur), cur);
			im_parse(child3(cur), cur);
			if (p->rtype != RT_ExtDef_Specifier_FunDec_CompSt) /* CAUTION! function hdr has the same scope of CompSt */
				del_current_scope(); /* oops, nevermind */

			break;
		case RT_StmtList_Stmt_StmtList:
			
			cur->return_type = p->return_type; /* have to know the return type! */
			im_parse(child1(cur), cur);
			im_parse(child2(cur), cur);
			break;
		case RT_StmtList_NULL:
			break;
		case RT_Stmt_Exp_SEMI:
			
			cur->return_type = p->return_type; /* have to know the return type! */
			im_parse(child1(cur), cur);
			break;
		case RT_Stmt_CompSt:
			
			cur->return_type = p->return_type;
			im_parse(child1(cur), cur);
			break;
		case RT_Stmt_RETURN_Exp_SEMI:
			cur->exptype = ET_NUM;
			
			cur->return_type = p->return_type;
			im_parse(child2(cur), cur);
			im_ins_tail(new_imnode_return(exp_op(child2(cur)))); /* RETURN! */
			break;
		case RT_Stmt_IF_LP_Exp_RP_Stmt:
			
			cur->exptype = ET_BOOL;
			child3(cur)->lbl_true = cur->lbl_true = new_imnode_label(new_lid());
			child3(cur)->lbl_false = cur->lbl_false = new_imnode_label(new_lid());
			
			cur->return_type = p->return_type;
			im_parse(child3(cur), cur);
			im_ins_tail(cur->lbl_true);
			im_parse(child5(cur), cur);
			im_ins_tail(cur->lbl_false);
			break;
		case RT_Stmt_IF_LP_Exp_RP_Stmt_ELSE_Stmt:
			
			cur->exptype = ET_BOOL;
			child3(cur)->lbl_true = cur->lbl_true = new_imnode_label(new_lid());
			child3(cur)->lbl_false = cur->lbl_false = new_imnode_label(new_lid());
			in = new_imnode_label(new_lid()); /* tmp label! */

			cur->return_type = p->return_type;
			im_parse(child3(cur), cur);
			im_ins_tail(cur->lbl_true);
			im_parse(child5(cur), cur);
			im_ins_tail(new_imnode_goto(in));
			im_ins_tail(cur->lbl_false);
			im_parse(child7(cur), cur);
			im_ins_tail(in);
			break;
		case RT_Stmt_WHILE_LP_Exp_RP_Stmt:
			
			cur->exptype = ET_BOOL;
			cur->lbl_true = new_imnode_label(new_lid());
			child3(cur)->lbl_false = cur->lbl_false = new_imnode_label(new_lid());
			child3(cur)->lbl_true = NULL; /* just move on! */

			cur->return_type = p->return_type;
			im_ins_tail(cur->lbl_true);
			im_parse(child3(cur), cur);
			im_parse(child5(cur), cur);
			im_ins_tail(new_imnode_goto(cur->lbl_true));
			im_ins_tail(cur->lbl_false);
			break;
		case RT_DefList_Def_DefList:
			
			cur->struct_def = p->struct_def;
			im_parse(child1(cur), cur);
			im_parse(child2(cur), cur);
			break;
		case RT_DefList_NULL:
			break;
		case RT_Def_Specifier_DecList_SEMI:
			
			cur->struct_def = p->struct_def;
			im_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype;
			im_parse(child2(cur), cur);
			break;
		case RT_DecList_Dec:
			
			cur->struct_def = p->struct_def;
			cur->vtype = p->vtype;
			im_parse(child1(cur), cur);
			cur->name = child1(cur)->name;
			break;
		case RT_DecList_Dec_COMMA_DecList:
			
			cur->struct_def = p->struct_def;
			cur->vtype = p->vtype;
			im_parse(child1(cur), cur);
			cur->name = child1(cur)->name;
			im_parse(child3(cur), cur);
			break;
		case RT_Dec_VarDec:
			
			cur->vtype = p->vtype;
			im_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype;
			cur->name = child1(cur)->name;
			/* define the var */
			set_snode_vid(n = add_snode(cur->name, SK_VAR, cur->vtype),  /* create an snode */
							new_vid(),  /* allocate a new vid */
							VT_CONCRETE); /* it's a concrete variable */
			if (!p->struct_def && cur->vtype->kind > LAST_BASIC_TYPE) /* It's a struct definition */
				im_ins_tail(new_imnode_dec(new_var_operand(get_snode_vid(n)), n->type->width));
			break;
		case RT_Dec_VarDec_ASSIGNOP_Exp:
			cur->exptype = ET_NUM;
			cur->struct_def = p->struct_def;
			cur->vtype = p->vtype;
			im_parse(child1(cur), cur);
			cur->vtype = child1(cur)->vtype;
			cur->name = child1(cur)->name;
			set_snode_vid(n = add_snode(cur->name, SK_VAR, cur->vtype),  /* create an snode */
							new_vid(),  /* allocate a new vid */
							VT_CONCRETE); /* it's a concrete variable */
			im_parse(child3(cur), cur);
			child1(cur)->vcmplx = (cur->vtype->kind <= LAST_BASIC_TYPE) ? VC_BASIC : VC_ADDR;
			if (child1(cur)->vcmplx == VC_BASIC)
			{
				exp1_numeric = FALSE;
				child1(cur)->inttype.rvop = new_var_operand(get_snode_vid(n));
			}
			do_assign(child1(cur), child3(cur));
			cur->inttype = child1(cur)->inttype; /* syn property */
			expr_num_to_bool(cur, p);

			break;
		case RT_Exp_Exp_ASSIGNOP_Exp:
			cur->exptype = ET_NUM;
			im_parse(child1(cur), cur);
			im_parse(child3(cur), cur);

			/* set cur */
			cur->lvalue = child1(cur)->lvalue;
			cur->vtype = child1(cur)->vtype;

			do_assign(child1(cur), child3(cur));
			cur->inttype = child1(cur)->inttype; /* syn property */
			expr_num_to_bool(cur, p);
			break;

			/* assign left op! */
			if (child1(cur)->vtype->kind > LAST_BASIC_TYPE) /* struct! */
			{
				//TODO
			}
			else
			{
				/* get right-operand */
				rop = exp_op(child3(cur));
				lop = exp_op(child1(cur));
				if (child1(cur)->rtype == RT_Exp_ID &&
					rop->otype != OT_CONST_INT) /* x=EXPRESSION, it's really but not most basic */
				{
					/* we dont need to generate a new expr, cause it already exists */
					n = find_snode(child1(cur)->name, SK_VAR);
					/* refresh symbol table (var-link) */
					set_snode_vid(n, rop->vid, get_snode_var_type(n));
				}
				else
					im_ins_tail(new_imnode_assign(lop, rop));
			}

			break;
#define Exp_Exp_Op_Exp \
	im_parse(child1(cur), cur); \
	im_parse(child3(cur), cur); \
	(cur->lvalue = LV_RVALUE,  \
	cur->vtype = child1(cur)->vtype);

		case RT_Exp_Exp_AND_Exp:
			if (p->exptype == ET_BOOL)
			{
				child1(cur)->lbl_true = NULL; /* fall! */
				child1(cur)->lbl_false = cur->lbl_false;
				child3(cur)->lbl_true = cur->lbl_true;
				child3(cur)->lbl_false = cur->lbl_false;
				if (cur->lbl_false == NULL)
				{
					in = new_imnode_label(new_lid());
					child1(cur)->lbl_false = in;
					child3(cur)->lbl_false = in;
				}
				else
					in = NULL;
					
				cur->exptype = ET_BOOL; /* we need bool expr! */
		
				Exp_Exp_Op_Exp
				if (in) /* new label~ */
					im_ins_tail(in);
				cur->vcmplx = VC_REL;
			}
			else /* p.exptype = ET_NUM */
			{
				cur->vcmplx = VC_BASIC; /* it's numeric! */
				in = new_imnode_label(new_lid());
				in2 = new_imnode_label(new_lid());
				child1(cur)->lbl_true = NULL;
				child1(cur)->lbl_false = in;
				cur->exptype = ET_BOOL; /* we need bool expr! */

				im_parse(child1(cur), cur); /* first child */
				cur->vtype = child1(cur)->vtype; /* GET the VTYPE!! */
				
				if (exp1_numeric) /* first child is numeric! */
				{
					cur_numeric = TRUE; /* yes! numeric! */
					if (exp1_ival) /* it's false! just false all! */
						cur_ival = 0;
					else /* it's true, just consider the rest part */
						cur_ival = 1; /* nothing todo */

				}
				if (!(exp1_numeric && cur_ival == 0)) /* well, NOT false for all */
				{
					child3(cur)->lbl_true = NULL;
					child3(cur)->lbl_false = in;

					im_parse(child3(cur), cur);

					if (exp3_numeric) /* third child is actually a constant! */
					{
						cur_numeric = TRUE; /* of course we can decide it! */
						cur_ival = (exp3_ival != 0);
					}

					/* decide whether there's a need to build up in and in2 */
					if (!exp1_numeric || !exp3_numeric) /* well, it's not numeric! */
					{
						/*
							if xxx goto FALSE
							t1 = 1
							goto END
							FALSE:
							t1=0
							END:
						*/
						cur_numeric = FALSE;
						im_ins_tail(new_imnode_assign(
								cur->inttype.rvop = new_var_operand(new_vid()), 
								iop_one)); /* t1 = 1 */
						im_ins_tail(new_imnode_goto(in2)); /* goto END */
						im_ins_tail(new_imnode_label(in->label.lid));/* false label */
						im_ins_tail(new_imnode_assign(cur->inttype.rvop, iop_zero));
						im_ins_tail(new_imnode_label(in2->label.lid)); /* LABEL END */
					}
				}
			}

			break;
		case RT_Exp_Exp_OR_Exp:
			if (p->exptype == ET_BOOL)
			{
				child1(cur)->lbl_true = cur->lbl_true;
				child1(cur)->lbl_false = NULL; /* fall! */
				child3(cur)->lbl_true = cur->lbl_true;
				child3(cur)->lbl_false = cur->lbl_false;
				if (cur->lbl_true == NULL)
				{
					in = new_imnode_label(new_lid());
					child1(cur)->lbl_true = in;
					child3(cur)->lbl_true = in;
				}
				else
					in = NULL;

				cur->exptype = ET_BOOL; /* we need bool expr! */
				
				Exp_Exp_Op_Exp

				if (in) /* new label~ */
					im_ins_tail(in);
				cur->vcmplx = VC_REL;
			}
			else /* p.exptype = ET_NUM */
			{
				cur->vcmplx = VC_BASIC; /* it's numeric! */
				in = new_imnode_label(new_lid());
				in2 = new_imnode_label(new_lid());
				child1(cur)->lbl_false = NULL;
				child1(cur)->lbl_true = in;
				cur->exptype = ET_BOOL; /* we need bool expr! */

				im_parse(child1(cur), cur); /* first child */
				cur->vtype = child1(cur)->vtype; /* GET the VTYPE!! */
				
				if (exp1_numeric) /* first child is numeric! */
				{
					cur_numeric = TRUE; /* yes! numeric! */
					if (exp1_ival) /* it's true! just true all! */
						cur_ival = 1;
					else /* it's true, just consider the rest part */
						cur_ival = 0; /* nothing todo */

				}
				if (!(exp1_numeric && cur_ival == 1)) /* well, NOT true for all */
				{
					child3(cur)->lbl_false = NULL;
					child3(cur)->lbl_true = in;

					im_parse(child3(cur), cur);

					if (exp3_numeric) /* third child is actually a constant! */
					{
						cur_numeric = TRUE; /* of course we can decide it! */
						cur_ival = (exp3_ival != 0);
					}

					/* decide whether there's a need to build up in and in2 */
					if (!exp1_numeric || !exp3_numeric) /* well, it's not numeric! */
					{
						/*
							if xxx goto TRUE
							t1 = 0
							goto END
							TRUE:
							t1=1
							END:
						*/
						cur_numeric = FALSE;
						im_ins_tail(new_imnode_assign(
								cur->inttype.rvop = new_var_operand(new_vid()), 
								iop_zero)); /* t1 = 0 */
						im_ins_tail(new_imnode_goto(in2)); /* goto END */
						im_ins_tail(new_imnode_label(in->label.lid));/* true label */
						im_ins_tail(new_imnode_assign(cur->inttype.rvop, iop_one));
						im_ins_tail(new_imnode_label(in2->label.lid)); /* LABEL END */
					}
				}
			}

			break;
		case RT_Exp_Exp_RELOP_Exp:
			cur->exptype = ET_NUM;
			Exp_Exp_Op_Exp

			if (exp1_numeric && exp3_numeric)
			{
				int result = judge_rel_expr(exp1_ival, 
						exp3_ival, child2(cur)->val.val_rel);
				if (p->exptype == ET_NUM)
				{
					cur->inttype.numeric = TRUE;
					cur->inttype.ival = result;
					cur->vcmplx = VC_BASIC;
				}
				else /* it's a cond expr */
				{
					if (cur->lbl_true && result) /* if we can goto true */
						im_ins_tail(new_imnode_goto(cur->lbl_true));
					else if (cur->lbl_false && !result)
						im_ins_tail(new_imnode_goto(cur->lbl_false));
					cur->vcmplx = VC_REL;
				}
				break;
			}
			
			if (p->exptype == ET_BOOL)
			{
				cur->vcmplx = VC_REL;
				if (cur->lbl_true && cur->lbl_false) /* both are here */
				{
					printd("#1 child3=%d\n", child3(cur)->inttype.ival);
					cur->cond.rel_node = new_imnode_ifgoto(exp_op(child1(cur)), 
						exp_op(child3(cur)), child2(cur)->val.val_rel, cur->lbl_true);
					im_ins_tail(cur->cond.rel_node);
					im_ins_tail(new_imnode_goto(cur->lbl_false));
				}
				else if (cur->lbl_true) /* only true */
				{
					printd("#2 true=%x; child1=%d\n", cur->lbl_true, child3(cur)->inttype.ival);
					cur->cond.rel_node = new_imnode_ifgoto(exp_op(child1(cur)), 
						exp_op(child3(cur)), child2(cur)->val.val_rel, cur->lbl_true);
					im_ins_tail(cur->cond.rel_node);
				}
				else if (cur->lbl_false) /* only false */
				{
					printd("#3 false=%x; child3=%d\n", cur->lbl_false, child3(cur)->inttype.ival);
					cur->cond.rel_node = new_imnode_ifgoto(exp_op(child1(cur)), 
						exp_op(child3(cur)), reverse_relop(child2(cur)->val.val_rel), cur->lbl_false);
					im_ins_tail(cur->cond.rel_node);
				}
			}
			else
			{
				/* TODO BOOL to NUM, not now!! */
				cur->vcmplx = VC_BASIC;
				cur->inttype.numeric = FALSE;
				in = new_imnode_label(new_lid());
				in2 = new_imnode_label(new_lid());
				im_ins_tail(new_imnode_ifgoto(exp_op(child1(cur)), 
						exp_op(child3(cur)), child2(cur)->val.val_rel, in));
				cur->inttype.rvop = new_var_operand(new_vid());
				im_ins_tail(new_imnode_assign(cur->inttype.rvop, iop_zero));
				im_ins_tail(new_imnode_goto(in2));
				im_ins_tail(in); /* add label */
				im_ins_tail(new_imnode_assign(cur->inttype.rvop, iop_one));
				im_ins_tail(in2); /* add label */
			}

			//assess_and_convert_expr_type(p, cur);
			break;
		case RT_Exp_Exp_PLUS_Exp:
			cur->exptype = ET_NUM;
			Exp_Exp_Op_Exp
			cur->vcmplx = VC_BASIC;
			if (exp1_numeric && exp3_numeric)
			{
				cur_numeric = TRUE;
				cur_ival = exp1_ival + exp3_ival;
			}
			else /* ok, we need some im_node */
			{
				cur_numeric = FALSE;
				im_ins_tail(new_imnode_add(cur->inttype.rvop = new_var_operand(new_vid()), 
							exp_op(child1(cur)),
							exp_op(child3(cur))));
			}
			expr_num_to_bool(cur, p);
			break;
		case RT_Exp_Exp_MINUS_Exp:
			cur->exptype = ET_NUM;
			Exp_Exp_Op_Exp
			cur->vcmplx = VC_BASIC;
			if (exp1_numeric && exp3_numeric)
			{
				cur_numeric = TRUE;
				cur_ival = exp1_ival - exp3_ival;
				break;
			}
			else /* ok, we need some im_node */
			{
				cur_numeric = FALSE;
				im_ins_tail(new_imnode_sub(cur->inttype.rvop = new_var_operand(new_vid()), 
							exp_op(child1(cur)),
							exp_op(child3(cur))));
			}
			
			expr_num_to_bool(cur, p);
			break;
		case RT_Exp_Exp_STAR_Exp:
			cur->exptype = ET_NUM;
			Exp_Exp_Op_Exp
			cur->vcmplx = VC_BASIC;
			if (exp1_numeric && exp3_numeric)
			{
				cur_numeric = TRUE;
				cur_ival = exp1_ival * exp3_ival;
				break;
			}
			else /* ok, we need some im_node */
			{
				cur_numeric = FALSE;
				im_ins_tail(new_imnode_mul(cur->inttype.rvop = new_var_operand(new_vid()), 
							exp_op(child1(cur)),
							exp_op(child3(cur))));
			}
			expr_num_to_bool(cur, p);
			break;
		case RT_Exp_Exp_DIV_Exp:
			cur->exptype = ET_NUM;
			Exp_Exp_Op_Exp
			if (exp1_numeric && exp3_numeric)
			{
				cur_numeric = TRUE;
				cur_ival = exp1_ival / exp3_ival;
				break;
			}
			else /* ok, we need some im_node */
			{
				cur_numeric = FALSE;
				im_ins_tail(new_imnode_div(cur->inttype.rvop = new_var_operand(new_vid()), 
							exp_op(child1(cur)),
							exp_op(child3(cur))));
			}
			expr_num_to_bool(cur, p);
			break;
		case RT_Exp_LP_Exp_RP:
			/* just inherits! */
			child2(cur)->lbl_true = cur->lbl_true;
			child2(cur)->lbl_false = cur->lbl_false;
			im_parse(child2(cur), cur);

			/* OK, COPY the union in gtree!!!! since inttype is the biggest! */
			cur->inttype = child2(cur)->inttype;
			/* OK, copying finished */

			cur->lvalue = child2(cur)->lvalue;
			cur->vtype = child2(cur)->vtype;
			cur->vcmplx = child2(cur)->vcmplx;
			break;
		case RT_Exp_MINUS_Exp:
			cur->exptype = ET_NUM;
			cur->lvalue = LV_RVALUE;
			cur->vtype = child2(cur)->vtype;
			im_parse(child2(cur), cur);
			if (exp2_numeric)
			{
				cur_numeric = TRUE;
				cur_ival = -exp2_ival;
			}
			else
			{
				cur_numeric = FALSE;
				im_ins_tail(new_imnode_sub(cur->inttype.rvop = new_var_operand(new_vid()),
						iop_zero, exp_op(child2(cur))));
			}
			cur->vcmplx = VC_BASIC;
			expr_num_to_bool(cur, p);
			break;
		case RT_Exp_NOT_Exp:
			/*TODO*/
			cur->exptype = ET_BOOL;
			if (p->exptype == ET_BOOL)
			{
				/* reverse the TRUE/FALSE */
				child2(cur)->lbl_true = cur->lbl_false;
				child2(cur)->lbl_false = cur->lbl_true;
				im_parse(child2(cur), cur);
				cur->lvalue = LV_RVALUE;
				cur->vtype = child2(cur)->vtype;
				cur->vcmplx = VC_REL;
			}
			else /* otherwise: NUM needed! */
			{
				if (exp2_numeric) /* if numeric! */
				{
					cur->vcmplx = VC_BASIC;
					cur_numeric = TRUE;
					cur_ival = exp2_ival;
				}
				else
				{
					/*
						if xx goto false
							t1 = 1
							goto end
						false:
							t1 = 0
						end:
					*/
					in = new_imnode_label(new_lid());
					in2 = new_imnode_label(new_lid());
					child2(cur)->lbl_true = NULL;
					child2(cur)->lbl_false = cur->lbl_true;
					
					im_parse(child2(cur), cur);

					cur_numeric = FALSE;
					im_ins_tail(new_imnode_assign(
							cur->inttype.rvop = new_var_operand(new_vid()), 
							iop_zero)); /* t1 = 0 */
					im_ins_tail(new_imnode_goto(in2)); /* goto END */
					im_ins_tail(new_imnode_label(in->label.lid));/* true label */
					im_ins_tail(new_imnode_assign(cur->inttype.rvop, iop_one));
					im_ins_tail(new_imnode_label(in2->label.lid)); /* LABEL END */
					cur->vtype = child2(cur)->vtype;
					cur->vcmplx = VC_REL;
				}
			}
			break;
		case RT_Exp_ID_LP_Args_RP:
			cur->exptype = ET_NUM;
			
			/* get ID and check it */
			cur->name = child1(cur)->val.val_str;
			if (strcmp(cur->name, "write") == 0) /* if it's WRITE! */
			{
				im_parse(child3(cur), cur);
				break;
			}
			n = find_snode(cur->name, SK_VAR);
			/* pass the function type on! */
			cur->field = n->type->function->param;
			im_parse(child3(cur), cur);
			/* args valid! */
			cur->lvalue = LV_RVALUE;
			cur->vtype = n->type->function->return_type;
			/* add CALL node! */
			im_ins_tail(new_imnode_call(cur->inttype.rvop = new_var_operand(new_vid()),
					cur->name));
			cur->vcmplx = VC_BASIC;
			expr_num_to_bool(cur, p);
			break;
		case RT_Exp_ID_LP_RP:
			cur->exptype = ET_NUM;
			
			/* get ID and check it */
			cur->name = child1(cur)->val.val_str;
			if (strcmp(cur->name, "read") == 0)
			{
				im_ins_tail(new_imnode_read(cur->inttype.rvop = new_var_operand(new_vid())));
				break;
			}
			n = find_snode(cur->name, SK_VAR);
			im_ins_tail(new_imnode_call(cur->inttype.rvop = new_var_operand(new_vid()), 
						cur->name));
			cur->inttype.numeric = FALSE; /* never be a constant */
			cur->lvalue = LV_RVALUE;
			cur->vtype = n->type->function->return_type;
			cur->vcmplx = VC_BASIC;
			expr_num_to_bool(cur, p);
			break;
		case RT_Exp_Exp_LB_Exp_RB:
			cur->exptype = ET_NUM;
			im_parse(child1(cur), cur);
			set_im_prev(cur, child1(cur));
			im_parse(child3(cur), cur);
			cur->vtype = child1(cur)->vtype;
			if (!child3(cur)->inttype.numeric) /* if it isn't numeric */
				/* t1 := x * width */
				im_ins_tail(new_imnode_mul(top = new_var_operand(new_vid()), /* new result */
									child3(cur)->inttype.rvop, /* x */
									new_int_const_operand(cur->vtype->array.type->width)));	/* width */
			else
				top = new_int_const_operand(child3(cur)->inttype.ival * cur->vtype->array.type->width);
				printd("i:%d; width:%d\n", child3(cur)->inttype.ival, cur->vtype->array.type->width);


			/* figure out the basic offset */
			/* t2 := &a + t1 */
			im_ins_tail(new_imnode_add(rop = new_var_operand(new_vid()),	/* new result */
									child1(cur)->comptype.base_addr, 		/* old offset */
									top));									/* new offset */

			cur->comptype.base_addr = rop; /* result vid! */
				
			/* all is well */
			cur->vtype = child1(cur)->vtype->array.type; /* set cur type as elem of the array */
			/* only non-array type can be an lvalue */
			if (cur->vtype->kind == TK_ARRAY)
				cur->lvalue = LV_RVALUE;
			else
				cur->lvalue = LV_LVALUE;

			cur->vcmplx = child1(cur)->vcmplx;
			expr_num_to_bool(cur, p);
			break;
		case RT_Exp_Exp_DOT_ID:
			cur->exptype = ET_NUM;
			im_parse(child1(cur), cur);
			set_im_prev(cur, child1(cur));
			cur->vtype = child1(cur)->vtype;
			f = find_field(cur->vtype->field_lst, child3(cur)->val.val_str);
			im_ins_tail(new_imnode_add(rop = new_var_operand(new_vid()),	/* new result */
									child1(cur)->comptype.base_addr, 		/* old offset */
									new_int_const_operand(f->offset)));		/* new offset */
			cur->comptype.base_addr = rop;
			cur->vcmplx = child1(cur)->vcmplx;
			/* OK */
			cur->vtype = f->type;
			cur->lvalue = (cur->vtype->kind == TK_ARRAY) ? LV_RVALUE : LV_LVALUE;
			expr_num_to_bool(cur, p);
			break;
		case RT_Exp_ID:
			cur->exptype = ET_NUM;
			
			/* Almost nonsense */
			cur->name = cur->val.val_str;
			cur->lvalue = LV_LVALUE; /* if exists */
			n = find_snode(cur->name, SK_VAR); /* found in stable */
			if (n->type->kind > LAST_BASIC_TYPE) /* if a complex var */
			{
				if (n->cur_v.vtype == VT_CONCRETE)
					cur->comptype.base_addr = new_addr_operand(n->cur_v.vid);
				else
					cur->comptype.base_addr = new_var_operand(n->cur_v.vid);
				cur->vcmplx = VC_ADDR; /* we'll use Filed base_addr */
			}
			else
			{
				cur->inttype.numeric = FALSE;
				cur->inttype.rvop = new_var_operand(n->cur_v.vid);
				cur->vcmplx = VC_BASIC; /* we'll use Filed inttype */
				cur->exptype = ET_NUM;
				assess_and_convert_expr_type(p, cur);
			}
			cur->vtype = n->type;
			expr_num_to_bool(cur, p);
			break;
		case RT_Exp_INT:
			cur->exptype = ET_NUM;
			
			cur->inttype.numeric = TRUE; /* it's a literal number */
			cur->vcmplx = VC_BASIC; /* we'll use Filed inttype */
			cur->inttype.ival = cur->val.val_int;
			cur->vtype = tnode_int;
			cur->lvalue = LV_RVALUE;
			cur->exptype = ET_NUM;
			expr_num_to_bool(cur, p);
			break;
		case RT_Exp_FLOAT:
			cur->exptype = ET_NUM;
			
			cur->inttype.numeric = TRUE; /* it's a literal number */
			cur->vcmplx = VC_BASIC; /* we'll use Filed inttype */
			cur->inttype.fval = cur->val.val_float;
			cur->vtype = tnode_float;
			cur->lvalue = LV_RVALUE;
			break;
		case RT_Args_Exp_COMMA_Args:
			cur->exptype = ET_NUM;
			im_parse(child1(cur), cur);
			cur->field = p->field; /* get the field, compare them one by one */

			f = cur->field;
			cur->field = cur->field->next;
			im_parse(child3(cur), cur);
			cur->field = f;
			/* It's OK, pass on to args */
			/* if it's an inttype, push it in */
			if (child1(cur)->vtype->kind == TK_INT)
				im_ins_tail(new_imnode_arg(exp_op(child1(cur))));
			else
			{ /* otherwise, complex, push PTR into it */
				im_ins_tail(new_imnode_arg(child1(cur)->comptype.base_addr));
			}
			cur->field = p->field; /* restore it! otherwise it could be null */
			break;
		case RT_Args_Exp:
			cur->exptype = ET_NUM;
			
			im_parse(child1(cur), cur);
			/* if it's an inttype, push it in */
			/* otherwise, complex, push PTR into it */
			rop = ((child1(cur)->vtype->kind == TK_INT) ? exp_op(child1(cur)) : child1(cur)->comptype.base_addr);
			if (p->name && strcmp(p->name, "write") == 0)
				im_ins_tail(new_imnode_write(rop));
			else
				im_ins_tail(new_imnode_arg(rop));

			cur->field = p->field; /* get the field, compare them one by one */
			break;
	}
	layer--;
}

/* public: start it! */
int interm_parse()
{
	tnode_int = new_tnode_basic(TK_INT);
	tnode_float = new_tnode_basic(TK_FLOAT);
	iop_zero = new_int_const_operand(0); /* #0 */
	iop_one = new_int_const_operand(1);  /* #1 */
	gtroot->im_prev = im_hdr(); /* get the header! */
	im_parse(gtroot, NULL);
}
