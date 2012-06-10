/* June, 3rd, 2012 by Sunicy */
/* This file is used for recording variables' address
	and generating MIPS code
	
	this module is prefixed with gc_ (generate code)
*/

#ifndef _GENCODE_H_
#define _GENCODE_H_

#define REG_t(x) (x + 8) 
#define REG_t0 REG_t(0)
#define REG_t1 REG_t(1)
#define REG_t2 REG_t(2)
#define REG_t3 REG_t(3)
#define REG_t4 REG_t(4)
#define REG_t5 REG_t(5)
#define REG_t6 REG_t(6)
#define REG_LAST_t REG_t6
#define REG_sp 29
#define REG_ra 31
#define REG_v0 2
#define REG_a0 4

/* BELOW are for variable addr info */ 

/* variable info(all in a function): address */
struct gc_var {
	int vid; /* variable id */
	int offset; /* addr offset */
	int len; /* addr's length! */

	struct gc_var *prev, *next; /* it's a lnk-list! */
};

/* create an entirely new function space */
void gc_new_func();
/* create and insert an entire new gc_var, 
	returns the new one if succ,
	NULL if already exists */
struct gc_var* gc_new_var(int vid, int offset, int len);

/* find a gc_var, 
	returns ptr if exists,
	NULL otherwise */
struct gc_var* gc_find_var(int vid);

/* returns a var's offset, assuming it exists!!! */
int gc_var_offset(int vid);

/* BELOW are for function search */
/* find the next function or EOF */
struct im_node* gc_find_next_func(struct im_node* prev);

/* BELOW are basic functions for operands */
/* load an operand into specified register */
void gc_load_operand(struct im_operand* op, int reg);

/* save register into a specified variable space */
void gc_save_operand(struct im_operand* op, int reg);

/* push $? into stack */
void gc_push(int reg);
/* pop $? from stack */
void gc_pop(int reg);

/* BELOW are for translating intermediate code into MIPS code */
/* ouput instruction lw */
static void gc_gen_lw(int reg_dst, int offset, int reg_base);
/* ouput instruction sw */
static void gc_gen_sw(int reg_src, int offset, int reg_base);
/* output instruct li */
static void gc_gen_li(int reg, int constant);
/* output instruct add */
static void gc_gen_add(int reg_dst, int reg_src1, int reg_src2);
/* output instruct sub */
static void gc_gen_sub(int reg_dst, int reg_src1, int reg_src2);
/* output instruct mul */
static void gc_gen_mul(int reg_dst, int reg_src1, int reg_src2);
/* output instruct div */
static void gc_gen_div(int reg_dst, int reg_src1, int reg_src2);
/* output instruct label */
static void gc_gen_label(int lid);
/* output instruct function-label */
static void gc_gen_flabel(char* fname);
/* output instruct j */
static void gc_gen_j(int lid);
/* output instruct jal */
static void gc_gen_jal(char* fname);
/* output instruct return */
static void gc_gen_jr();
/* output instruct if XX goto */
static void gc_gen_ifgoto(enum relop rel, int reg1, int reg2, int lid);
/* output instruct mov $reg1, $reg2 */
static void gc_gen_mov(int reg1, int reg2);



/* generate it! */
void gc_generate();


#endif
