
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "syntax.y"

#include <stdio.h>
#include "lexical.c"
#include "gtree.h"
#include "std.h"

int error_type = 0;
static union gnode_val gval;

#define FORCE_OUTPUT 1

#define pm(x) printf("%s ( %d )\n", x, yylineno)

/* assert gtree */
#define _AG \
			if (error_type != 0) \
				break
/* _ANS is short for New a Nonterminal Symbol */
#define _NNS(symbol,rtype) \
	gt_new_node(#symbol, yyloc.first_line, gval, NT_##symbol,rtype)/*, printf("%s at Line %d\n", #symbol, yylineno)*/

/* _ATS is short for New a Terminal Symbol */
#define _NTS(symbol) \
	/*printf("SYMBOL: %s; VAL: %d\n", #symbol, gval.val_int), */gt_new_node(#symbol, yylineno, gval, NT_##symbol, RT_TERMINAL)

/* _AS is short for Append a Sibling */
#define _AS(p, cur) \
	gt_append_sibling(p, cur)

/* _AC is short for Append a Child */
#define _AC(p, cur) \
	gt_append_child(p, cur)



/* Line 189 of yacc.c  */
#line 109 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LOWEST = 258,
     COMMA = 259,
     SEMI = 260,
     LC = 261,
     RC = 262,
     INT = 263,
     FLOAT = 264,
     ID = 265,
     SIMPLE_IF_STMT = 266,
     ELSE = 267,
     STRUCT = 268,
     RETURN = 269,
     IF = 270,
     WHILE = 271,
     ASSIGNOP = 272,
     OR = 273,
     AND = 274,
     RELOP = 275,
     MINUS = 276,
     PLUS = 277,
     STAR = 278,
     DIV = 279,
     NOT = 280,
     MINUS_NUM = 281,
     TYPE = 282,
     DOT = 283,
     RB = 284,
     LB = 285,
     RP = 286,
     LP = 287
   };
#endif
/* Tokens.  */
#define LOWEST 258
#define COMMA 259
#define SEMI 260
#define LC 261
#define RC 262
#define INT 263
#define FLOAT 264
#define ID 265
#define SIMPLE_IF_STMT 266
#define ELSE 267
#define STRUCT 268
#define RETURN 269
#define IF 270
#define WHILE 271
#define ASSIGNOP 272
#define OR 273
#define AND 274
#define RELOP 275
#define MINUS 276
#define PLUS 277
#define STAR 278
#define DIV 279
#define NOT 280
#define MINUS_NUM 281
#define TYPE 282
#define DOT 283
#define RB 284
#define LB 285
#define RP 286
#define LP 287




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 38 "syntax.y"

	int type_int;
	float type_float;
	char * type_str;
	enum relop type_rel;
	struct gnode* gnode;



/* Line 214 of yacc.c  */
#line 219 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 244 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   280

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  67
/* YYNRULES -- Number of states.  */
#define YYNSTATES  120

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    12,    16,    19,    23,
      27,    31,    33,    37,    39,    41,    47,    50,    52,    53,
      55,    57,    62,    67,    71,    75,    77,    80,    85,    88,
      89,    92,    94,    98,   104,   112,   118,   120,   123,   126,
     129,   130,   134,   138,   140,   144,   146,   150,   154,   158,
     162,   166,   170,   174,   178,   182,   186,   189,   192,   197,
     201,   206,   210,   212,   214,   216,   220,   224
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      34,     0,    -1,    35,    -1,    35,     1,    -1,    36,    35,
      -1,    -1,    38,    37,     5,    -1,    38,     5,    -1,    38,
      43,    46,    -1,    38,    43,     5,    -1,    38,    37,     1,
      -1,    42,    -1,    42,     4,    37,    -1,    27,    -1,    39,
      -1,    13,    40,     6,    49,     7,    -1,    13,    41,    -1,
      10,    -1,    -1,    10,    -1,    10,    -1,    42,    30,     8,
      29,    -1,    10,    32,    44,    31,    -1,    10,    32,    31,
      -1,    45,     4,    44,    -1,    45,    -1,    38,    42,    -1,
       6,    49,    47,     7,    -1,    48,    47,    -1,    -1,    53,
       5,    -1,    46,    -1,    14,    53,     5,    -1,    15,    32,
      53,    31,    48,    -1,    15,    32,    53,    31,    48,    12,
      48,    -1,    16,    32,    53,    31,    48,    -1,     5,    -1,
       1,     5,    -1,    53,     1,    -1,    50,    49,    -1,    -1,
      38,    51,     5,    -1,    38,    51,     1,    -1,    52,    -1,
      52,     4,    51,    -1,    42,    -1,    42,    17,    53,    -1,
      53,    17,    53,    -1,    53,    19,    53,    -1,    53,    18,
      53,    -1,    53,    20,    53,    -1,    53,    22,    53,    -1,
      53,    21,    53,    -1,    53,    23,    53,    -1,    53,    24,
      53,    -1,    32,    53,    31,    -1,    21,    53,    -1,    25,
      53,    -1,    10,    32,    54,    31,    -1,    10,    32,    31,
      -1,    53,    30,    53,    29,    -1,    53,    28,    10,    -1,
      10,    -1,     8,    -1,     9,    -1,    53,    30,     1,    -1,
      53,     4,    54,    -1,    53,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    69,    69,    83,    88,    89,    91,    92,    93,    94,
      95,    97,    98,   101,   106,   108,   109,   111,   116,   118,
     125,   130,   136,   141,   147,   148,   150,   154,   156,   157,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   170,
     171,   173,   174,   176,   177,   179,   180,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   199,
     204,   205,   210,   215,   220,   225,   230,   231
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LOWEST", "COMMA", "SEMI", "LC", "RC",
  "INT", "FLOAT", "ID", "SIMPLE_IF_STMT", "ELSE", "STRUCT", "RETURN", "IF",
  "WHILE", "ASSIGNOP", "OR", "AND", "RELOP", "MINUS", "PLUS", "STAR",
  "DIV", "NOT", "MINUS_NUM", "TYPE", "DOT", "RB", "LB", "RP", "LP",
  "$accept", "Program", "ExtDefList", "ExtDef", "ExtDecList", "Specifier",
  "StructSpecifier", "OptTag", "Tag", "VarDec", "FunDec", "VarList",
  "ParamDec", "CompSt", "StmtList", "Stmt", "DefList", "Def", "DecList",
  "Dec", "Exp", "Args", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    33,    34,    34,    35,    35,    36,    36,    36,    36,
      36,    37,    37,    38,    38,    39,    39,    40,    40,    41,
      42,    42,    43,    43,    44,    44,    45,    46,    47,    47,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    49,
      49,    50,    50,    51,    51,    52,    52,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    54,    54
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     0,     3,     2,     3,     3,
       3,     1,     3,     1,     1,     5,     2,     1,     0,     1,
       1,     4,     4,     3,     3,     1,     2,     4,     2,     0,
       2,     1,     3,     5,     7,     5,     1,     2,     2,     2,
       0,     3,     3,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     4,     3,
       4,     3,     1,     1,     1,     3,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,    18,    13,     0,     0,     5,     0,    14,    19,     0,
      16,     1,     3,     4,     7,    20,     0,    11,     0,    40,
       0,    10,     6,     0,     0,     9,    40,     8,     0,     0,
      40,    23,     0,     0,    25,    20,    12,     0,     0,    45,
       0,    43,    15,    39,    26,    22,     0,    21,     0,    36,
      63,    64,    62,     0,     0,     0,     0,     0,     0,    31,
       0,     0,     0,     0,    42,    41,     0,    24,    37,     0,
       0,     0,     0,    56,    57,     0,    27,    28,    38,    30,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      46,    44,    59,    67,     0,    32,     0,     0,    55,    47,
      49,    48,    50,    52,    51,    53,    54,    61,    65,     0,
       0,    58,     0,     0,    60,    66,    33,    35,     0,    34
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,    16,    28,     7,     9,    10,    17,
      18,    33,    34,    59,    60,    61,    29,    30,    40,    41,
      62,    94
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -61
static const yytype_int16 yypact[] =
{
      -4,     4,   -61,    24,    67,    -4,    15,   -61,    31,    40,
     -61,   -61,   -61,   -61,   -61,    19,    12,     8,    91,    -4,
      -5,   -61,   -61,    44,    54,   -61,    -4,   -61,    44,    68,
      -4,   -61,    44,    47,    82,   -61,   -61,    64,    34,   -15,
      83,    95,   -61,   -61,    70,   -61,    -4,   -61,    96,   -61,
     -61,   -61,    71,   154,    72,    74,   154,   154,   154,   -61,
     109,    34,    90,   154,   -61,   -61,    44,   -61,   -61,   135,
     131,   154,   154,    62,    62,   170,   -61,   -61,   -61,   -61,
     154,   154,   154,   154,   154,   154,   154,   154,    92,    73,
     229,   -61,   -61,   111,    86,   -61,   185,   200,   -61,   229,
     150,   240,   250,    49,    49,    62,    62,   -61,   -61,   215,
     154,   -61,    55,    55,   -61,   -61,   107,   -61,    55,   -61
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -61,   -61,   116,   -61,    99,     1,   -61,   -61,   -61,   -21,
     -61,    77,   -61,   106,    65,   -60,    59,   -61,    61,   -61,
     -53,    27
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -30
static const yytype_int8 yytable[] =
{
      70,     6,    63,    73,    74,    75,     6,    39,     1,     1,
      90,    44,    23,    21,     8,    24,    93,    22,    96,    97,
      14,    32,     2,     2,    11,    15,    31,    99,   100,   101,
     102,   103,   104,   105,   106,    48,   109,   -17,    24,    49,
      26,   -29,    50,    51,    52,    39,    19,    32,    53,    54,
      55,    20,   116,   117,    35,    56,    48,    93,   119,    57,
      49,    26,    37,    50,    51,    52,    58,    -2,    12,    53,
      54,    55,    86,    87,   108,    42,    56,    88,    45,    89,
      57,    50,    51,    52,    64,    38,    46,    58,    65,    43,
      88,    78,    89,    47,    56,    79,    25,    26,    57,    66,
      24,    68,   107,    69,    71,    58,    72,    80,    81,    82,
      83,    84,    85,    86,    87,   110,    76,   111,    88,   118,
      89,    13,    36,    67,    27,     0,    77,    91,    80,    81,
      82,    83,    84,    85,    86,    87,    95,   115,     0,    88,
       0,    89,     0,    50,    51,    52,     0,     0,    80,    81,
      82,    83,    84,    85,    86,    87,    56,     0,     0,    88,
      57,    89,    50,    51,    52,     0,    92,    58,     0,    82,
      83,    84,    85,    86,    87,    56,     0,     0,    88,    57,
      89,     0,     0,     0,     0,     0,    58,    80,    81,    82,
      83,    84,    85,    86,    87,     0,     0,     0,    88,     0,
      89,    98,    80,    81,    82,    83,    84,    85,    86,    87,
       0,     0,     0,    88,     0,    89,   112,    80,    81,    82,
      83,    84,    85,    86,    87,     0,     0,     0,    88,     0,
      89,   113,    80,    81,    82,    83,    84,    85,    86,    87,
       0,     0,     0,    88,   114,    89,    80,    81,    82,    83,
      84,    85,    86,    87,     0,     0,     0,    88,     0,    89,
      83,    84,    85,    86,    87,     0,     0,     0,    88,     0,
      89,    84,    85,    86,    87,     0,     0,     0,    88,     0,
      89
};

static const yytype_int8 yycheck[] =
{
      53,     0,    17,    56,    57,    58,     5,    28,    13,    13,
      63,    32,     4,     1,    10,    30,    69,     5,    71,    72,
       5,    20,    27,    27,     0,    10,    31,    80,    81,    82,
      83,    84,    85,    86,    87,     1,    89,     6,    30,     5,
       6,     7,     8,     9,    10,    66,     6,    46,    14,    15,
      16,    32,   112,   113,    10,    21,     1,   110,   118,    25,
       5,     6,     8,     8,     9,    10,    32,     0,     1,    14,
      15,    16,    23,    24,     1,     7,    21,    28,    31,    30,
      25,     8,     9,    10,     1,    26,     4,    32,     5,    30,
      28,     1,    30,    29,    21,     5,     5,     6,    25,     4,
      30,     5,    10,    32,    32,    32,    32,    17,    18,    19,
      20,    21,    22,    23,    24,     4,     7,    31,    28,    12,
      30,     5,    23,    46,    18,    -1,    61,    66,    17,    18,
      19,    20,    21,    22,    23,    24,     5,   110,    -1,    28,
      -1,    30,    -1,     8,     9,    10,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    21,    -1,    -1,    28,
      25,    30,     8,     9,    10,    -1,    31,    32,    -1,    19,
      20,    21,    22,    23,    24,    21,    -1,    -1,    28,    25,
      30,    -1,    -1,    -1,    -1,    -1,    32,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    28,    -1,
      30,    31,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    28,    -1,    30,    31,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    28,    -1,
      30,    31,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    28,    29,    30,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    28,    -1,    30,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    28,    -1,
      30,    21,    22,    23,    24,    -1,    -1,    -1,    28,    -1,
      30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    13,    27,    34,    35,    36,    38,    39,    10,    40,
      41,     0,     1,    35,     5,    10,    37,    42,    43,     6,
      32,     1,     5,     4,    30,     5,     6,    46,    38,    49,
      50,    31,    38,    44,    45,    10,    37,     8,    49,    42,
      51,    52,     7,    49,    42,    31,     4,    29,     1,     5,
       8,     9,    10,    14,    15,    16,    21,    25,    32,    46,
      47,    48,    53,    17,     1,     5,     4,    44,     5,    32,
      53,    32,    32,    53,    53,    53,     7,    47,     1,     5,
      17,    18,    19,    20,    21,    22,    23,    24,    28,    30,
      53,    51,    31,    53,    54,     5,    53,    53,    31,    53,
      53,    53,    53,    53,    53,    53,    53,    10,     1,    53,
       4,    31,    31,    31,    29,    54,    48,    48,    12,    48
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 69 "syntax.y"
    {
								_AG;
								(yyval.gnode) =_NNS(Program,RT_Program_ExtDefList); 
								_AC((yyval.gnode),(yyvsp[(1) - (1)].gnode));
								if (!error_type)
								{
									#ifdef PRINT_TREE
										gt_display((yyval.gnode),0);
									#endif
									gtroot = (yyval.gnode);
								}
								else
									gtroot = NULL;
							}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 83 "syntax.y"
    {
										(yyval.gnode) = NULL;
										printf("Error type %d at line %d: fatal error, is this the end of program?\n", error_type = 2, yylineno);
									}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 88 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(ExtDefList,RT_ExtDefList_ExtDef_ExtDefList); _AS(_AC((yyval.gnode),(yyvsp[(1) - (2)].gnode)),(yyvsp[(2) - (2)].gnode));}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 89 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(ExtDefList,RT_ExtDefList_NULL);/*_AC($$,NULL);*/}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 91 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(ExtDef,RT_ExtDef_Specifier_ExtDecList_SEMI); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),(yyvsp[(2) - (3)].gnode)),_NTS(SEMI));}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 92 "syntax.y"
    {_AG;_AG;(yyval.gnode) = _NNS(ExtDef,RT_ExtDef_Specifier_SEMI); _AS(_AC((yyval.gnode),(yyvsp[(1) - (2)].gnode)),_NTS(SEMI));}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 93 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(ExtDef,RT_ExtDef_Specifier_FunDec_CompSt); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),(yyvsp[(2) - (3)].gnode)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 94 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(ExtDef,RT_ExtDef_Specifier_FunDec_SEMI); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),(yyvsp[(2) - (3)].gnode)),_NTS(SEMI));}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 95 "syntax.y"
    {(yyval.gnode) = NULL; printf("Error type %d at line %d: ';' expected\n", error_type = 2, yylineno);}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 97 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(ExtDecList,RT_ExtDecList_VarDec); _AC((yyval.gnode),(yyvsp[(1) - (1)].gnode));}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 98 "syntax.y"
    {_AG;(yyval.gnode) =  _NNS(ExtDecList,RT_ExtDecList_VarDec_COMMA_ExtDecList); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(COMMA)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 101 "syntax.y"
    {
							_AG;(yyval.gnode) = _NNS(Specifier,RT_Specifier_TYPE); 
							gval.val_str = (yyvsp[(1) - (1)].type_str);
							_AC((yyval.gnode),_NTS(TYPE));
						}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 106 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Specifier,RT_Specifier_StructSpecifier); _AC((yyval.gnode),(yyvsp[(1) - (1)].gnode));}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 108 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(StructSpecifier,RT_StructSpecifier_STRUCT_OptTag_LC_DefList_RC); _AS(_AS(_AS(_AS(_AC((yyval.gnode),_NTS(STRUCT)),(yyvsp[(2) - (5)].gnode)),_NTS(LC)),(yyvsp[(4) - (5)].gnode)),_NTS(RC));}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 109 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(StructSpecifier,RT_StructSpecifier_STRUCT_Tag); _AS(_AC((yyval.gnode),_NTS(STRUCT)),(yyvsp[(2) - (2)].gnode));}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 111 "syntax.y"
    {
						_AG;(yyval.gnode) = _NNS(OptTag,RT_OptTag_ID);
						gval.val_str = (yyvsp[(1) - (1)].type_str);
						_AC((yyval.gnode),_NTS(ID));
					}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 116 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(OptTag,RT_OptTag_NULL);_AC((yyval.gnode),NULL);}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 118 "syntax.y"
    {
						_AG;(yyval.gnode) = _NNS(Tag,RT_Tag_ID);
						gval.val_str = (yyvsp[(1) - (1)].type_str);
						_AC((yyval.gnode),_NTS(ID));
					}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 125 "syntax.y"
    {
						_AG;(yyval.gnode) = _NNS(VarDec,RT_VarDec_ID);
						gval.val_str = (yyvsp[(1) - (1)].type_str);
						_AC((yyval.gnode),_NTS(ID));
					}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 130 "syntax.y"
    {
									_AG;(yyval.gnode) = _NNS(VarDec,RT_VarDec_VarDec_LB_INT_RB);
									gval.val_int = (yyvsp[(3) - (4)].type_int);
									_AS(_AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (4)].gnode)),_NTS(LB)),_NTS(INT)),_NTS(RB));
								}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 136 "syntax.y"
    {
									_AG;(yyval.gnode) = _NNS(FunDec,RT_FunDec_ID_LP_VarList_RP);
									gval.val_str = (yyvsp[(1) - (4)].type_str);
									_AS(_AS(_AS(_AC((yyval.gnode),_NTS(ID)),_NTS(LP)), (yyvsp[(3) - (4)].gnode)),_NTS(RP));
								}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 141 "syntax.y"
    {
								_AG;(yyval.gnode) = _NNS(FunDec,RT_FunDec_ID_LP_RP);
								gval.val_str = (yyvsp[(1) - (3)].type_str);
								_AS(_AS(_AC((yyval.gnode),_NTS(ID)),_NTS(LP)),_NTS(RP));
							}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 147 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(VarList,RT_VarList_ParamDec_COMMA_VarList); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(COMMA)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 148 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(VarList,RT_VarList_ParamDec); _AC((yyval.gnode),(yyvsp[(1) - (1)].gnode));}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 150 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(ParamDec,RT_ParamDec_Specifier_VarDec); _AS(_AC((yyval.gnode),(yyvsp[(1) - (2)].gnode)),(yyvsp[(2) - (2)].gnode));}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 154 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(CompSt,RT_CompSt_LC_DefList_StmtList_RC); _AS(_AS(_AS(_AC((yyval.gnode),_NTS(LC)),(yyvsp[(2) - (4)].gnode)),(yyvsp[(3) - (4)].gnode)),_NTS(RC));}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 156 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(StmtList,RT_StmtList_Stmt_StmtList); _AS(_AC((yyval.gnode),(yyvsp[(1) - (2)].gnode)),(yyvsp[(2) - (2)].gnode));}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 157 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(StmtList,RT_StmtList_NULL); /*_AC($$,NULL);*/}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 159 "syntax.y"
    {_AG;(yyval.gnode) =_NNS(Stmt,RT_Stmt_Exp_SEMI); _AS(_AC((yyval.gnode),(yyvsp[(1) - (2)].gnode)),_NTS(SEMI));}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 160 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Stmt,RT_Stmt_CompSt); _AC((yyval.gnode),(yyvsp[(1) - (1)].gnode));}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 161 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Stmt,RT_Stmt_RETURN_Exp_SEMI); _AS(_AS(_AC((yyval.gnode),_NTS(RETURN)),(yyvsp[(2) - (3)].gnode)),_NTS(SEMI));}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 162 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Stmt,RT_Stmt_IF_LP_Exp_RP_Stmt); _AS(_AS(_AS(_AS(_AC((yyval.gnode),_NTS(IF)),_NTS(LP)),(yyvsp[(3) - (5)].gnode)),_NTS(RP)),(yyvsp[(5) - (5)].gnode));}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 163 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Stmt,RT_Stmt_IF_LP_Exp_RP_Stmt_ELSE_Stmt); _AS(_AS(_AS(_AS(_AS(_AS(_AC((yyval.gnode),_NTS(IF)),_NTS(LP)),(yyvsp[(3) - (7)].gnode)),_NTS(RP)),(yyvsp[(5) - (7)].gnode)),_NTS(ELSE)),(yyvsp[(7) - (7)].gnode));}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 164 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Stmt,RT_Stmt_WHILE_LP_Exp_RP_Stmt); _AS(_AS(_AS(_AS(_AC((yyval.gnode),_NTS(WHILE)),_NTS(LP)),(yyvsp[(3) - (5)].gnode)),_NTS(RP)),(yyvsp[(5) - (5)].gnode));}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 165 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Stmt,RT_Stmt_SEMI); _AC((yyval.gnode),_NTS(SEMI));}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 166 "syntax.y"
    {(yyval.gnode) = NULL;printf("Error type %d at line %d: Syntax error.\n", error_type = 2, yylineno);}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 167 "syntax.y"
    {(yyval.gnode) = NULL;printf("Error type %d at line %d: (';' expected ?) unexpected '%s'.\n", error_type = 2, yylineno, yytext);yyerrok;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 170 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(DefList,RT_DefList_Def_DefList); _AS(_AC((yyval.gnode),(yyvsp[(1) - (2)].gnode)),(yyvsp[(2) - (2)].gnode));}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 171 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(DefList,RT_DefList_NULL);/*_AC($$,NULL);*/}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 173 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Def,RT_Def_Specifier_DecList_SEMI); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),(yyvsp[(2) - (3)].gnode)),_NTS(SEMI));}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 174 "syntax.y"
    {(yyval.gnode) = NULL; printf("Error type %d at line %d: ';' expected\n", error_type = 2, yylineno);}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 176 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(DecList,RT_DecList_Dec); _AC((yyval.gnode),(yyvsp[(1) - (1)].gnode));}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 177 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(DecList,RT_DecList_Dec_COMMA_DecList); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(COMMA)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 179 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Dec,RT_Dec_VarDec); _AC((yyval.gnode),(yyvsp[(1) - (1)].gnode));}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 180 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Dec,RT_Dec_VarDec_ASSIGNOP_Exp); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(ASSIGNOP)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 183 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_Exp_ASSIGNOP_Exp); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(ASSIGNOP)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 184 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_Exp_AND_Exp); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(AND)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 185 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_Exp_OR_Exp); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(OR)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 186 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_Exp_RELOP_Exp); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(RELOP)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 187 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_Exp_PLUS_Exp); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(PLUS)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 188 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_Exp_MINUS_Exp); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(MINUS)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 189 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_Exp_STAR_Exp); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(STAR)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 190 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_Exp_DIV_Exp); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(DIV)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 191 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_LP_Exp_RP); _AS(_AS(_AC((yyval.gnode),_NTS(LP)),(yyvsp[(2) - (3)].gnode)),_NTS(RP));}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 192 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_MINUS_Exp); _AS(_AC((yyval.gnode),_NTS(MINUS)),(yyvsp[(2) - (2)].gnode));}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 193 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_NOT_Exp); _AS(_AC((yyval.gnode),_NTS(NOT)),(yyvsp[(2) - (2)].gnode));}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 194 "syntax.y"
    { 
									_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_ID_LP_Args_RP);
									gval.val_str = (yyvsp[(1) - (4)].type_str);
									_AS(_AS(_AS(_AC((yyval.gnode),_NTS(ID)),_NTS(LP)),(yyvsp[(3) - (4)].gnode)),_NTS(RP));
								}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 199 "syntax.y"
    {
								_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_ID_LP_RP);
								gval.val_str = (yyvsp[(1) - (3)].type_str);
								_AS(_AS(_AC((yyval.gnode),_NTS(ID)),_NTS(LP)),_NTS(RP));
							}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 204 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_Exp_LB_Exp_RB); _AS(_AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (4)].gnode)),_NTS(LB)),(yyvsp[(3) - (4)].gnode)),_NTS(RB));}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 205 "syntax.y"
    {
								_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_Exp_DOT_ID);
								gval.val_str = (yyvsp[(3) - (3)].type_str);
								_AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(DOT)),_NTS(ID));
							}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 210 "syntax.y"
    {
					gval.val_str = (yyvsp[(1) - (1)].type_str);
					_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_ID);
					_AC((yyval.gnode),_NTS(ID));
				}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 215 "syntax.y"
    {
					gval.val_int = (yyvsp[(1) - (1)].type_int);
					_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_INT);
					_AC((yyval.gnode),_NTS(INT));
				}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 220 "syntax.y"
    {
					gval.val_float = (yyvsp[(1) - (1)].type_float);
					_AG;(yyval.gnode) = _NNS(Exp,RT_Exp_FLOAT);
					_AC((yyval.gnode),_NTS(FLOAT));
				}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 225 "syntax.y"
    {
					(yyval.gnode) = NULL;
					printf("Error type %d at line %d: parentheses error\n", error_type = 2, yylineno);
				}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 230 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Args,RT_Args_Exp_COMMA_Args); _AS(_AS(_AC((yyval.gnode),(yyvsp[(1) - (3)].gnode)),_NTS(COMMA)),(yyvsp[(3) - (3)].gnode));}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 231 "syntax.y"
    {_AG;(yyval.gnode) = _NNS(Args,RT_Args_Exp); _AC((yyval.gnode),(yyvsp[(1) - (1)].gnode));}
    break;



/* Line 1455 of yacc.c  */
#line 2142 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 233 "syntax.y"


yyerror(char* msg)
{
}

