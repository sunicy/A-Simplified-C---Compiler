/* Created on Apr. 26th, 2012 by Sunicy Tao */

/* This file aims to deal with Symbol Table */
/* including Hash Table and Scope Stack */
/* 1st: TYPE tree */
/* 2nd: VAR tree */
/* 3rd: FUNCTION tree */

#ifndef _STABLE_H_
#define _STABLE_H_

enum typekind { /* type kind */
	TK_INT, TK_FLOAT, TK_ARRAY, TK_STRUCT, TK_FUNC, TK_ERROR
};

enum symkind {	/* type of symbol table node */
	SK_UNKNOWN, 
	SK_DEF,		/* It's a definition node */
	SK_VAR,		/* It's a variable node */
	SK_LINKHDR,	/* It's a link-list header */
	SK_SSTACKHDR/* It's a scope stack header */
};

enum var_type {
	VT_PTR, 		/* the tmp variable is actually a pointer */
	VT_CONCRETE,	/* the tmp variable is the variable itself */
	VT_CONSTANT		/* It's actually a const */
};		/* VT_PTR, VT_CONCRETE */

struct tnode;

#define LAST_BASIC_TYPE TK_FLOAT

/* used for array */
struct tarray {
	struct tnode* type; 
	int size;
	int width;
};

/* used for struct/union */
struct tfield_lst {
	char* name;
	struct tnode* type;
	struct tfield_lst* next;
	int width;
	int offset;
};

/* used for function */
#define FUNC_DEFINED 1
#define FUNC_UNDEFINED 0
struct func {
	struct tnode* return_type; 		/* return type */
	struct tfield_lst* param;		/* param list */
	int defined;					/* non-zero: Defined, 0: only declarated */
};


/* type  */
struct tnode {
	enum typekind kind;				/* variable, function, link-header */
	int min_scope_depth;			/* TODO: the minimun scope depth tnode is used in */
	int width;						/* inh: width of the type */
	union {
		/* used for function */
		struct func* function;		/* ptr to function */

		/* used for struct */
		struct tfield_lst* field_lst;

		/* used for array */
		struct tarray array;
	};
};

/* symbol table node */
struct snode {
	struct snode *up, *down;		/* link of SCOPE-STACK */
	struct snode *left, *right;		/* link of HASH TABLE */

	enum symkind kind;

	char* name; 					/* name of variable, function */
	struct {
		int vid;					/* tmp vid */
		enum var_type vtype;		/* VT_PTR, VT_CONCRETE */
	} cur_v;						/* indicates the tmp variable currently used */
	struct tnode *type;
};

#define HASH_LENGTH 100
#define hashhdr snode /* define hash header type */
#define sstackhdr snode /* define SCOPE STACK header type */

/* :: Hash LINK */

/* find nearest snode, if kind=UNKNOWN, kind doesnt matter*/
struct snode* find_snode(char* name, enum symkind kind);

/* find nearest snode through the same Scope */
struct snode* find_snode_scope(struct sstackhdr* ssh, char* name, enum symkind kind);

/* find nearest snode through the CURRENT Scope */
struct snode* find_snode_cur_scope(char* name, enum symkind kind);

/* try to add an snode into hashtable, and returns snode pointer if succ,
 * NULL if falied; 													
 * Arguments: sshdr[Specifies the scope], name[var, struct, fun name]	*/
struct snode* add_into_shash(struct sstackhdr* sshdr, char* name,
								enum symkind kind, struct tnode* type);

/* set variable property */
void set_snode_vid(struct snode* n, int vid, enum var_type vtype);

/* get variable ID */
int get_snode_vid(struct snode* n);

/* get variable type (ptr or concrete) */
enum var_type get_snode_var_type(struct snode* n);

/* Simplified version of add_into_shash, only add into CURRENT scope */
struct snode* add_snode(char* name, enum symkind kind, struct tnode* type);

/* Simplified version of add_into_shash, only add into PARENT of current scope */
struct snode* add_snode_parent(char* name, enum symkind kind, struct tnode* type);

/* :: Scope Stack */
/* ASSUMPTION: sslhdr->right means CURRENT sstackhdr */

/* create a new scope, just after the header link[sslhdr]! */
struct sstackhdr* new_scope();

/* get the current scope node */
struct sstackhdr* cur_sstack();
#define cur_scope cur_sstack

/* delete CURRENT scope, returns 0 if succ, -1 if failed*/
int del_current_scope();

/* :: TYPE NODE */

/* create a tnode specifying its kind */
struct tnode* new_tnode(enum typekind kind);

/* create a tnode with basic type */
#define new_tnode_basic new_tnode 

/* create an array made of basic type */
struct tnode* new_tnode_arr_by_basic(enum typekind kind, int size);

/* create an array made of a tnode */
struct tnode* new_tnode_arr_by_tnode(struct tnode* sub, int size);

/* create a struct tnode, specifying OptName and fieldlst */
struct tnode* new_tnode_struct(struct tfield_lst* field);

/* create a field-lst and link it on */
struct tfield_lst* new_fieldlst_link(struct tnode* n, char* name, struct tfield_lst* next);

/* create a func, specifying return type and param-link-lst */
struct func* new_func(struct tnode* return_type, struct tfield_lst* param, int func_defined);

/* delete a func */
void del_func(struct func* f);

/* create a tnode made of func */
struct tnode* new_tnode_func(struct func* f);

/* judge whether 2 types are same, 0 if same */
int typecmp(struct tnode* n1, struct tnode* n2);

/* judge whether 2 field-lst are same, 0 if same */
int fieldcmp(struct tfield_lst* f1, struct tfield_lst* f2);

/* judge whether 2 funcs are same, 0 if same */
int funccmp(struct func* f1, struct func* f2);

/* search a field-lst for one ID */
struct tfield_lst* find_field(struct tfield_lst* field, char* name);

/* create a field-list from current scope */
struct tfield_lst* create_fieldlst_from_cur_scope();

/* :: OVERALL functions */
void init_stable();


#endif
