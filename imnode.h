/* This file is used to build up the Intermediate Code List
	By Sunicy, May 25th, 2012 */
#ifndef _IMNODE_H_
#define _IMNODE_H_

enum im_operand_type {
	OT_VAR,			/* A variable */
	OT_CONST_INT, 	/* A constant integer */
	OT_CONST_FLOAT,	/* A constant float */
	OT_ADDR,		/* An address */
	OT_STAR			/* equavalent to *x */
};

struct im_operand {
	enum im_operand_type otype; /* Operand type */
	union {
		int ival;		/* integer constant */
		float fval;		/* float constant */
		int vid;		/* variable id, used when OT_ADDR or OT_VAR */
	};
};

enum im_node_type {
	IT_HEADER,		/* Well, it's a header */
	IT_LABEL, 		/* LABEL sunicy1 */
	IT_ASSIGN, 		/* x = y */
	IT_ADD, 		/* x = y + z */
	IT_SUB, 		/* x = y - z */
	IT_MUL, 		/* x = y * z */
	IT_DIV, 		/* x = y / z */
	IT_RSTAR,		/* x = *y */
	IT_LSTAR, 		/* *x = y */
	IT_GOTO, 		/* GOTO sunicy2 */
	IT_IFGOTO, 		/* IF x RELOP y GOTO sunicy3 */
	IT_RETURN, 		/* RETURN x */
	IT_DEC, 		/* DEC x [SIZE] */
	IT_ARG, 		/* ARG x */
	IT_CALL, 		/* x = CALL f */
	IT_PARAM, 		/* PARAM v */
	IT_READ, 		/* READ x */
	IT_WRITE,		/* WRITE x */
	IT_FUNC_LABEL	/* FUNCTION x : */
};

struct im_node {
	enum im_node_type ntype; /* What kind is this stmt? */
	union {
		struct { struct im_operand *left, *right; } assign;
		struct { struct im_operand *result, *op1, *op2; } binop;
		struct { 
			int lid;	/* label id */
		} label;
		struct {
			struct im_node *label; /* where to jump? */
		} gotu;
		struct { 
			struct im_operand *x, *y;
			enum relop rel;
			struct im_node *label;
		} ifgoto;
		struct { struct im_operand *x; } unaryop;
		struct { struct im_operand *x; int width; } dec;
		struct { struct im_operand *x; char* fname; } call;
		struct { char* fname;} func;
	};
	struct im_node *prev, *next; /* link lst */
};

struct im_operand* new_operand_from_old(struct im_operand* old, int vid);

/* create an im_operand of addr(&x), vid indicates the variable */
struct im_operand* new_addr_operand(int vid);

/* create an im_operand of int constant(#1), ival indicates the value */
struct im_operand* new_int_const_operand(int ival);

/* create an im_operand of an actual var */
struct im_operand* new_var_operand(int vid);

/* create an im_operand of a STAR var */
struct im_operand* new_star_operand(int vid);

/* determine the next new variable id */
int new_vid();

/* determine the next param variable id */
#define new_pvid new_vid

/* determine the next label id */
int new_lid();

/* create an im_node: IT_LABEL */
struct im_node* new_imnode_label(int lid);

/* create an im_node: IT_FUNC_LABEL */
struct im_node* new_imnode_func_label(char* fname);

/* create an im_node: IT_ASSIGN, RSTAR, LSTAR */
struct im_node* new_imnode_assign(struct im_operand *op1, 
							struct im_operand *op2);

/* create an im_node: IT_ADD */
struct im_node* new_imnode_add(struct im_operand *result, 
		struct im_operand *op1, struct im_operand *op2);
		
/* create an im_node: IT_SUB */
struct im_node* new_imnode_sub(struct im_operand *result, 
		struct im_operand *op1, struct im_operand *op2);
		
/* create an im_node: IT_MUL */
struct im_node* new_imnode_mul(struct im_operand *result, 
		struct im_operand *op1, struct im_operand *op2);
		
/* create an im_node: IT_DIV */
struct im_node* new_imnode_div(struct im_operand *result, 
		struct im_operand *op1, struct im_operand *op2);
		
/* create an im_node: IT_GOTO */
struct im_node* new_imnode_goto(struct im_node *label);

/* create an im_node: IT_IFGOTO(X REL Y ? GOTO lid) */
struct im_node* new_imnode_ifgoto(struct im_operand *x, 
		struct im_operand *y, enum relop rel, struct im_node *label);

/* create an im_node: IT_RETURN */
struct im_node* new_imnode_return(struct im_operand *x);

/* create an im_node: IT_DEC */
struct im_node* new_imnode_dec(struct im_operand *x, int width);

/* create an im_node: IT_ARG */
struct im_node* new_imnode_arg(struct im_operand *x);

/* create an im_node: IT_CALL, result, function name */
struct im_node* new_imnode_call(struct im_operand *x, char* fname);

/* create an im_node: IT_PARAM */
struct im_node* new_imnode_param(struct im_operand *v);

/* create an im_node: IT_READ */
struct im_node* new_imnode_read(struct im_operand *x);

/* create an im_node: IT_WRITE */
struct im_node* new_imnode_write(struct im_operand *x);

/* reverse ifgoto condition */
struct im_node* reverse_imnode_ifgoto(struct im_node* n);

/* BELOW ARE ABOUT LINK-LIST */
/* get the header */
struct im_node* im_hdr();

/* get the last element */
struct im_node* im_tail();

/* insert a LIST of im_node(s) AFTER specified node, 
	and returns the inserted node's TAIL */
struct im_node* im_ins_after(struct im_node* prev, struct im_node* cur);

/* insert a LIST of im_node(s) BEFORE specified node, 
	and returns the inserted node's HEADER */
struct im_node* im_ins_before(struct im_node* next, struct im_node* cur);

struct im_node* im_ins_tail(struct im_node* cur);

/* delete one im_node, returns its next */
struct im_node* im_del_node(struct im_node* cur);

/* display the code(one node) */
void im_display_node(struct im_node* p);
/* display the code! */
void im_display();

/* BELOW ARE FOR INITIALIZATION */
void im_init();

#endif
