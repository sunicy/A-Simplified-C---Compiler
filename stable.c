/* Created on Apr. 26th, 2012 by Sunicy Tao */

/* This file aims to deal with Symbol Table */
/* including Hash Table and Scope Stack */

#include "std.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stable.h"

struct hashhdr shash[HASH_LENGTH];
struct sstackhdr* sslhdr = NULL; /* ssl is short for Scope Stack Link */

static int cur_scope_depth = 0; /* TODO: maintained in add-scope and del-scope */

/* :: HASH Table */

/* get hash from string */
static int str_hash(char* name)
{
	unsigned int val = 0, i; 
	for ( ; *name; ++name) 
	{ 
		val = (val << 2) + *name; 
		if ( i = val & ~0x3fff)
			val = (val ^ (i >> 12)) & 0x3fff; 
	} 
	return val % HASH_LENGTH; 
}

/* create an snode and initialize it */
static struct snode* new_snode()
{
	struct snode* p = MALLOC(struct snode);/*(struct snode*)malloc(sizeof(struct snode));*/
	p->up = p->down = p->left = p->right = NULL;
	p->kind = SK_UNKNOWN;
	p->name = NULL;
	p->type = NULL;
	p->cur_v.vid = -1; /* NO VARIABLE! */
	return p;
}

/* find nearest snode, if kind=UNKNOWN, kind doesnt matter*/
struct snode* find_snode(char* name, enum symkind kind)
{
	struct snode* p = &shash[str_hash(name)];
	while (p->right)
	{
		if (strcmp(p->right->name, name) == 0 && 
			(kind == SK_UNKNOWN || p->right->kind == kind))
			return p->right;
		p = p->right;
	}
	/* failed! */
	return NULL;
}

/* find nearest snode through the same Scope */
struct snode* find_snode_scope(struct sstackhdr* ssh, char* name, enum symkind kind)
{
	struct snode* p = ssh->down;
	while (p)
	{
		if (strcmp(p->name, name) == 0 && 
			(kind == SK_UNKNOWN || p->kind == kind))
			return p;
		p = p->down;
	}
	return NULL;
}

/* find nearest snode through the CURRENT Scope */
inline struct snode* find_snode_cur_scope(char* name, enum symkind kind)
{
	return find_snode_scope(cur_scope(), name, kind);
}

/* try to add an snode into hashtable, and returns snode pointer if succ,
 * NULL if falied; 													
 * Arguments: sshdr[Specifies the scope], name[var, struct, fun name]	*/
struct snode* add_into_shash(struct sstackhdr* sshdr, char* name,
								enum symkind kind, struct tnode* type)
{
	struct snode* p = new_snode();
	struct hashhdr* hhdr = &shash[str_hash(name)];

	if (sshdr == NULL) /* no scope! */
		return NULL;

	/* basic info */
	p->name = name;
	p->kind = kind;
	p->type = type;

	/* link into the left-right list */
	p->right = hhdr->right;
	p->left = hhdr;
	hhdr->right = p;
	if (p->right)
		p->right->left = p;

	/* link into the up-down list */
	p->down = sshdr->down;
	p->up = sshdr;
	sshdr->down = p;
	if (p->down)
		p->down->up = p;

	return p;
}

/* set variable property */
inline void set_snode_vid(struct snode* n, int vid, enum var_type vtype)
{
	n->cur_v.vid = vid;
	n->cur_v.vtype = vtype;
}

/* get variable ID */
inline int get_snode_vid(struct snode* n)
{
	return n->cur_v.vid;
}

/* get variable type (ptr or concrete) */
inline enum var_type get_snode_var_type(struct snode* n)
{
	return n->cur_v.vtype;
}

/* Simplified version of add_into_shash, only add into CURRENT scope */
inline struct snode* add_snode(char* name, enum symkind kind, struct tnode* type)
{
	return add_into_shash(cur_scope(), name, kind, type);
}

/* Simplified version of add_into_shash, only add into PARENT of current scope */
struct snode* add_snode_parent(char* name, enum symkind kind, struct tnode* type)
{
	return add_into_shash(cur_scope()->right, name, kind, type);
}

/* :: Scope Stack */
/* ASSUMPTION: sslhdr->right means CURRENT sstackhdr */
static struct sstackhdr* new_sstackhdr()
{
	struct sstackhdr* p = new_snode();

	p->kind = SK_SSTACKHDR;
	
	return p;
}

void display_scopes()
{
	struct sstackhdr* p = sslhdr;
	while (p)
	{
		printf("%x -> ", (unsigned int)p);
		p = p->right;
	}
	printf("\n");
}

/* create a new scope, just after the header link[sslhdr]! */
struct sstackhdr* new_scope()
{
	struct sstackhdr *p = new_sstackhdr();
	
	/* link to the scope header */
	p->right = sslhdr->right;
	p->left = sslhdr;
	sslhdr->right = p;
	if (p->right)
		p->right->left = p;

	cur_scope_depth++;
	return p; /* Now it's the current */
}

/* get the current scope node */
inline struct sstackhdr* cur_sstack()
{
	return sslhdr->right;
}

/* delete CURRENT scope, returns 0 if succ, -1 if failed*/
int del_current_scope()
{

	struct sstackhdr* cur = cur_sstack();
	/* oops, no stack at all~ */
	if (!cur)
		return -1;
	
	/* escape the header first */
	cur = cur->down;
	
	/* travel and free nodes through the up-side-down link-lst */
	while (cur)
	{
		struct snode* down = cur->down;
		cur->left->right = cur->right;
		if (cur->right)
			cur->right->left = cur->left;

		cur->up->down = cur->down;
		if (cur->down)
			cur->down->up = cur->up;

		free(cur); /* TODO: IT's DANGEROUS! Cuz tnodes aren't released! */
		/* TODO: It can be solved by considering current-scope-depth */

		cur = down;
	}

	/* kill the hdr itself */
	cur = cur_scope();
	sslhdr->right = cur->right;
	if (cur->right)
		cur->right->left = sslhdr;
	free(cur);

	cur_scope_depth--;
	return 0;
}

/* :: TYPE NODE */

/* create a tnode specifying its kind */
inline struct tnode* new_tnode(enum typekind kind)
{
	struct tnode* n = MALLOC(struct tnode);
	n->width = 4; /* defualt: INT/FLOAT */
	n->kind = kind;
	return n;
}

/* try to delete a tnode, 0 if succ */
static int del_tnode(struct tnode *n)
{
	return -1; /*TODO */
}

/* create an array made of basic type */
inline struct tnode* new_tnode_arr_by_basic(enum typekind kind, int size)
{
	return new_tnode_arr_by_tnode(new_tnode_basic(kind), size);
}

/* create an array made of a tnode */
inline struct tnode* new_tnode_arr_by_tnode(struct tnode* sub, int size)
{
	struct tnode* n = new_tnode(TK_ARRAY);
	n->array.type = sub;
	n->array.size = size;
	n->width = size * sub->width;
	printd("arr-width: %d\n", n->width);
	return n;
}

/* create a struct tnode, specifying fieldlst */
struct tnode* new_tnode_struct(struct tfield_lst* field)
{
	struct tnode* n = new_tnode(TK_STRUCT);
	n->field_lst = field;
	printd("struct-width=%d\n", field->width);
	n->width = field->width;
	return n;
}

/* create a field-lst and link it on */
struct tfield_lst* new_fieldlst_link(struct tnode* n, char* name, struct tfield_lst* next)
{
	struct tfield_lst* f = MALLOC(struct tfield_lst);
	f->name = name;
	f->type = n;
	f->next = next;
	/* calc width recursively */
	f->offset = (next ? next->width : 0);
	f->width = f->offset + n->width; 
	printd("field-width: %d; offset=%d\n", f->width, f->offset);
	return f;
}

/* create a func, specifying return type and param-link-lst */
struct func* new_func(struct tnode* return_type, struct tfield_lst* param, int func_defined)
{
	struct func* f = MALLOC(struct func);
	f->return_type = return_type;
	f->param = param;
	f->defined = func_defined;

	return f;
}

/* delete a func */
void del_func(struct func* f)
{
	struct tfield_lst *i = f->param;
	while(i)
	{
		struct tfield_lst *t = i->next;
		free(i);
		i = t;
	}
	del_tnode(f->return_type);
	free(f);
	
}

/* create a tnode made of func */
struct tnode* new_tnode_func(struct func* f)
{
	struct tnode* n = new_tnode(TK_FUNC);
	n->function = f;

	return n;
}

/* judge whether 2 types are same, 0 if same */
int typecmp(struct tnode* n1, struct tnode* n2)
{
	/* ptr is the same, of course it is the same */
	if (n1 == n2)
		return 0;
	
	/* not the same kind, fail */
	if (n1->kind != n2->kind)
		return 1;
	
	if (n1->kind == TK_ARRAY)
		return typecmp(n1->array.type, n2->array.type);
	if (n1->kind == TK_STRUCT)
		return fieldcmp(n1->field_lst, n2->field_lst);
}

/* judge whether 2 field-lst are same, 0 if same */
int fieldcmp(struct tfield_lst* f1, struct tfield_lst* f2)
{
	while (f1 && f2)
	{
		if (typecmp(f1->type, f2->type)) /* not equal! */
			return 1;
		f1 = f1->next;
		f2 = f2->next;
	}
	if (f1 || f2) /* not in the same length */
		return 1;
	return 0;
}

/* judge whether 2 funcs are same, 0 if same */
int funccmp(struct func* f1, struct func* f2)
{
	if (typecmp(f1->return_type, f2->return_type) != 0)
		return 1;
	if (fieldcmp(f1->param, f2->param) != 0)
		return 1;
	return 0;
}

/* search a field-lst for one ID */
struct tfield_lst* find_field(struct tfield_lst* field, char* name)
{
	while (field)
	{
		if (strcmp(name, field->name) == 0) /* if found */
			return field;
		field = field->next;
	}
	return  NULL; /* not found */
}

/* create a field-list from current scope */
struct tfield_lst* create_fieldlst_from_cur_scope()
{
	struct snode* i = cur_scope()->down;
	struct tfield_lst *p = NULL, *q;
	while (i)
	{
		q = new_fieldlst_link(i->type, i->name, p);
		p = q;
		i = i->down;
	}
	return p;
}

/* :: OVERALL functions */
void init_stable()
{
	int i;
	sslhdr = new_sstackhdr();
	/* sslhdr->left = sslhdr; */
	for (i = 0; i < HASH_LENGTH; i++)
		shash[i].left = shash[i].right = NULL;
}
