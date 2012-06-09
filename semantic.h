/* Created on Apr. 27th, 2012 by Sunicy Tao */

/* This file aims to do the semantic analysis,
	with symbol table and grammer tree
*/

#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

#define SNERR_VAR_UNDEFINED 1
#define SNERR_FUNC_UNDEFINED 2
#define SNERR_VAR_MULTIDEFINED 3
#define SNERR_FUNC_MULTIDEFINED 4
#define SNERR_ASSIGN_TYPE_UNMATCH 5
#define SNERR_RVALUE_BEFORE_ASSIGNOP 6
#define SNERR_TYPE_UNMATCH 7
#define SNERR_RETURN_TYPE_UNMATCH 8
#define SNERR_FUNC_ARG_UNMATCH 9
#define SNERR_ARRAY_TYPE_REQUIRED 10
#define SNERR_USE_VAR_AS_FUNC 11
#define SNERR_INVALID_ARRAY_INDEX 12
#define SNERR_USE_VAR_AS_STRUCT 13
#define SNERR_INVALID_STRUCT_SCOPE 14
#define SNERR_INVALID_DEF_IN_STRUCT 15
#define SNERR_STRUCT_MULTIDEFINED 16
#define SNERR_STRUCT_UNDEFINED 17
#define SNERR_FUNC_DEC_UNDEFINED 18
#define SNERR_FUNC_DEC_DEF_MISMATCH 19

#define SNERR_COND_WRONG_TYPE 30


#define SNERR_VAR_UNDEFINED_MSG				"Variable undefined" 
#define SNERR_FUNC_UNDEFINED_MSG			"Function undefined"
#define SNERR_VAR_MULTIDEFINED_MSG			"Variable multidefined"
#define SNERR_FUNC_MULTIDEFINED_MSG			"Function multidefined"
#define SNERR_ASSIGN_TYPE_UNMATCH_MSG		"Incompatible types when assigning"
#define SNERR_RVALUE_BEFORE_ASSIGNOP_MSG	"Lvalue required"
#define SNERR_TYPE_UNMATCH_MSG				"Incompatible types"
#define SNERR_RETURN_TYPE_UNMATCH_MSG		"Incompatible type for 'return'"
#define SNERR_FUNC_ARG_UNMATCH_MSG			"Incompatible function argments"
#define SNERR_ARRAY_TYPE_REQUIRED_MSG		"Array type required"
#define SNERR_USE_VAR_AS_FUNC_MSG			"Do not call a variable"
#define SNERR_INVALID_ARRAY_INDEX_MSG		"Invalid array index (Please use an integer expr)"
#define SNERR_USE_VAR_AS_STRUCT_MSG			"Struct type is required"
#define SNERR_INVALID_STRUCT_SCOPE_MSG		"Invalid struct scope"
#define SNERR_INVALID_DEF_IN_STRUCT_MSG		"Invalid definition in struct" 
#define SNERR_STRUCT_MULTIDEFINED_MSG		"Struct multidefined"
#define SNERR_STRUCT_UNDEFINED_MSG			"Struct undefined"
#define SNERR_FUNC_DEC_UNDEFINED_MSG		"Function declared but not defined" 
#define SNERR_FUNC_DEC_DEF_MISMATCH_MSG		"Function declaration and definition mismatched"

#define SNERR_COND_WRONG_TYPE_MSG 			"Invalid condition expression"
/* public: start it! 0 for succ*/
int semantic_parse();

#endif
