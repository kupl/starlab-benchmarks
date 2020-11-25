/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
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

/* Line 2068 of yacc.c  */
#line 34 "gram.y"

  List *lst;
  Node *node;
  Cons *cons;
  Stmt *stmt;
  Expr *expr;



/* Line 2068 of yacc.c  */
#line 128 "gram.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


