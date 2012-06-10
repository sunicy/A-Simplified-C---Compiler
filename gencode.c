/* June, 3rd, 2012 by Sunicy */
/* This file is used for recording variables' address
	and generating MIPS code
	
	this module is prefixed with gc_ (generate code)
*/

#include <stdio.h>
#include <stdlib.h>
#include "std.h"
#include "gtree.h"
#include "imnode.h"
#include "gencode.h"

#define LABEL_PREFIX "SUNICY"

/* below are some reg names */
#define REG_STR(x) #x

/* below are some instructions */
#define ISTR_LW 	"\tlw %s, %d(%s)\n"		/* s1 = MEM[s2 + #d1] */
#define ISTR_SW 	"\tsw %s, %d(%s)\n"		/* MEM[s2 + #d1] = s1 */
#define ISTR_LI		"\tli %s, %d\n"			/* s1 = #d1 */
#define ISTR_ADD	"\tadd %s, %s, %s\n"		/* s1 = s2 + s3 */
#define ISTR_MUL	"\tmul %s, %s, %s\n"		/* s1 = s2 * s3 */
#define ISTR_SUB	"\tsub %s, %s, %s\n"		/* s1 = s2 - s3 */
#define ISTR_DIV	"\tdiv %s, %s\n"			/* s1 / s2 */
#define ISTR_MFLO	"\tmflo %s\n"				/* get the division */
#define ISTR_JAL	"\tjal %s\n"				/* call s1 */
#define ISTR_JR		"\tjr %s\n"				/* return $ra */
#define ISTR_J		"\tj "LABEL_PREFIX"%d\n"/* jump s1&d1 */
#define ISTR_FLBL	"%s:\n"/* jump s1&d1 */
#define ISTR_MOV	"\tmove %s, %s\n"			/* s1 = s2 */
#define ISTR_RET	"\tjr %s\n"				/* jr s1 */
#define ISTR_BEQ	"\tbeq %s, %s, "LABEL_PREFIX"%d\n"		/* if s1 == s2 goto s3 */
#define ISTR_BNE	"\tbne %s, %s, "LABEL_PREFIX"%d\n"		/* if s1 != s2 goto s3 */
#define ISTR_BGT	"\tbgt %s, %s, "LABEL_PREFIX"%d\n"		/* if s1 > s2 goto s3 */
#define ISTR_BLT	"\tblt %s, %s, "LABEL_PREFIX"%d\n"		/* if s1 < s2 goto s3 */
#define ISTR_BGE	"\tbge %s, %s, "LABEL_PREFIX"%d\n"		/* if s1 >= s2 goto s3 */
#define ISTR_BLE	"\tble %s, %s, "LABEL_PREFIX"%d\n"		/* if s1 <= s2 goto s3 */
#define ISTR_LBL	LABEL_PREFIX"%d:\n"

#define ASSEMBLY_HEADER \
".data\n" \
"_prompt: .asciiz \"Enter an integer: \"\n" \
"_ret: .asciiz \"\\n\" \n" \
".globl main \n" \
".text \n" \
"read: \n" \
"\tli $v0, 4 \n"\
"\tla $a0, _prompt \n"\
"\tsyscall \n"\
"\tli $v0, 5 \n"\
"\tsyscall \n"\
"\tjr $ra \n"\
"write: \n"\
"\tli $v0, 1 \n"\
"\tsyscall \n"\
"\tli $v0, 4 \n"\
"\tla $a0, _ret \n"\
"\tsyscall\n"\
"\tmove $v0, $0\n"\
"\tjr $ra\n"

static FILE* fout;


/* always the header of var-link in one function */
/* its prev indicates the tail one */
static struct gc_var* listhdr;

/* create an entirely new function space */
void gc_new_func()
{
	struct gc_var *p = listhdr->next;
	while (p)
	{
		struct gc_var* q = p->next;
		free(p);
		p = q;
	}
	listhdr->next = listhdr->prev = NULL; /* reset! */
}

/* create and insert an entire new gc_var, 
	returns the new one if succ,
	NULL if already exists */
struct gc_var* gc_new_var(int vid, int offset, int len)
{
	struct gc_var *n;
	if (gc_find_var(vid))
		return NULL;
	
	n = MALLOC(struct gc_var);
	n->vid = vid;
	n->offset = offset;
	n->len = len;
	/* link it in! */
	n->next = NULL; /* always as the last one ! */
	if (listhdr->prev) /* if not empty */
	{
		n->prev = listhdr->prev;
		n->prev->next = n;
	}
	else
		n->prev = listhdr;
	listhdr->prev = n;
	if (!listhdr->next) /* if the link is empty */
		listhdr->next = n;

	return n;
}

static void gc_display_list()
{
	struct gc_var *p = listhdr;
	while(p = p->next)
	{
		printf("%d->", p->vid);
	}
	printf("\n");
}

/* find a gc_var, 
	returns ptr if exists,
	NULL otherwise */
struct gc_var* gc_find_var(int vid)
{
	struct gc_var *n = listhdr->next;
	while (n)
	{
		if (n->vid == vid)
			return n; /* have one! */
		n = n->next;
	}
	return NULL; /* not found! */
}

/* returns a var's offset, assuming it exists!!! */
int gc_var_offset(int vid)
{
	return gc_find_var(vid)->offset;
}


/* BELOW are for function search */
/* find the next function or EOF */
struct im_node* gc_find_next_func(struct im_node* prev)
{
	/* well, at least one step! */
	while (prev = prev->next)
	{
		if (prev->ntype == IT_FUNC_LABEL)
			return prev;
	}
	return prev; /* NULL! */
}

/* just print the reg~ */
static char* reg_name(int reg_no)
{
	if (reg_no == REG_t0)
		return "$t0";
	else if (reg_no == REG_t1)
		return "$t1";
	else if (reg_no == REG_t2)
		return "$t2";
	else if (reg_no == REG_t3)
		return "$t3";
	else if (reg_no == REG_t4)
		return "$t4";
	else if (reg_no == REG_t5)
		return "$t5";
	else if (reg_no == REG_t6)
		return "$t6";
	else if (reg_no == REG_sp)
		return "$sp";
	else if (reg_no == REG_ra)
		return "$ra";
	else if (reg_no == REG_v0)
		return "$v0";
	else if (reg_no == REG_a0)
		return "$a0";
}

/* BELOW are basic functions for operands */
/* load an operand into specified register */
/* only use t3, t4 and t5 */
void gc_load_operand(struct im_operand* op, int reg)
{
	if (op->otype == OT_VAR)
		gc_gen_lw(reg, gc_var_offset(op->vid), REG_sp);
	else if (op->otype == OT_CONST_INT)
		gc_gen_li(reg, op->ival);
	else if (op->otype == OT_ADDR)
	{
		/* $reg = $t4(offset) + $sp) */
		gc_gen_li(REG_t4, gc_var_offset(op->vid));
		gc_gen_add(reg, REG_t4, REG_sp);
	}
	else if (op->otype == OT_STAR)
	{
		/* $t4 = v1 */
		gc_gen_lw(REG_t4, gc_var_offset(op->vid), REG_sp);
		/* $reg = MEM[0 + $t4] */
		gc_gen_lw(reg, 0, REG_t4);
	}
}

/* save register into a specified variable space */
void gc_save_operand(struct im_operand* op, int reg)
{
	if (op->otype == OT_VAR)
		gc_gen_sw(reg, gc_var_offset(op->vid), REG_sp);
	else if (op->otype == OT_STAR)
	{
		/* $t4 = v1 */
		gc_gen_lw(REG_t4, gc_var_offset(op->vid), REG_sp);
		/* MEM[0 + $t4] = $reg */
		gc_gen_sw(reg, 0, REG_t4);
	}
}

/* push $? into stack */
void gc_push(int reg)
{
	/* $sp = $sp - 4($t4) */
	gc_gen_li(REG_t4, 4);
	gc_gen_sub(REG_sp, REG_sp, REG_t4);
	/* MEM[sp + 0] = $reg */
	gc_gen_sw(reg, 0, REG_sp);
}

/* pop $? from stack */
void gc_pop(int reg)
{
	/* $reg = MEM[sp + 0] */
	gc_gen_lw(reg, 0, REG_sp);
	/* $sp = $sp - 4($t4) */
	gc_gen_li(REG_t4, 4);
	gc_gen_add(REG_sp, REG_sp, REG_t4);
}

/* BELOW are for translating intermediate code into MIPS code */
/* ouput instruction lw */
inline static void gc_gen_lw(int reg_dst, int offset, int reg_base)
{
	fprintf(fout, ISTR_LW, reg_name(reg_dst), offset, reg_name(reg_base));
}

/* ouput instruction sw */
inline static void gc_gen_sw(int reg_src, int offset, int reg_base)
{
	fprintf(fout, ISTR_SW, reg_name(reg_src), offset, reg_name(reg_base));
}

/* output instruct li */
inline static void gc_gen_li(int reg, int constant)
{
	fprintf(fout, ISTR_LI, reg_name(reg), constant);
}

/* output instruct add */
static void gc_gen_add(int reg_dst, int reg_src1, int reg_src2)
{
	fprintf(fout, ISTR_ADD, reg_name(reg_dst), 
			reg_name(reg_src1), reg_name(reg_src2));
}

/* output instruct sub */
static void gc_gen_sub(int reg_dst, int reg_src1, int reg_src2)
{
	fprintf(fout, ISTR_SUB, reg_name(reg_dst), 
			reg_name(reg_src1), reg_name(reg_src2));
}

/* output instruct mul */
static void gc_gen_mul(int reg_dst, int reg_src1, int reg_src2)
{
	fprintf(fout, ISTR_MUL, reg_name(reg_dst), 
			reg_name(reg_src1), reg_name(reg_src2));
}

/* output instruct div */
static void gc_gen_div(int reg_dst, int reg_src1, int reg_src2)
{
	fprintf(fout, ISTR_DIV, reg_name(reg_src1), reg_name(reg_src2));
	fprintf(fout, ISTR_MFLO, reg_name(reg_dst));
}

/* output instruct label */
static void gc_gen_label(int lid)
{
	fprintf(fout, ISTR_LBL, lid);
}

/* output instruct function-label */
static void gc_gen_flabel(char* fname)
{
	fprintf(fout, ISTR_FLBL, fname);
}

/* output instruct j */
static void gc_gen_j(int lid)
{
	fprintf(fout, ISTR_J, lid);
}

/* output instruct jal */
static void gc_gen_jal(char* fname)
{
	fprintf(fout, ISTR_JAL, fname);
}

/* output instruct return */
static void gc_gen_jr()
{
	fprintf(fout, ISTR_JR, reg_name(REG_ra));
}

/* output instruct if XX goto */
static void gc_gen_ifgoto(enum relop rel, int reg1, int reg2, int lid)
#define REL_CASE(x) \
	case R_##x: \
		fprintf(fout, ISTR_B##x, reg_name(reg1), reg_name(reg2), lid);\
		break;
{
	switch (rel)
	{
		REL_CASE(GT)
		REL_CASE(LT)
		REL_CASE(GE)
		REL_CASE(LE)
		REL_CASE(EQ)
		REL_CASE(NE)
	}
}

/* output instruct mov $reg1, $reg2 */
static void gc_gen_mov(int reg1, int reg2)
{
	fprintf(fout, ISTR_MOV, reg_name(reg1), reg_name(reg2));
}

/* generate it! */
void gc_generate()
{
	struct im_node *p, *q;
	struct im_node *begin, *end;
	/* print the header! */
	fprintf(fout, ASSEMBLY_HEADER);
	/*begin = im_hdr()->next;  the very beginning! sure it is a function! */
	for (begin = im_hdr()->next; begin != NULL; begin = end)
	{
		/* 
			ARG n
			...
			ARG 1  
			RETURN ADDRESS <-- SP
			text...
		*/
		int offset = 0;
		int var_size = 0; /* local variables' size in total */
		int param_size = 0; /* parameters' size in total */
		int t;
		end = gc_find_next_func(begin);
		gc_new_func(); /* renew a function space! */
		/* print the function~ */
		gc_gen_flabel(begin->func.fname);
		/* first of all, set all parameters' offset(with positive numbers)  */
		for (p = begin->next; p->ntype == IT_PARAM; p = p->next)
			gc_new_var(p->unaryop.x->vid, offset += 4, 4); /* well, always 4B! */
		offset = -4; /* let's reverse the offset to -4, -8... */
		/* second, DEC if exists */
		for (; p->ntype == IT_DEC; p = p->next)
		{
			gc_new_var(p->dec.x->vid, offset, p->dec.width);
			offset -= p->dec.width;
		}
		/* thirdly, go through all the text to add variables */
		q = p; /* where is the text? */
		for (; p != end; p = p->next)
		{
			switch (p->ntype)
			{
				case IT_ASSIGN: 
					if (gc_new_var(p->assign.left->vid, offset, 4)) /* well, 4B~ */
						offset -= 4;
					break;
				case IT_ADD: case IT_SUB: case IT_MUL: case IT_DIV:
					if (gc_new_var(p->binop.result->vid, offset, 4)) /* well, 4B~ */
						offset -= 4;
					break;
				case IT_READ: case IT_CALL:
					if (gc_new_var(p->unaryop.x->vid, offset, 4)) /* well, 4B~ */
						offset -= 4;
					break;
			}
		}
		var_size = -offset - 4; /* actually, that's the size in total(offset indicates the NEXT!) */
		p = q; /* get back to the text */
		/* translate! */
		for (; p != end; p = p->next)
		{
			switch (p->ntype)
			{
				case IT_LABEL:
					gc_gen_label(p->label.lid);
					break;
				case IT_ASSIGN:
					/* $t0 */
					gc_load_operand(p->assign.right, REG_t0);
					gc_save_operand(p->assign.left, REG_t0);
					break;
#define BINOP(x) \
	/* $t0 = $t0 op $t1 */ \
	gc_load_operand(p->binop.op1, REG_t0);\
	gc_load_operand(p->binop.op2, REG_t1);\
	gc_gen_##x(REG_t0, REG_t0, REG_t1);\
	gc_save_operand(p->binop.result, REG_t0);

				case IT_ADD:
					BINOP(add)
					break;
				case IT_SUB:
					BINOP(sub)
					break;
				case IT_MUL:
					BINOP(mul)
					break;
				case IT_DIV:
					BINOP(div)
					break;
				case IT_GOTO:
					gc_gen_j(p->gotu.label->label.lid);
					break;
				case IT_IFGOTO:
					gc_load_operand(p->ifgoto.x, REG_t0);
					gc_load_operand(p->ifgoto.y, REG_t1);
					gc_gen_ifgoto(p->ifgoto.rel, REG_t0, REG_t1, p->ifgoto.label->label.lid); 
					break;
				case IT_ARG:
					/* find all args */
					t = 0;
					for (; p->ntype == IT_ARG; p = p->next)
					{
						/* $t0 = ... */
						gc_load_operand(p->unaryop.x, REG_t0);
						/* push! */
						gc_gen_sw(REG_t0, offset - t, REG_sp);
						//gc_push(REG_t0);
						t += 4;
					}
					/* store $ra */
					//gc_push(REG_ra);
					gc_gen_sw(REG_ra, offset - t, REG_sp);
					/* add sp! sp = sp - (SIZE + args + RETURN)($t4) */
					gc_gen_li(REG_t4, var_size + t + 4);
					gc_gen_sub(REG_sp, REG_sp, REG_t4);
					/* now it is CALL! */
					gc_gen_jal(p->call.fname);
					/* get back to previous SP */
					/* STEP 1: get back the $ra */
					gc_pop(REG_ra);
					/* STEP 2: sp = sp + t */
					gc_gen_li(REG_t4, t);
					gc_gen_add(REG_sp, REG_sp, REG_t4);
					/* STEP 3: sp = sp + var_size */
					gc_gen_li(REG_t4, var_size);
					gc_gen_add(REG_sp, REG_sp, REG_t4);
					/* now it's the return value: $v0 */
					gc_save_operand(p->call.x, REG_v0);
					break;
				case IT_CALL:
					/* add sp! sp = sp - (SIZE - 4)($t4) */
					gc_gen_li(REG_t4, var_size);
					gc_gen_sub(REG_sp, REG_sp, REG_t4);
					/* store $ra */
					gc_push(REG_ra);
					/* now it is CALL! */
					gc_gen_jal(p->call.fname);
					/* get back to previous SP */
					/* STEP 1: get back the $ra */
					gc_pop(REG_ra);
					/* STEP 2: sp = sp + t(#0) */
					/* STEP 3: sp = sp + var_size */
					gc_gen_li(REG_t4, var_size);
					gc_gen_add(REG_sp, REG_sp, REG_t4);
					/* now it's the return value: $v0 */
					gc_save_operand(p->call.x, REG_v0);
					break;
				case IT_READ:
					/* save $ra to $t0 */
					gc_gen_mov(REG_t0, REG_ra);
					/* call read */
					gc_gen_jal("read");
					/* save the return value */
					gc_save_operand(p->unaryop.x, REG_v0);
					/* get back the ra */
					gc_gen_mov(REG_ra, REG_t0);
					break;
				case IT_WRITE:
					/* save $ra to $t0 */
					gc_gen_mov(REG_t0, REG_ra);
					/* set the argment of WRITE */
					gc_load_operand(p->unaryop.x, REG_a0);
					/* call it! */
					gc_gen_jal("write");
					/* get back the ra */
					gc_gen_mov(REG_ra, REG_t0);
					break;
				case IT_RETURN:
					gc_load_operand(p->unaryop.x, REG_v0);
					gc_gen_jr();
					break;
			}
		}
	}
}


/* INITIALIZATION */
void gc_init(FILE* f)
{
	fout = (f) ? f : stdout;
	listhdr = MALLOC(struct gc_var);
	listhdr->next = listhdr->prev = NULL;
}
