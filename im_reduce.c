/* Created on May. 27th, 2012 by Sunicy Tao */

/* This file aims to reduce the intermediate code, 
	and it's a part of Module IntermediateCode

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
#include "im_reduce.h"

#define const_op(op) ((op)->otype == OT_CONST_INT)
#define lvalue_op(op) ((op)->otype == OT_VAR)
#define same_op(op1,op2) (((op1)->otype == (op2)->otype)&&((op1)->vid == (op2)->vid))
#define goto_like_op(p) (p->ntype == IT_GOTO || \
						p->ntype == IT_IFGOTO || \
						p->ntype == IT_LABEL || \
						p->ntype == IT_FUNC_LABEL)

static int find_vid_after(struct im_node* n, int vid)
{
	while (n = n->next)
	{

		switch (n->ntype)
		{
			case IT_SUB: case IT_ADD: case IT_MUL: case IT_DIV:
				if (!const_op(n->binop.op1) && n->binop.op1->vid == vid)
					return TRUE;
				if (!const_op(n->binop.op2) && n->binop.op2->vid == vid)
					return TRUE;
				break;
			case IT_ASSIGN:
				if (!const_op(n->assign.right) && n->assign.right->vid == vid)
					return TRUE;
				break;
			case IT_IFGOTO:
				if (!const_op(n->ifgoto.x) && n->ifgoto.x->vid == vid)
					return TRUE;
				if (!const_op(n->ifgoto.y) && n->ifgoto.y->vid == vid)
					return TRUE;
				break;
			case IT_RETURN: case IT_ARG: case IT_WRITE:/* case IT_READ:*/
				if (!const_op(n->unaryop.x) && n->unaryop.x->vid == vid)
					return TRUE;
				break;
		}
	}
	return FALSE;
}

static int find_rvalue_vid(struct im_node* n, int vid)
{
	switch (n->ntype)
	{
		case IT_SUB: case IT_ADD: case IT_MUL: case IT_DIV:
			if (!const_op(n->binop.op1) && n->binop.op1->vid == vid)
				return TRUE;
			if (!const_op(n->binop.op2) && n->binop.op2->vid == vid)
				return TRUE;
			break;
		case IT_ASSIGN:
			if (!const_op(n->assign.right) && n->assign.right->vid == vid)
				return TRUE;
			break;
		case IT_IFGOTO:
			if (!const_op(n->ifgoto.x) && n->ifgoto.x->vid == vid)
				return TRUE;
			if (!const_op(n->ifgoto.y) && n->ifgoto.y->vid == vid)
				return TRUE;
			break;
		case IT_RETURN: case IT_ARG: case IT_WRITE:/* case IT_READ:*/
			if (!const_op(n->unaryop.x) && n->unaryop.x->vid == vid)
				return TRUE;
			break;
	}
	return FALSE;
}

/* if some vid is changed! */
int find_lvalue_vid(struct im_node* n, int vid)
{
	switch (n->ntype)
	{
		case IT_SUB: case IT_ADD: case IT_MUL: case IT_DIV:
			if (n->binop.result->vid == vid)
				return TRUE;
			break;
		case IT_ASSIGN:
			/* *t1 = ... isn't taken into consideration */
			if (n->assign.left->vid == vid && n->assign.left->otype != OT_STAR)
				return TRUE;
			break;
		case IT_READ:
			if (n->unaryop.x->vid == vid)
				return TRUE;
			break;
		case IT_CALL:
			if (n->call.x->vid == vid)
				return TRUE;
			break;
	}
	return FALSE;
}

/* p indicates the previous one */
static struct im_node* find_next_block_point(struct im_node* p)
{
	while (p = p->next)
		if (goto_like_op(p))
			return p;
	return NULL; /* till the end! */
}

/* if a var is used before end(end excluded) */
static int find_vid_before_point(struct im_node* end, int vid)
{
	struct im_node *p = im_hdr()->next;
	while (p != end)
	{
		if (find_lvalue_vid(p, vid) || find_rvalue_vid(p, vid)) /* if used */
			return TRUE;
		p = p->next;
	}
	return FALSE; /*never be used */
}

/* if a var is used after begin(begin excluded) */
static int find_vid_after_point(struct im_node* begin, int vid)
{
	struct im_node *p;
	if (begin)
		p = begin->next;
	else
		return FALSE;

	while (p != NULL)
	{
		if (find_lvalue_vid(p, vid) || find_rvalue_vid(p, vid)) /* if used */
			return TRUE;
		p = p->next;
	}
	return FALSE; /*never be used */
}

/* true if a tmp var */
inline static int temp_var(struct im_node* begin, struct im_node *end, int vid)
{
	return (!find_vid_before_point(begin, vid) && !find_vid_after_point(end, vid));
}

/* begin, end excluded */
static int lvalue_var_count(struct im_node* begin, struct im_node* end, int vid)
{
	int count = 0;
	for (begin = begin->next; begin != end; begin = begin->next)
	{
		if (find_lvalue_vid(begin, vid))
			count++;
	}
	return count;
}

/* t1 = t2 + t3; => t1 = t0 + t3... */
/* returns COUNT */
static int change_rvalue_vid(struct im_node* begin, struct im_node *end, 
								int vid/*src vid*/, int  dst_vid)
#define change_op(x) x = new_operand_from_old(x, dst_vid), count++
{
	struct im_node *n;
	int count = 0;
	for (n = begin->next; n != end; n = n->next)
	{
		switch (n->ntype)
		{
			case IT_SUB: case IT_ADD: case IT_MUL: case IT_DIV:
				if (!const_op(n->binop.op1) && n->binop.op1->vid == vid)
					//n->binop.op1 = new_operand_from_old(, dst_vid, count++;
					change_op(n->binop.op1);
				if (!const_op(n->binop.op2) && n->binop.op2->vid == vid)
					change_op(n->binop.op2);
					//n->binop.op2->vid = dst_vid, count++;
				break;
			case IT_ASSIGN:
				if (!const_op(n->assign.right) && n->assign.right->vid == vid)
					//n->assign.right->vid = dst_vid, count++;
					change_op(n->assign.right);
				if (n->assign.left->otype == OT_STAR && n->assign.left->vid == vid)
					change_op(n->assign.left);
				break;
			case IT_IFGOTO:
				if (!const_op(n->ifgoto.x) && n->ifgoto.x->vid == vid)
					//n->ifgoto.x->vid = dst_vid, count++;
					change_op(n->ifgoto.x);
				if (!const_op(n->ifgoto.y) && n->ifgoto.y->vid == vid)
					//n->ifgoto.y->vid = dst_vid, count++;
					change_op(n->ifgoto.y);
				break;
			case IT_RETURN: case IT_ARG: case IT_WRITE:/* case IT_READ:*/
				if (!const_op(n->unaryop.x) && n->unaryop.x->vid == vid)
					//n->unaryop.x->vid = dst_vid, count++;
					change_op(n->unaryop.x);
				break;
		}
	}
	return count;
}

#define reduce_continous(name, sig) \
{ \
	struct im_node* p = im_hdr()->next; \
	while (p) \
	{ \
		struct im_node* q = p->next; \
		int tmp = 0; \
		if (q == NULL) \
			break; /* fun's over~ */ \
		/*  \
			t2 = t2 + #10  \
			t2 = t2 + #10 \
		*/ \
		/* MEMORY LEAKER~ */ \
		if (p->ntype == name && q->ntype == name && lvalue_op(p->binop.result) && \
			same_op(p->binop.result, p->binop.op1) && \
			same_op(p->binop.result, q->binop.result) && \
			same_op(p->binop.result, q->binop.op1)\
			&& const_op(q->binop.op2) && const_op(q->binop.op2)) \
		{\
			struct im_operand *lop = new_int_const_operand(q->binop.op2->ival sig p->binop.op2->ival); \
			q->binop.op2 = lop; \
			p = im_del_node(p); \
		}\
		/* t2 = t1 + #10; t2 = t3 + #10 ==> t2 = t3 + #10 */ \
		else if (p->ntype == name && q->ntype == name && lvalue_op(p->binop.result) &&\
			lvalue_op(q->binop.result) && lvalue_op(p->binop.op1)\
			&& const_op(p->binop.op2) && const_op(q->binop.op2) && same_op(p->binop.result, q->binop.result) &&\
			lvalue_op(q->binop.op1) && !same_op(p->binop.result, q->binop.op1)) \
		{\
			p = im_del_node(p); \
		}\
		/* t1:=#11; t2:=t1+#8 ==> t2:=#19 */\
		else if (p->ntype == IT_ASSIGN && const_op(p->assign.right) && q->ntype == name && \
			lvalue_op(q->binop.result) && same_op(q->binop.result, p->assign.left) && \
			same_op(q->binop.result, q->binop.op1) && const_op(q->binop.op2)) \
		{ \
			struct im_operand *lop = new_int_const_operand(q->binop.op2->ival sig p->assign.right->ival); \
			q->ntype = IT_ASSIGN; \
			q->assign.left = q->binop.result; \
			q->assign.right = lop; \
			p = im_del_node(p); \
		} \
		/* t2 = t1 + #1; t3 = t2 + #3; ==> t3 = t1 + #4*/ \
		else if (p->ntype == name && const_op(p->binop.op2) && q->ntype == name && \
			lvalue_op(q->binop.result) && lvalue_op(p->binop.result) &&  \
			same_op(q-> binop.op1, p->binop.result) && \
			lvalue_op(q->binop.op1) && const_op(q->binop.op2) \
			&& (!find_vid_after(q, p->binop.result->vid)))  \
		{ \
			struct im_operand *lop = new_int_const_operand(q->binop.op2->ival sig p->binop.op2->ival); \
			q->binop.op2 = lop; \
			q->binop.op1 = p->binop.op1; \
			p = im_del_node(p); \
		} \
		else if (p->ntype == IT_ASSIGN && const_op(p->assign.right) && q->ntype == name && \
			lvalue_op(q->binop.result) && lvalue_op(p->assign.left) && \
			lvalue_op(q->binop.op1) && const_op(q->binop.op2) && !find_vid_after(q, p->unaryop.x->vid)) \
		{ \
			struct im_operand *lop = new_int_const_operand(q->binop.op2->ival sig p->assign.right->ival); \
			q->ntype = IT_ASSIGN; \
			q->assign.left = q->binop.result; \
			q->assign.right = lop; \
			p = im_del_node(p); \
		} \
		else \
			p = p->next; /* move on */ \
	} \
} 

static void reduce_assign()
{
	struct im_node* q = im_hdr()->next;
	while(q)
	{
		struct im_node* p = q->next;
		if (p == NULL)
			break; /* fun's over again */
		/* 
			t101 = ....                   v = ...
			v = t101               =>
			(t101 is temperoray var)
		*/
		if (q->ntype == IT_ADD || q->ntype == IT_MUL ||
			q->ntype == IT_SUB || q->ntype == IT_DIV)
			if (p->ntype == IT_ASSIGN && lvalue_op(p->assign.left) &&
				lvalue_op(q->binop.result) && same_op(q->binop.result, p->assign.right) &&
				!(find_vid_after(p, q->binop.result->vid)))
			{
				q->binop.result = p->assign.left;
				q = im_del_node(p);
				continue;
			}
		q = q->next;
	}
}

static void reduce_ifgoto()
{
	struct im_node* p = im_hdr()->next;
	while (p)
	{
		struct im_node *q, *r;
		q = p->next;
		if (q == NULL)
			break; /* sad */
		r = q->next;
		if (r == NULL)
			break; /* well, sad again */

		/* IF x REL y GOTO label1			IF x -REL y GOTO label2
			GOTO label2				==>  	LABEL label1:
			LABEL label1:					...
			...
		*/
		if (p->ntype == IT_IFGOTO && q->ntype == IT_GOTO && r->ntype == IT_LABEL)
			if (p->ifgoto.label == r)
			{
				p->ifgoto.rel = reverse_relop(p->ifgoto.rel);
				p->ifgoto.label = q->gotu.label;
				im_del_node(q);
				p = r->next;
				continue;
			}
		p = p->next;
	}
}

/*
	GOTO label1
	LABEL label1 :  ==> LABEL label1 :
*/
void reduce_goto_label()
{
	struct im_node *p = im_hdr()->next;
	while (p)
	{
		struct im_node* q = p->next;
		if (q == NULL)
			break; /* fun's over~ */
		if (p->ntype == IT_GOTO && q->ntype == IT_LABEL &&
			p->gotu.label->label.lid == q->label.lid)
			p = im_del_node(p);
		else
			p = p->next;
	}
}

int find_label(struct im_node *p)
{
	
}

void reduce_empty_label()
{
	struct im_node *p = im_hdr()->next;
	while (p)
	{
		int flag = FALSE;
		struct im_node *q = im_hdr();
		if (p->ntype != IT_LABEL)
		{
			p = p->next;
			continue; /* oops.. */
		}

		while (q = q->next)
		{
			if ((q->ntype == IT_GOTO && q->gotu.label->label.lid == p->label.lid) ||
				(q->ntype == IT_IFGOTO && q->ifgoto.label->label.lid == p->label.lid)) 
			{
			/* if p is used! */
				flag = TRUE;
				break;
			}
		}
		if (!flag) /* not used! */
			p = im_del_node(p); /* delete the LABEL */
		else
			p = p->next;
	}
}



void reduce_block_locate()
#define basic_op(p) (p->ntype == IT_ADD || \
			p->ntype == IT_SUB || \
			p->ntype == IT_MUL || \
			p->ntype == IT_DIV)
{
	struct im_node *p = im_hdr()->next;
	struct im_node *begin, *end;
	//begin = im_hdr()->next; /* the very first node */
	for (begin = im_hdr()->next; begin != NULL; begin = end)
	{
		end = find_next_block_point(begin);
		/*get the first non-goto-like node */
		for (p = begin->next; p != end; p = p->next)
		{
			struct im_node *q;
			int vidr = -1, vid1 = -1, vid2 = -1; /* t100 := t[vid1] + t[vid2] */
			if (!basic_op(p))  /* not the basic op? skip! */
				continue;
			vidr = p->binop.result->vid;
			if (p->binop.op1->otype != OT_CONST_INT)
				vid1 = p->binop.op1->vid;
			if (p->binop.op2->otype != OT_CONST_INT)
				vid2 = p->binop.op2->vid;
			
			for (q = p->next; q != end; q = q->next)
			{
				/* is q OK? */
				if (basic_op(q))
				{
					if (same_op(p->binop.op1, q->binop.op1) && /* just the same! */
						same_op(p->binop.op2, q->binop.op2) &&
						temp_var(begin, end, q->binop.result->vid) &&
						lvalue_var_count(begin, end, q->binop.result->vid) < 2) /* and is a ONE-TIME temp var! */
					{
						int qvid = q->binop.result->vid;
						/* delete the var! */
						q = im_del_node(q)->prev;
						/* change the rest reference! */
						change_rvalue_vid(q, end, qvid, p->binop.result->vid);
					}
				}
				else if (vid1 > -1 && find_lvalue_vid(q, vid1))
					break; /* var of p has changed!, fun's over */
				else if (vid2 > -1 && find_lvalue_vid(q, vid2))
					break;
				else if (vidr > -1 && find_lvalue_vid(q, vidr))
					break;
			}
		}
	}
}

/* do the reducing work */
void im_reduce(int level)
{
	if (level > 0)
		reduce_assign();

	if (level > 5)
	{
		reduce_continous(IT_ADD, +);
		reduce_continous(IT_SUB, -);
		reduce_continous(IT_MUL, *);
		reduce_continous(IT_DIV, /);
	}
	if (level > 6)
		reduce_ifgoto();

	if (level > 7)
		reduce_goto_label();

	if (level > 8)
		reduce_empty_label();

	if (level > 9)
		reduce_block_locate();
}
