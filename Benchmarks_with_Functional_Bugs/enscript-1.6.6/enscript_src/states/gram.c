/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "gram.y"

/* 								-*- c -*-
 * Grammar for states.
 * Copyright (c) 1997-1998 Markku Rossi.
 *
 * Author: Markku Rossi <mtr@iki.fi>
 */

/*
 * This file is part of GNU Enscript.
 *
 * Enscript is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Enscript is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Enscript.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * $Id: gram.y,v 1.1.1.1 2003/03/05 07:25:52 mtr Exp $
 */

#include "defs.h"


/* Line 268 of yacc.c  */
#line 104 "gram.c"

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
     tSYMBOL = 258,
     tREGEXP = 259,
     tSTRING = 260,
     tINTEGER = 261,
     tREAL = 262,
     tSUB = 263,
     tSTATE = 264,
     tSTART = 265,
     tSTARTRULES = 266,
     tNAMERULES = 267,
     tBEGIN = 268,
     tEND = 269,
     tRETURN = 270,
     tIF = 271,
     tELSE = 272,
     tLOCAL = 273,
     tWHILE = 274,
     tFOR = 275,
     tEXTENDS = 276,
     tDIVASSIGN = 277,
     tMULASSIGN = 278,
     tSUBASSIGN = 279,
     tADDASSIGN = 280,
     tOR = 281,
     tAND = 282,
     tNE = 283,
     tEQ = 284,
     tLE = 285,
     tGE = 286,
     tDIV = 287,
     tMINUSMINUS = 288,
     tPLUSPLUS = 289
   };
#endif
/* Tokens.  */
#define tSYMBOL 258
#define tREGEXP 259
#define tSTRING 260
#define tINTEGER 261
#define tREAL 262
#define tSUB 263
#define tSTATE 264
#define tSTART 265
#define tSTARTRULES 266
#define tNAMERULES 267
#define tBEGIN 268
#define tEND 269
#define tRETURN 270
#define tIF 271
#define tELSE 272
#define tLOCAL 273
#define tWHILE 274
#define tFOR 275
#define tEXTENDS 276
#define tDIVASSIGN 277
#define tMULASSIGN 278
#define tSUBASSIGN 279
#define tADDASSIGN 280
#define tOR 281
#define tAND 282
#define tNE 283
#define tEQ 284
#define tLE 285
#define tGE 286
#define tDIV 287
#define tMINUSMINUS 288
#define tPLUSPLUS 289




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 34 "gram.y"

  List *lst;
  Node *node;
  Cons *cons;
  Stmt *stmt;
  Expr *expr;



/* Line 293 of yacc.c  */
#line 218 "gram.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 230 "gram.c"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

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

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
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
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   571

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  76
/* YYNRULES -- Number of states.  */
#define YYNSTATES  167

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    41,     2,     2,     2,     2,     2,     2,
      50,    51,    39,    37,    49,    38,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    28,    48,
      33,    22,    34,    27,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    44,     2,    45,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    46,     2,    47,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    23,    24,    25,
      26,    29,    30,    31,    32,    35,    36,    40,    42,    43
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    12,    17,    22,    28,    36,
      38,    39,    44,    45,    48,    53,    58,    63,    68,    69,
      71,    73,    77,    78,    82,    84,    88,    90,    94,    95,
      98,   101,   105,   115,   119,   125,   133,   139,   149,   152,
     154,   156,   158,   160,   162,   165,   169,   173,   178,   182,
     186,   190,   194,   198,   201,   204,   207,   210,   217,   221,
     226,   232,   236,   240,   244,   248,   252,   256,   260,   264,
     268,   272,   273,   275,   276,   278,   280
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      53,     0,    -1,    -1,    53,    54,    -1,    10,    46,    63,
      47,    -1,    11,    46,    55,    47,    -1,    12,    46,    55,
      47,    -1,     9,     3,    46,    56,    47,    -1,     9,     3,
      21,     3,    46,    56,    47,    -1,    64,    -1,    -1,    55,
       4,     3,    48,    -1,    -1,    56,    57,    -1,    13,    46,
      63,    47,    -1,    14,    46,    63,    47,    -1,     4,    46,
      63,    47,    -1,     3,    46,    63,    47,    -1,    -1,    59,
      -1,     3,    -1,    59,    49,     3,    -1,    -1,    18,    61,
      48,    -1,    62,    -1,    61,    49,    62,    -1,     3,    -1,
       3,    22,    65,    -1,    -1,    63,    64,    -1,    15,    48,
      -1,    15,    65,    48,    -1,     8,     3,    50,    58,    51,
      46,    60,    63,    47,    -1,    46,    63,    47,    -1,    16,
      50,    65,    51,    64,    -1,    16,    50,    65,    51,    64,
      17,    64,    -1,    19,    50,    65,    51,    64,    -1,    20,
      50,    66,    48,    65,    48,    66,    51,    64,    -1,    65,
      48,    -1,     5,    -1,     4,    -1,     6,    -1,     7,    -1,
       3,    -1,    41,    65,    -1,    65,    30,    65,    -1,    65,
      29,    65,    -1,     3,    50,    67,    51,    -1,     3,    22,
      65,    -1,     3,    26,    65,    -1,     3,    25,    65,    -1,
       3,    24,    65,    -1,     3,    23,    65,    -1,     3,    43,
      -1,     3,    42,    -1,    43,     3,    -1,    42,     3,    -1,
      65,    44,    65,    45,    22,    65,    -1,    50,    65,    51,
      -1,    65,    44,    65,    45,    -1,    65,    27,    65,    28,
      65,    -1,    65,    39,    65,    -1,    65,    40,    65,    -1,
      65,    37,    65,    -1,    65,    38,    65,    -1,    65,    33,
      65,    -1,    65,    34,    65,    -1,    65,    32,    65,    -1,
      65,    31,    65,    -1,    65,    36,    65,    -1,    65,    35,
      65,    -1,    -1,    65,    -1,    -1,    68,    -1,    65,    -1,
      68,    49,    65,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    65,    65,    66,    69,    70,    72,    74,    76,    78,
      81,    82,    86,    87,    89,    90,    91,    92,    95,    96,
      99,   100,   103,   104,   107,   108,   111,   112,   115,   116,
     119,   121,   123,   128,   130,   132,   135,   137,   140,   144,
     146,   148,   150,   152,   154,   156,   157,   158,   160,   162,
     164,   166,   168,   170,   172,   174,   176,   178,   180,   181,
     183,   185,   186,   187,   188,   190,   191,   192,   193,   194,
     195,   198,   199,   202,   203,   206,   207
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tSYMBOL", "tREGEXP", "tSTRING",
  "tINTEGER", "tREAL", "tSUB", "tSTATE", "tSTART", "tSTARTRULES",
  "tNAMERULES", "tBEGIN", "tEND", "tRETURN", "tIF", "tELSE", "tLOCAL",
  "tWHILE", "tFOR", "tEXTENDS", "'='", "tDIVASSIGN", "tMULASSIGN",
  "tSUBASSIGN", "tADDASSIGN", "'?'", "':'", "tOR", "tAND", "tNE", "tEQ",
  "'<'", "'>'", "tLE", "tGE", "'+'", "'-'", "'*'", "tDIV", "'!'",
  "tMINUSMINUS", "tPLUSPLUS", "'['", "']'", "'{'", "'}'", "';'", "','",
  "'('", "')'", "$accept", "file", "toplevel", "regexp_sym_list",
  "staterules", "staterule", "symbol_list", "rest_symbol_list", "locals",
  "locals_rest", "local_def", "stmt_list", "stmt", "expr", "cond_expr",
  "expr_list", "rest_expr_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,    61,   277,   278,   279,   280,    63,    58,   281,
     282,   283,   284,    60,    62,   285,   286,    43,    45,    42,
     287,    33,   288,   289,    91,    93,   123,   125,    59,    44,
      40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    53,    53,    54,    54,    54,    54,    54,    54,
      55,    55,    56,    56,    57,    57,    57,    57,    58,    58,
      59,    59,    60,    60,    61,    61,    62,    62,    63,    63,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    66,    66,    67,    67,    68,    68
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     4,     4,     4,     5,     7,     1,
       0,     4,     0,     2,     4,     4,     4,     4,     0,     1,
       1,     3,     0,     3,     1,     3,     1,     3,     0,     2,
       2,     3,     9,     3,     5,     7,     5,     9,     2,     1,
       1,     1,     1,     1,     2,     3,     3,     4,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     6,     3,     4,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     0,     1,     0,     1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,    43,    40,    39,    41,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      28,     0,     3,     9,     0,     0,     0,     0,     0,     0,
      54,    53,    73,     0,     0,    28,    10,    10,    30,     0,
       0,     0,    71,    44,    56,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,    48,    52,    51,    50,    49,    75,     0,
      74,    18,     0,    12,     0,     0,     0,    31,     0,     0,
      72,     0,    33,    29,    58,     0,    46,    45,    68,    67,
      65,    66,    70,    69,    63,    64,    61,    62,     0,    47,
       0,    20,     0,    19,     0,     0,     4,     0,     5,     6,
       0,     0,     0,     0,    59,    76,     0,     0,    12,     0,
       0,     0,     0,     7,    13,     0,    34,    36,     0,    60,
       0,    22,    21,     0,    28,    28,    28,    28,    11,     0,
      71,    57,     0,    28,     8,     0,     0,     0,     0,    35,
       0,    26,     0,    24,     0,    17,    16,    14,    15,     0,
       0,    23,     0,    32,    37,    27,    25
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    22,    75,   105,   124,   102,   103,   143,   152,
     153,    46,    83,    24,    81,    69,    70
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -41
static const yytype_int16 yypact[] =
{
     -41,    71,   -41,   300,   -41,   -41,   -41,   -41,     1,     9,
     -40,   -28,   -27,    18,   -24,   -18,   -17,   296,    28,    31,
     -41,   296,   -41,   -41,   391,   296,   296,   296,   296,   296,
     -41,   -41,   296,   -15,   -19,   -41,   -41,   -41,   -41,   411,
     296,   296,   296,    12,   -41,   -41,   126,   322,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   -41,   486,   486,   486,   486,   486,   486,     6,
      20,    64,    67,   -41,   146,     3,     4,   -41,   345,   368,
     486,    10,   -41,   -41,   -41,   470,   501,   515,   527,   527,
      55,    55,    55,    55,    45,    45,    12,    12,   451,   -41,
     296,   -41,    21,    24,    60,    49,   -41,    94,   -41,   -41,
     290,   290,   296,   296,    85,   486,    62,   112,   -41,    70,
      73,    74,    76,   -41,   -41,    75,   107,   -41,   431,   486,
     296,   108,   -41,    51,   -41,   -41,   -41,   -41,   -41,   290,
     296,   486,   122,   -41,   -41,   175,   194,   223,   242,   -41,
      84,   105,   -38,   -41,   271,   -41,   -41,   -41,   -41,   290,
     296,   -41,   122,   -41,   -41,   486,   -41
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -41,   -41,   -41,    99,    19,   -41,   -41,   -41,   -41,   -41,
     -23,   -32,    -1,   -12,     0,   -41,   -41
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      23,    39,    72,    74,    33,    43,    35,   107,   107,    47,
     161,   162,    34,    63,    64,    65,    66,    67,    36,    37,
      68,     3,     4,     5,     6,     7,    40,    73,    78,    79,
      80,    44,    41,    42,    45,    71,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
     108,   109,   119,   120,   119,   120,    61,    99,   112,    17,
      18,    19,   121,   122,   121,   122,    38,   101,    21,   100,
     104,     2,   116,   117,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    59,    60,    13,    14,   115,    61,
      15,    16,    57,    58,    59,    60,   123,   125,   144,    61,
     128,   129,   145,   146,   147,   148,   118,   130,   131,   126,
     127,   154,    17,    18,    19,   132,   134,    20,   141,   135,
     136,    21,   137,   138,   139,   151,   142,   160,    80,     3,
       4,     5,     6,     7,     8,   159,    76,   133,   149,   166,
     150,    13,    14,     0,     0,    15,    16,     0,   165,     3,
       4,     5,     6,     7,     8,     0,     0,     0,   164,     0,
       0,    13,    14,     0,     0,    15,    16,    17,    18,    19,
       0,     0,    20,    82,     0,     0,    21,     0,     3,     4,
       5,     6,     7,     8,     0,     0,     0,    17,    18,    19,
      13,    14,    20,   106,    15,    16,    21,     3,     4,     5,
       6,     7,     8,     0,     0,     0,     0,     0,     0,    13,
      14,     0,     0,    15,    16,     0,    17,    18,    19,     0,
       0,    20,   155,     0,     0,    21,     3,     4,     5,     6,
       7,     8,     0,     0,     0,    17,    18,    19,    13,    14,
      20,   156,    15,    16,    21,     3,     4,     5,     6,     7,
       8,     0,     0,     0,     0,     0,     0,    13,    14,     0,
       0,    15,    16,     0,    17,    18,    19,     0,     0,    20,
     157,     0,     0,    21,     3,     4,     5,     6,     7,     8,
       0,     0,     0,    17,    18,    19,    13,    14,    20,   158,
      15,    16,    21,     3,     4,     5,     6,     7,     8,     3,
       4,     5,     6,     7,     0,    13,    14,     0,     0,    15,
      16,     0,    17,    18,    19,     0,     0,    20,   163,     0,
       0,    21,    25,    26,    27,    28,    29,     0,     0,     0,
       0,    17,    18,    19,     0,     0,    20,    17,    18,    19,
      21,     0,    30,    31,     0,     0,    21,     0,     0,    48,
      32,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,     0,     0,     0,    61,     0,     0,     0,
       0,     0,    48,    84,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,     0,     0,     0,    61,
       0,     0,     0,     0,     0,    48,   110,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,     0,
       0,     0,    61,     0,     0,     0,     0,     0,    48,   111,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,     0,     0,     0,    61,     0,     0,    48,    62,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,     0,     0,     0,    61,     0,     0,    48,    77,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,     0,     0,     0,    61,     0,     0,    48,   140,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,     0,     0,     0,    61,   114,    48,   113,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,     0,     0,    48,    61,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,     0,     0,     0,
      61,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,     0,     0,     0,    61,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,     0,     0,     0,    61,
      53,    54,    55,    56,    57,    58,    59,    60,     0,     0,
       0,    61
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-41))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       1,    13,    21,    35,     3,    17,    46,     4,     4,    21,
      48,    49,     3,    25,    26,    27,    28,    29,    46,    46,
      32,     3,     4,     5,     6,     7,    50,    46,    40,    41,
      42,     3,    50,    50,     3,    50,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      47,    47,     3,     4,     3,     4,    44,    51,    48,    41,
      42,    43,    13,    14,    13,    14,    48,     3,    50,    49,
       3,     0,    51,    49,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    39,    40,    15,    16,   100,    44,
      19,    20,    37,    38,    39,    40,    47,     3,    47,    44,
     112,   113,   134,   135,   136,   137,    46,    22,    46,   110,
     111,   143,    41,    42,    43,     3,    46,    46,   130,    46,
      46,    50,    46,    48,    17,     3,    18,    22,   140,     3,
       4,     5,     6,     7,     8,    51,    37,   118,   139,   162,
     140,    15,    16,    -1,    -1,    19,    20,    -1,   160,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,   159,    -1,
      -1,    15,    16,    -1,    -1,    19,    20,    41,    42,    43,
      -1,    -1,    46,    47,    -1,    -1,    50,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    41,    42,    43,
      15,    16,    46,    47,    19,    20,    50,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      16,    -1,    -1,    19,    20,    -1,    41,    42,    43,    -1,
      -1,    46,    47,    -1,    -1,    50,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    41,    42,    43,    15,    16,
      46,    47,    19,    20,    50,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    -1,
      -1,    19,    20,    -1,    41,    42,    43,    -1,    -1,    46,
      47,    -1,    -1,    50,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    41,    42,    43,    15,    16,    46,    47,
      19,    20,    50,     3,     4,     5,     6,     7,     8,     3,
       4,     5,     6,     7,    -1,    15,    16,    -1,    -1,    19,
      20,    -1,    41,    42,    43,    -1,    -1,    46,    47,    -1,
      -1,    50,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      -1,    41,    42,    43,    -1,    -1,    46,    41,    42,    43,
      50,    -1,    42,    43,    -1,    -1,    50,    -1,    -1,    27,
      50,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    27,    51,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    27,    51,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    27,    51,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    -1,    -1,    27,    48,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    -1,    -1,    27,    48,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    -1,    -1,    27,    48,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    45,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    -1,    27,    44,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      44,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    -1,    44,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    -1,
      -1,    44
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    53,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    15,    16,    19,    20,    41,    42,    43,
      46,    50,    54,    64,    65,    22,    23,    24,    25,    26,
      42,    43,    50,     3,     3,    46,    46,    46,    48,    65,
      50,    50,    50,    65,     3,     3,    63,    65,    27,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    44,    48,    65,    65,    65,    65,    65,    65,    67,
      68,    50,    21,    46,    63,    55,    55,    48,    65,    65,
      65,    66,    47,    64,    51,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    51,
      49,     3,    58,    59,     3,    56,    47,     4,    47,    47,
      51,    51,    48,    28,    45,    65,    51,    49,    46,     3,
       4,    13,    14,    47,    57,     3,    64,    64,    65,    65,
      22,    46,     3,    56,    46,    46,    46,    46,    48,    17,
      48,    65,    18,    60,    47,    63,    63,    63,    63,    64,
      66,     3,    61,    62,    63,    47,    47,    47,    47,    51,
      22,    48,    49,    47,    64,    65,    62
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
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


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
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
		  Type, Value); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
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
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

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

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

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

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
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

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

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
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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
  if (yypact_value_is_default (yyn))
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
      if (yytable_value_is_error (yyn))
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:

/* Line 1806 of yacc.c  */
#line 69 "gram.y"
    { start_stmts = (yyvsp[(3) - (4)].lst); }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 71 "gram.y"
    { startrules = (yyvsp[(3) - (4)].lst); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 73 "gram.y"
    { namerules = (yyvsp[(3) - (4)].lst); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 75 "gram.y"
    { define_state ((yyvsp[(2) - (5)].node), NULL, (yyvsp[(4) - (5)].lst)); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 77 "gram.y"
    { define_state ((yyvsp[(2) - (7)].node), (yyvsp[(4) - (7)].node), (yyvsp[(6) - (7)].lst)); }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 78 "gram.y"
    { list_append (global_stmts, (yyvsp[(1) - (1)].stmt)); }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 81 "gram.y"
    { (yyval.lst) = list (); }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 83 "gram.y"
    { list_append ((yyvsp[(1) - (4)].lst), cons ((yyvsp[(2) - (4)].node), (yyvsp[(3) - (4)].node))); }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 86 "gram.y"
    { (yyval.lst) = list (); }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 87 "gram.y"
    { list_append ((yyvsp[(1) - (2)].lst), (yyvsp[(2) - (2)].cons)); }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 89 "gram.y"
    { (yyval.cons) = cons (RULE_BEGIN, (yyvsp[(3) - (4)].lst)); }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 90 "gram.y"
    { (yyval.cons) = cons (RULE_END, (yyvsp[(3) - (4)].lst)); }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 91 "gram.y"
    { (yyval.cons) = cons ((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].lst)); }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 92 "gram.y"
    { (yyval.cons) = cons ((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].lst)); }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 95 "gram.y"
    { (yyval.lst) = list (); }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 96 "gram.y"
    { (yyval.lst) = (yyvsp[(1) - (1)].lst); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 99 "gram.y"
    { (yyval.lst) = list (); list_append ((yyval.lst), (yyvsp[(1) - (1)].node)); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 100 "gram.y"
    { list_append ((yyvsp[(1) - (3)].lst), (yyvsp[(3) - (3)].node)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 103 "gram.y"
    { (yyval.lst) = list (); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 104 "gram.y"
    { (yyval.lst) = (yyvsp[(2) - (3)].lst); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 107 "gram.y"
    { (yyval.lst) = list (); list_append ((yyval.lst), (yyvsp[(1) - (1)].cons)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 108 "gram.y"
    { list_append ((yyvsp[(1) - (3)].lst), (yyvsp[(3) - (3)].cons)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 111 "gram.y"
    { (yyval.cons) = cons ((yyvsp[(1) - (1)].node), NULL); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 112 "gram.y"
    { (yyval.cons) = cons ((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].expr)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 115 "gram.y"
    { (yyval.lst) = list (); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 116 "gram.y"
    { list_append ((yyvsp[(1) - (2)].lst), (yyvsp[(2) - (2)].stmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 119 "gram.y"
    { (yyval.stmt) = mk_stmt (sRETURN, NULL, NULL,
							NULL, NULL); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 121 "gram.y"
    { (yyval.stmt) = mk_stmt (sRETURN, (yyvsp[(2) - (3)].expr), NULL,
							NULL, NULL); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 124 "gram.y"
    { (yyval.stmt) = mk_stmt (sDEFSUB, (yyvsp[(2) - (9)].node),
							cons (cons ((yyvsp[(4) - (9)].lst), (yyvsp[(7) - (9)].lst)),
							      (yyvsp[(8) - (9)].lst)),
							NULL, NULL); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 128 "gram.y"
    { (yyval.stmt) = mk_stmt (sBLOCK, (yyvsp[(2) - (3)].lst), NULL,
							NULL, NULL); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 130 "gram.y"
    { (yyval.stmt) = mk_stmt (sIF, (yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt), NULL,
							NULL); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 133 "gram.y"
    { (yyval.stmt) = mk_stmt (sIF, (yyvsp[(3) - (7)].expr), (yyvsp[(5) - (7)].stmt), (yyvsp[(7) - (7)].stmt),
							NULL); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 135 "gram.y"
    { (yyval.stmt) = mk_stmt (sWHILE, (yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt),
							NULL, NULL); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 138 "gram.y"
    { (yyval.stmt) = mk_stmt (sFOR, (yyvsp[(3) - (9)].expr), (yyvsp[(5) - (9)].expr), (yyvsp[(7) - (9)].expr),
							(yyvsp[(9) - (9)].stmt)); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 140 "gram.y"
    { (yyval.stmt) = mk_stmt (sEXPR, (yyvsp[(1) - (2)].expr), NULL,
							NULL, NULL); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 144 "gram.y"
    { (yyval.expr) = mk_expr (eSTRING, (yyvsp[(1) - (1)].node), NULL,
							NULL); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 146 "gram.y"
    { (yyval.expr) = mk_expr (eREGEXP, (yyvsp[(1) - (1)].node), NULL,
							NULL); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 148 "gram.y"
    { (yyval.expr) = mk_expr (eINTEGER, (yyvsp[(1) - (1)].node), NULL,
							NULL); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 150 "gram.y"
    { (yyval.expr) = mk_expr (eREAL, (yyvsp[(1) - (1)].node), NULL,
							NULL); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 152 "gram.y"
    { (yyval.expr) = mk_expr (eSYMBOL, (yyvsp[(1) - (1)].node), NULL,
							NULL); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 154 "gram.y"
    { (yyval.expr) = mk_expr (eNOT, (yyvsp[(2) - (2)].expr), NULL,
							NULL); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 156 "gram.y"
    { (yyval.expr) = mk_expr (eAND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr), NULL); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 157 "gram.y"
    { (yyval.expr) = mk_expr (eOR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr), NULL); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 158 "gram.y"
    { (yyval.expr) = mk_expr (eFCALL, (yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].lst),
							NULL); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 160 "gram.y"
    { (yyval.expr) = mk_expr (eASSIGN, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].expr),
							NULL); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 162 "gram.y"
    { (yyval.expr) = mk_expr (eADDASSIGN, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].expr),
							NULL); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 164 "gram.y"
    { (yyval.expr) = mk_expr (eSUBASSIGN, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].expr),
							NULL); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 166 "gram.y"
    { (yyval.expr) = mk_expr (eMULASSIGN, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].expr),
							NULL); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 168 "gram.y"
    { (yyval.expr) = mk_expr (eDIVASSIGN, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].expr),
							NULL); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 170 "gram.y"
    { (yyval.expr) = mk_expr (ePOSTFIXADD, (yyvsp[(1) - (2)].node), NULL,
							NULL); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 172 "gram.y"
    { (yyval.expr) = mk_expr (ePOSTFIXSUB, (yyvsp[(1) - (2)].node), NULL,
							NULL); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 174 "gram.y"
    { (yyval.expr) = mk_expr (ePREFIXADD, (yyvsp[(2) - (2)].node), NULL,
							NULL); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 176 "gram.y"
    { (yyval.expr) = mk_expr (ePREFIXSUB, (yyvsp[(2) - (2)].node), NULL,
							NULL); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 178 "gram.y"
    { (yyval.expr) = mk_expr (eARRAYASSIGN, (yyvsp[(1) - (6)].expr), (yyvsp[(3) - (6)].expr),
							(yyvsp[(6) - (6)].expr)); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 180 "gram.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 181 "gram.y"
    { (yyval.expr) = mk_expr (eARRAYREF, (yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].expr),
							NULL); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 183 "gram.y"
    { (yyval.expr) = mk_expr (eQUESTCOLON, (yyvsp[(1) - (5)].expr), (yyvsp[(3) - (5)].expr),
							(yyvsp[(5) - (5)].expr)); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 185 "gram.y"
    { (yyval.expr) = mk_expr (eMULT, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr), NULL); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 186 "gram.y"
    { (yyval.expr) = mk_expr (eDIV, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr), NULL); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 187 "gram.y"
    { (yyval.expr) = mk_expr (ePLUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr), NULL); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 188 "gram.y"
    { (yyval.expr) = mk_expr (eMINUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr),
							NULL); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 190 "gram.y"
    { (yyval.expr) = mk_expr (eLT, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr), NULL); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 191 "gram.y"
    { (yyval.expr) = mk_expr (eGT, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr), NULL); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 192 "gram.y"
    { (yyval.expr) = mk_expr (eEQ, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr), NULL); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 193 "gram.y"
    { (yyval.expr) = mk_expr (eNE, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr), NULL); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 194 "gram.y"
    { (yyval.expr) = mk_expr (eGE, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr), NULL); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 195 "gram.y"
    { (yyval.expr) = mk_expr (eLE, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr), NULL); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 198 "gram.y"
    { (yyval.expr) = NULL; }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 199 "gram.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 202 "gram.y"
    { (yyval.lst) = list (); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 203 "gram.y"
    { (yyval.lst) = (yyvsp[(1) - (1)].lst); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 206 "gram.y"
    { (yyval.lst) = list (); list_append ((yyval.lst), (yyvsp[(1) - (1)].expr)); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 207 "gram.y"
    { list_append ((yyvsp[(1) - (3)].lst), (yyvsp[(3) - (3)].expr)); }
    break;



/* Line 1806 of yacc.c  */
#line 2218 "gram.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

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
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



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
		      yytoken, &yylval);
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
      if (!yypact_value_is_default (yyn))
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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



/* Line 2067 of yacc.c  */
#line 210 "gram.y"


void
yyerror (msg)
     char *msg;
{
  fprintf (stderr, "%s:%d: %s\n", yyin_name, linenum, msg);
}

