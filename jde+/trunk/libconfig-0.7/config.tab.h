/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     BOOLEAN = 258,
     INTEGER = 259,
     FLOAT = 260,
     STRING = 261,
     NAME = 262,
     EQUALS = 263,
     NEWLINE = 264,
     ARRAY_START = 265,
     ARRAY_END = 266,
     COMMA = 267,
     GROUP_START = 268,
     GROUP_END = 269,
     END = 270,
     GARBAGE = 271
   };
#endif
#define BOOLEAN 258
#define INTEGER 259
#define FLOAT 260
#define STRING 261
#define NAME 262
#define EQUALS 263
#define NEWLINE 264
#define ARRAY_START 265
#define ARRAY_END 266
#define COMMA 267
#define GROUP_START 268
#define GROUP_END 269
#define END 270
#define GARBAGE 271




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 55 "grammar.y"
typedef union YYSTYPE {
  long ival;
  double fval;
  char *sval;
  } YYSTYPE;
/* Line 1240 of yacc.c.  */
#line 74 "config.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif





