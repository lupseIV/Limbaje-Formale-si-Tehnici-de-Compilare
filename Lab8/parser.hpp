/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INCLUDE = 258,                 /* "#include"  */
    IOSTREAM = 259,                /* "iostream"  */
    USING = 260,                   /* "using"  */
    NAMESPACE = 261,               /* "namespace"  */
    STD = 262,                     /* "std"  */
    STRUCT = 263,                  /* "struct"  */
    INT = 264,                     /* "int"  */
    DOUBLE = 265,                  /* "double"  */
    MAIN = 266,                    /* "main"  */
    RETURN = 267,                  /* "return"  */
    CIN = 268,                     /* "cin"  */
    COUT = 269,                    /* "cout"  */
    ENDL = 270,                    /* "endl"  */
    IF = 271,                      /* "if"  */
    ELSE = 272,                    /* "else"  */
    WHILE = 273,                   /* "while"  */
    SHL = 274,                     /* "<<"  */
    SHR = 275,                     /* ">>"  */
    LE = 276,                      /* "<="  */
    GE = 277,                      /* ">="  */
    EQ = 278,                      /* "=="  */
    NE = 279,                      /* "!="  */
    ASSIGN = 280,                  /* "="  */
    PLUS = 281,                    /* "+"  */
    MINUS = 282,                   /* "-"  */
    MUL = 283,                     /* "*"  */
    DIV = 284,                     /* "/"  */
    LT = 285,                      /* "<"  */
    GT = 286,                      /* ">"  */
    SEMI = 287,                    /* ";"  */
    COMMA = 288,                   /* ","  */
    DOT = 289,                     /* "."  */
    LBRACE = 290,                  /* "{"  */
    RBRACE = 291,                  /* "}"  */
    LPAREN = 292,                  /* "("  */
    RPAREN = 293,                  /* ")"  */
    ID = 294,                      /* ID  */
    CONST = 295,                   /* CONST  */
    LOWER_THAN_ELSE = 296          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
