/* This file is used to build up the Intermediate Code List
	By Sunicy, May 25th, 2012 */

#include "std.h"
#include <stdlib.h>
#include <stdio.h>
#include "gtree.h"
#include "imnode.h"

/* global last-* indicates variable id and label id */
static int last_vid = 0;
static int last_lid = 0;

static FILE* fout;

/* link-list header */
/* assumption: imhdr->prev = TAIL! */
static struct im_node* imhdr;

/* create im_node */
static struct im_node* new_imnode(enum im_node_type ntype)
{
	struct im_node* n = MALLOC(struct im_node);
	n->ntype = ntype;
	n->prev = n->next = NULL;
	return n;
}

/* create im_operand */
static struct im_operand* new_imoperand(enum im_operand_type otype)
{
	struct im_operand* o = MALLOC(struct im_operand);
	o->otype = otype;
	return o;
}

inline struct im_operand* new_operand_from_old(struct im_operand* old, int vid)
{
	struct im_operand* o = new_imoperand(old->otype);
	o->vid = vid;
	return o;
}

/* create an im_operand of addr(&x), vid indicates the variable */
inline struct im_operand* new_addr_operand(int vid)
{
	struct im_operand* o = new_imoperand(OT_ADDR);
	o->vid = vid;
	return o;
}

/* create an im_operand of int constant(#1), ival indicates the value */
inline struct im_operand* new_int_const_operand(int ival)
{
	struct im_operand* o = new_imoperand(OT_CONST_INT);
	o->ival = ival;
	return o;
}

/* create an im_operand of an actual var */
inline struct im_operand* new_var_operand(int vid)
{
	struct im_operand* o = new_imoperand(OT_VAR);
	o->vid = vid;
	return o;
}

/* create an im_operand of a STAR var */
inline struct im_operand* new_star_operand(int vid)
{
	struct im_operand* o = new_imoperand(OT_STAR);
	o->vid = vid;
	return o;
}

/* determine the next new variable id */
inline int new_vid()
{
	return last_vid++;
}

/* determine the next label id */
inline int new_lid()
{
	return last_lid++;
}

/* create an im_node: IT_LABEL */
struct im_node* new_imnode_label(int lid)
{
	struct im_node* n = new_imnode(IT_LABEL);
	n->label.lid = lid;
	return n;
}

/* create an im_node: IT_FUNC_LABEL */
struct im_node* new_imnode_func_label(char* fname)
{
	struct im_node* n = new_imnode(IT_FUNC_LABEL);
	n->func.fname = fname;
	return n;
}

/* create an im_node: IT_ASSIGN, RSTAR, LSTAR */
struct im_node* new_imnode_assign(struct im_operand *op1, 
							struct im_operand *op2)
{
	/* TODO: what's difference between ASSIGN, RSTAR and LSTAR?!
			since *a = *b is legal! */
	struct im_node* n = new_imnode(IT_ASSIGN);
	n->assign.left = op1;
	n->assign.right = op2;
	return n;
}

/* create an im_node: IT_ADD */
struct im_node* new_imnode_add(struct im_operand *result, 
		struct im_operand *op1, struct im_operand *op2)
{
	struct im_node* n = new_imnode(IT_ADD);
	n->binop.result = result;
	n->binop.op1 = op1;
	n->binop.op2 = op2;
	return n;
}
		
/* create an im_node: IT_SUB */
struct im_node* new_imnode_sub(struct im_operand *result, 
		struct im_operand *op1, struct im_operand *op2)
{
	struct im_node* n = new_imnode(IT_SUB);
	n->binop.result = result;
	n->binop.op1 = op1;
	n->binop.op2 = op2;
	return n;
}
		
/* create an im_node: IT_MUL */
struct im_node* new_imnode_mul(struct im_operand *result, 
		struct im_operand *op1, struct im_operand *op2)
{
	struct im_node* n = new_imnode(IT_MUL);
	n->binop.result = result;
	n->binop.op1 = op1;
	n->binop.op2 = op2;
	return n;
}
		
/* create an im_node: IT_DIV */
struct im_node* new_imnode_div(struct im_operand *result, 
		struct im_operand *op1, struct im_operand *op2)
{
	struct im_node* n = new_imnode(IT_DIV);
	n->binop.result = result;
	n->binop.op1 = op1;
	n->binop.op2 = op2;
	return n;
}
		
/* create an im_node: IT_GOTO */
struct im_node* new_imnode_goto(struct im_node *label)
{
	struct im_node* n = new_imnode(IT_GOTO);
	n->gotu.label = label;
	return n;
}

/* create an im_node: IT_IFGOTO(X REL Y ? GOTO lid) */
struct im_node* new_imnode_ifgoto(struct im_operand *x, 
		struct im_operand *y, enum relop rel, struct im_node *label)
{
	struct im_node* n = new_imnode(IT_IFGOTO);
	n->ifgoto.x = x;
	n->ifgoto.y = y;
	n->ifgoto.rel = rel;
	n->ifgoto.label = label;
	return n;
}

/* create an im_node: IT_RETURN */
struct im_node* new_imnode_return(struct im_operand *x)
{
	struct im_node* n = new_imnode(IT_RETURN);
	n->unaryop.x = x;
	return n;
}

/* create an im_node: IT_DEC */
struct im_node* new_imnode_dec(struct im_operand *x, int width)
{
	struct im_node* n = new_imnode(IT_DEC);
	n->dec.x = x;
	n->dec.width = width;
	return n;
}

/* create an im_node: IT_ARG */
struct im_node* new_imnode_arg(struct im_operand *x)
{
	struct im_node* n = new_imnode(IT_ARG);
	n->unaryop.x = x;
	return n;
}

/* create an im_node: IT_CALL, result, function name */
struct im_node* new_imnode_call(struct im_operand *x, char* fname)
{
	struct im_node* n = new_imnode(IT_CALL);
	n->call.x = x;
	n->call.fname = fname; /* CAUTION: it's ok cuz fname is a pointer, and 
		the real string is in gnode */
	return n;
}

/* create an im_node: IT_PARAM */
struct im_node* new_imnode_param(struct im_operand *v)
{
	struct im_node* n = new_imnode(IT_PARAM);
	n->unaryop.x = v;
	return n;
}

/* create an im_node: IT_READ */
struct im_node* new_imnode_read(struct im_operand *x)
{
	struct im_node* n = new_imnode(IT_READ);
	n->unaryop.x = x;
	return n;
}

/* create an im_node: IT_WRITE */
struct im_node* new_imnode_write(struct im_operand *x)
{
	struct im_node* n = new_imnode(IT_WRITE);
	n->unaryop.x = x;
	return n;
}

/* reverse ifgoto condition */
inline struct im_node* reverse_imnode_ifgoto(struct im_node* n)
{
	n->ifgoto.rel = reverse_relop(n->ifgoto.rel);
	return n;
}

/* BELOW ARE ABOUT LINK-LIST */

/* get the header */
inline struct im_node* im_hdr()
{
	return imhdr;
}

/* get the last element */
inline struct im_node* im_tail()
{
	return imhdr->prev;
}

/* insert a LIST of im_node(s) AFTER specified node, 
	and returns the inserted node's TAIL */
struct im_node* im_ins_after(struct im_node* prev, struct im_node* cur)
{
	struct im_node* p = cur;
	if (cur == NULL || prev == NULL) /* bad! */
		return NULL;
	
	cur->prev = prev;
	/* goto the tail */
	while (p->next)
		p = p->next;
	p->next = cur->next;
	if (prev->next)
		prev->next->prev = p;
	else /* otherwise p is the tail, so maintian hdr's prev! */
		imhdr->prev = p;
	prev->next = cur;
	return p;
}

/* insert a LIST of im_node(s) BEFORE specified node, 
	and returns the inserted node's HEADER */
struct im_node* im_ins_before(struct im_node* next, struct im_node* cur)
{
	struct im_node* p = cur;
	if (cur == NULL || next == NULL || cur == imhdr) /* NULL or HEADER means bad! */
		return NULL;
	
	cur->prev = next->prev;
	next->prev->next = cur;
	/* goto the tail */
	while (p->next)
		p = p->next;
	p->next = cur;
	cur->prev = p;
	return p;
}

struct im_node* im_ins_tail(struct im_node* cur)
{
	return im_ins_after(im_tail(), cur);
}

/* delete one im_node, returns its next */
struct im_node* im_del_node(struct im_node* cur)
{
	struct im_node* p = cur->next;
	if (cur->next)
		cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	free(cur);
	return p;
}

/* e.g: v11 => returns v; t12 returns t */
static inline char* get_var_prefix(int vid)
{
	/* TODO */
	return "t";
}

/*  if assuming that odd-vid indicates "v-N"
	and even-vid indicates "t-N", then we have to 
	get the output vid
	e.g: vid=10 => t5; vid=7 => v3	
*/

static inline int get_output_vid(int vid)
{
	/* TODO */
	return vid;
}

static inline void print_operand(struct im_operand* x)
{
	if (x->otype == OT_ADDR)
		fprintf(fout, "&%s%d", get_var_prefix(x->vid), get_output_vid(x->vid));
	else if (x->otype == OT_STAR)
		fprintf(fout, "*%s%d", get_var_prefix(x->vid), get_output_vid(x->vid));
	else if (x->otype == OT_CONST_INT)
		fprintf(fout, "#%d", x->ival);
	else
		fprintf(fout, "%s%d", get_var_prefix(x->vid), get_output_vid(x->vid));
}

static inline print_relop(enum relop r)
{
	if (r == R_GE)
		fprintf(fout, " >= ");
	else if (r == R_GT)
		fprintf(fout, " > ");
	else if (r == R_LE)
		fprintf(fout, " <= ");
	else if (r == R_LT)
		fprintf(fout, " < ");
	else if (r == R_NE)
		fprintf(fout, " != ");
	else if (r == R_EQ)
		fprintf(fout, " == ");
}

/* display the code! */
void im_display()
{
#define print_assignop() fprintf(fout, " := ")
#define print_binop(sig) \
		(	print_operand(p->binop.result), \
			print_assignop(), \
			print_operand(p->binop.op1), \
			fprintf(fout, " %s ", sig), \
			print_operand(p->binop.op2) \
		) 
#define LBL_PREFIX "label"
	
	struct im_node *p = imhdr;
	while (p = p->next)
	{
		//printd("#DISPLAY: %d\n", p->ntype);
		switch (p->ntype)
		{
			case IT_ASSIGN:
				print_operand(p->assign.left);
				print_assignop();
				print_operand(p->assign.right);
				break;
			case IT_LABEL:
				fprintf(fout, "LABEL %s%d :", LBL_PREFIX, p->label.lid);
				break;
			case IT_ADD:
				print_binop("+");
				break;
			case IT_SUB:
				print_binop("-");
				break;
			case IT_MUL:
				print_binop("*");
				break;
			case IT_DIV:
				print_binop("/");
				break;
			case IT_RSTAR: 
			case IT_LSTAR:
				/* TODO ? */
				break;
			case IT_GOTO:
				fprintf(fout, "GOTO %s%d", LBL_PREFIX, p->gotu.label->label.lid); /* - -# */
				break;
			case IT_IFGOTO:
				fprintf(fout, "IF ");
				print_operand(p->ifgoto.x);
				print_relop(p->ifgoto.rel);
				print_operand(p->ifgoto.y);
				fprintf(fout, " GOTO %s%d", LBL_PREFIX, p->ifgoto.label->label.lid);
				break;
			case IT_RETURN:
				fprintf(fout, "RETURN ");
				print_operand(p->unaryop.x);
				break;
			case IT_DEC:
				// DEC x [SIZE]
				fprintf(fout, "DEC ");
				print_operand(p->dec.x);
				fprintf(fout, " %d", p->dec.width);
				break;
			case IT_ARG:
				fprintf(fout, "ARG ");
				print_operand(p->unaryop.x);
				break;
			case IT_CALL:
				print_operand(p->call.x);
				fprintf(fout, " := CALL %s", p->call.fname);
				break;
			case IT_PARAM:
				fprintf(fout, "PARAM ");
				print_operand(p->unaryop.x);
				break;
			case IT_READ:
				fprintf(fout, "READ ");
				print_operand(p->unaryop.x);
				break;
			case IT_WRITE:
				fprintf(fout, "WRITE ");
				print_operand(p->unaryop.x);
				break;
			case IT_FUNC_LABEL:
				fprintf(fout, "FUNCTION %s :", p->func.fname);
				break;
		}
		fprintf(fout, "\n");
	}
}

void im_display_node(struct im_node* p)
{
	switch (p->ntype)
	{
		case IT_ASSIGN:
			print_operand(p->assign.left);
			print_assignop();
			print_operand(p->assign.right);
			break;
		case IT_LABEL:
			fprintf(fout, "LABEL %s%d :", LBL_PREFIX, p->label.lid);
			break;
		case IT_ADD:
			print_binop("+");
			break;
		case IT_SUB:
			print_binop("-");
			break;
		case IT_MUL:
			print_binop("*");
			break;
		case IT_DIV:
			print_binop("/");
			break;
		case IT_RSTAR: 
		case IT_LSTAR:
			/* TODO ? */
			break;
		case IT_GOTO:
			fprintf(fout, "GOTO %s%d", LBL_PREFIX, p->gotu.label->label.lid); /* - -# */
			break;
		case IT_IFGOTO:
			fprintf(fout, "IF ");
			print_operand(p->ifgoto.x);
			print_relop(p->ifgoto.rel);
			print_operand(p->ifgoto.y);
			fprintf(fout, " GOTO %s%d", LBL_PREFIX, p->ifgoto.label->label.lid);
			break;
		case IT_RETURN:
			fprintf(fout, "RETURN ");
			print_operand(p->unaryop.x);
			break;
		case IT_DEC:
			// DEC x [SIZE]
			fprintf(fout, "DEC ");
			print_operand(p->dec.x);
			fprintf(fout, " %d", p->dec.width);
			break;
		case IT_ARG:
			fprintf(fout, "ARG ");
			print_operand(p->unaryop.x);
			break;
		case IT_CALL:
			print_operand(p->call.x);
			fprintf(fout, " := CALL %s", p->call.fname);
			break;
		case IT_PARAM:
			fprintf(fout, "PARAM ");
			print_operand(p->unaryop.x);
			break;
		case IT_READ:
			fprintf(fout, "READ ");
			print_operand(p->unaryop.x);
			break;
		case IT_WRITE:
			fprintf(fout, "WRITE ");
			print_operand(p->unaryop.x);
			break;
		case IT_FUNC_LABEL:
			fprintf(fout, "FUNCTION %s :", p->func.fname);
			break;
	}
	fprintf(fout, "\n");
}

/* BELOW ARE FOR INITIALIZATION */
void im_init(FILE* f)
{
	imhdr = new_imnode(IT_HEADER);
	imhdr->prev = imhdr;
	fout = (f) ? f : stdout;
}

