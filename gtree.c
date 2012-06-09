#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "syntax.tab.h"
#include "gtree.h"

#define SPACE_WIDTH 2
/* allocate memory for new node */ 
ptr_gnode gt_new_node(char* type_name, int lineno, union gnode_val val,
						enum nodetype type, enum reducetype rtype)
{
	ptr_gnode node = (struct gnode*)malloc(sizeof(struct gnode));

	node->type = type;
	node->type_name = type_name;
	node->val = val;
	node->lineno = lineno;
	node->rtype = rtype;
	node->sibling = node->child = NULL;
	node->vtype = NULL;
	node->lvalue = LV_UNKNOWN;
	node->struct_def = 0;
	return node;
}

/* append the current gnode to one gnode as a succ sibling */
/* CAUTION: not INSERT! Original succ sibling will be lost! */
ptr_gnode gt_append_sibling(ptr_gnode prev_sibling, ptr_gnode cur_node)
{
	prev_sibling->sibling = cur_node;
	return cur_node;
}

/* append the current gnode to one gnode as a Child */
ptr_gnode gt_append_child(ptr_gnode parent, ptr_gnode cur_node)
{
	parent->child = cur_node;
	return cur_node;
}

void gt_print_space(int level)
{
	int i;
	for (i = level * SPACE_WIDTH; i > 0; i--)
		printf(" ");
}

/* recursively print gtree (p: Non-NULL garenteed) */
/* level = 0 at the very beginning */
void gt_display(ptr_gnode p, int level)
{
	/* print itself */
	gt_print_space(level);
//	printf("[self: %d]", p);
	printf("%s [%d]", p->type_name, p->rtype);
//	printf("[child:%d; sibling:%d]", p->child, p->sibling);
	/* if a non-terminal symbol(has at least a child) */
	if (p->child != NULL)
	{
		printf(" (%d)\n", p->lineno);
		gt_display(p->child, level + 1);
	}
	/* otherwise, maybe we should have some details! */
	else if (p->type == NT_ID) 
		printf(": %s\n", p->val.val_str);
	else if (p->type == NT_INT)
		printf(": %d\n", p->val.val_int);
	else if (p->type == NT_FLOAT)
		printf(": %f\n", p->val.val_float);
	else if (p->type == NT_TYPE)
		printf(": %s\n", p->val.val_str);
	else if (p->type == NT_RELOP)
		printf(": %d\n", p->val.val_rel);
	else
		printf("\n");

	/* print siblings! */
	if (p->sibling != NULL)
		gt_display(p->sibling, level);
}
