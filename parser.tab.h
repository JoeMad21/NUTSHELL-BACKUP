/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 210 "parser.y"

#include <string>

#line 52 "parser.tab.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BACK = 258,
    PIPE = 259,
    APP_STDERR = 260,
    APP_STDOUT = 261,
    OW_STDERR = 262,
    OW_STDOUT = 263,
    APP_STDIN = 264,
    OW_STDIN = 265,
    FLAG = 266,
    SETENV = 267,
    PRINTENV = 268,
    BYE = 269,
    WORD = 270,
    NEW_LINE = 271,
    TILDE = 272,
    META = 273,
    CD = 274,
    ALIAS = 275,
    UNSETENV = 276,
    UNALIAS = 277,
    NULL_TOKEN = 278,
    HOME = 279,
    PATH = 280,
    NOT_A_TOKEN = 281
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{

  /* setenv  */
  char* setenv;
  /* BACK  */
  std::string* BACK;
  /* PIPE  */
  std::string* PIPE;
  /* APP_STDERR  */
  std::string* APP_STDERR;
  /* APP_STDOUT  */
  std::string* APP_STDOUT;
  /* OW_STDERR  */
  std::string* OW_STDERR;
  /* OW_STDOUT  */
  std::string* OW_STDOUT;
  /* APP_STDIN  */
  std::string* APP_STDIN;
  /* OW_STDIN  */
  std::string* OW_STDIN;
  /* FLAG  */
  std::string* FLAG;
  /* SETENV  */
  std::string* SETENV;
  /* PRINTENV  */
  std::string* PRINTENV;
  /* BYE  */
  std::string* BYE;
  /* WORD  */
  std::string* WORD;
  /* NEW_LINE  */
  std::string* NEW_LINE;
  /* TILDE  */
  std::string* TILDE;
  /* META  */
  std::string* META;
  /* CD  */
  std::string* CD;
  /* ALIAS  */
  std::string* ALIAS;
  /* UNSETENV  */
  std::string* UNSETENV;
  /* UNALIAS  */
  std::string* UNALIAS;
  /* NULL_TOKEN  */
  std::string* NULL_TOKEN;
  /* HOME  */
  std::string* HOME;
  /* PATH  */
  std::string* PATH;
  /* NOT_A_TOKEN  */
  std::string* NOT_A_TOKEN;
#line 141 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
