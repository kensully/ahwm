
/*  A Bison parser, made from parser.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	TOK_DISPLAYTITLEBAR	257
#define	TOK_DEFAULTWORKSPACE	258
#define	TOK_TRUE	259
#define	TOK_FALSE	260
#define	TOK_TRANSIENTFOR	261
#define	TOK_HASTRANSIENT	262
#define	TOK_NOT	263
#define	TOK_ISSHAPED	264
#define	TOK_INWORKSPACE	265
#define	TOK_WINDOWCLASS	266
#define	TOK_WINDOWNAME	267
#define	TOK_WINDOWINSTANCE	268
#define	TOK_FUNCTION	269
#define	TOK_MENU	270
#define	TOK_BINDKEY	271
#define	TOK_BINDBUTTON	272
#define	TOK_UNBINDKEY	273
#define	TOK_UNBINDBUTTON	274
#define	TOK_ROOT	275
#define	TOK_FRAME	276
#define	TOK_TITLEBAR	277
#define	TOK_MOVETOWORKSPACE	278
#define	TOK_GOTOWORKSPACE	279
#define	TOK_ALTTAB	280
#define	TOK_KILLNICELY	281
#define	TOK_KILLWITHEXTREMEPREJUDICE	282
#define	TOK_LAUNCH	283
#define	TOK_FOCUS	284
#define	TOK_MAXIMIZE	285
#define	TOK_NOP	286
#define	TOK_QUOTE	287
#define	TOK_MOVEINTERACTIVELY	288
#define	TOK_RESIZEINTERACTIVELY	289
#define	TOK_MOVERESIZE	290
#define	TOK_QUIT	291
#define	TOK_BEEP	292
#define	TOK_INVOKE	293
#define	TOK_SHOWMENU	294
#define	TOK_REFRESH	295
#define	TOK_SEMI	296
#define	TOK_EQUALS	297
#define	TOK_LBRACE	298
#define	TOK_RBRACE	299
#define	TOK_COMMA	300
#define	TOK_LPAREN	301
#define	TOK_RPAREN	302
#define	TOK_STRING	303
#define	TOK_INTEGER	304
#define	TOK_FLOAT	305

#line 1 "parser.y"

/* $Id$
 * This was written by Alex Hioreanu in 2001.
 * This code is in the public domain and the author disclaims all
 * copyright privileges.
 */
/*
 * Parser definition file for use with bison (not tested with yacc).
 * Should have no s/r or r/r conflicts and doesn't use right recursion
 * anywhere.  I opted for clarity in lieu of brevity because this
 * stuff is complex enough without overloading rules, structures or
 * names.  A simple little parser, no "gotchas".
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#define parse_debug(x) printf(x)

#line 80 "parser.y"

#include "prefs.h"
line *make_line(int type, void *dollar_one);

#line 85 "parser.y"
typedef union {
    int value_int;
    float value_float;
    char *value_string;
    line *value_line;
    option *value_option;
    context *value_context;
    type *value_type;
    keybinding *value_keybinding;
    keyunbinding *value_keyunbinding;
    mousebinding *value_mousebinding;
    mouseunbinding *value_mouseunbinding;
    function *value_function;
    arglist *value_arglist;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		84
#define	YYFLAG		-32768
#define	YYNTBASE	52

#define YYTRANSLATE(x) ((unsigned)(x) <= 305 ? yytranslate[x] : 70)

static const char yytranslate[] = {     0,
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
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     3,     6,     9,    12,    15,    18,    21,    24,
    27,    31,    33,    35,    37,    39,    41,    43,    45,    52,
    54,    56,    58,    60,    62,    63,    65,    67,    69,    73,
    78,    81,    85,    87,    89,    91,    96,   100,   102,   104,
   106,   108,   110,   112,   114,   116,   118,   120,   122,   124,
   126,   128,   130,   132,   134,   136,   140
};

static const short yyrhs[] = {    53,
     0,     0,    53,    54,     0,    55,    42,     0,    59,    42,
     0,    62,    42,     0,    64,    42,     0,    63,    42,     0,
    65,    42,     0,     1,    42,     0,    56,    43,    57,     0,
     3,     0,     4,     0,    58,     0,    49,     0,    50,     0,
     5,     0,     6,     0,    61,    60,    57,    44,    53,    45,
     0,    10,     0,    11,     0,    13,     0,    12,     0,    14,
     0,     0,     9,     0,     7,     0,     8,     0,    17,    49,
    67,     0,    18,    66,    49,    67,     0,    19,    49,     0,
    20,    66,    49,     0,    21,     0,    22,     0,    23,     0,
    68,    47,    69,    48,     0,    68,    47,    48,     0,    24,
     0,    25,     0,    26,     0,    27,     0,    28,     0,    29,
     0,    30,     0,    31,     0,    32,     0,    33,     0,    34,
     0,    35,     0,    36,     0,    37,     0,    38,     0,    39,
     0,    40,     0,    41,     0,    69,    46,    57,     0,    57,
     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   120,   122,   123,   137,   138,   139,   140,   141,   142,   143,
   150,   162,   163,   165,   175,   185,   196,   197,   199,   212,
   213,   214,   215,   216,   218,   219,   220,   221,   223,   233,
   244,   253,   264,   265,   266,   268,   277,   287,   288,   289,
   290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
   300,   301,   302,   303,   304,   306,   323
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","TOK_DISPLAYTITLEBAR",
"TOK_DEFAULTWORKSPACE","TOK_TRUE","TOK_FALSE","TOK_TRANSIENTFOR","TOK_HASTRANSIENT",
"TOK_NOT","TOK_ISSHAPED","TOK_INWORKSPACE","TOK_WINDOWCLASS","TOK_WINDOWNAME",
"TOK_WINDOWINSTANCE","TOK_FUNCTION","TOK_MENU","TOK_BINDKEY","TOK_BINDBUTTON",
"TOK_UNBINDKEY","TOK_UNBINDBUTTON","TOK_ROOT","TOK_FRAME","TOK_TITLEBAR","TOK_MOVETOWORKSPACE",
"TOK_GOTOWORKSPACE","TOK_ALTTAB","TOK_KILLNICELY","TOK_KILLWITHEXTREMEPREJUDICE",
"TOK_LAUNCH","TOK_FOCUS","TOK_MAXIMIZE","TOK_NOP","TOK_QUOTE","TOK_MOVEINTERACTIVELY",
"TOK_RESIZEINTERACTIVELY","TOK_MOVERESIZE","TOK_QUIT","TOK_BEEP","TOK_INVOKE",
"TOK_SHOWMENU","TOK_REFRESH","TOK_SEMI","TOK_EQUALS","TOK_LBRACE","TOK_RBRACE",
"TOK_COMMA","TOK_LPAREN","TOK_RPAREN","TOK_STRING","TOK_INTEGER","TOK_FLOAT",
"config_file","config","line","option","option_name","type","boolean","context",
"context_name","context_option","keybinding","mousebinding","keyunbinding","mouseunbinding",
"location","function","function_name","arglist", NULL
};
#endif

static const short yyr1[] = {     0,
    52,    53,    53,    54,    54,    54,    54,    54,    54,    54,
    55,    56,    56,    57,    57,    57,    58,    58,    59,    60,
    60,    60,    60,    60,    61,    61,    61,    61,    62,    63,
    64,    65,    66,    66,    66,    67,    67,    68,    68,    68,
    68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
    68,    68,    68,    68,    68,    69,    69
};

static const short yyr2[] = {     0,
     1,     0,     2,     2,     2,     2,     2,     2,     2,     2,
     3,     1,     1,     1,     1,     1,     1,     1,     6,     1,
     1,     1,     1,     1,     0,     1,     1,     1,     3,     4,
     2,     3,     1,     1,     1,     4,     3,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     3,     1
};

static const short yydefact[] = {     2,
     0,     0,    12,    13,    27,    28,    26,     0,     0,     0,
     0,     3,     0,     0,     0,     0,     0,     0,     0,     0,
    10,     0,    33,    34,    35,     0,    31,     0,     4,     0,
     5,    20,    21,    23,    22,    24,     0,     6,     8,     7,
     9,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    29,
     0,     0,    32,    17,    18,    15,    16,    11,    14,     0,
     0,    30,     2,    37,    57,     0,     0,     0,    36,    19,
    56,     0,     0,     0
};

static const short yydefgoto[] = {    82,
     1,    12,    13,    14,    68,    69,    15,    37,    16,    17,
    18,    19,    20,    26,    60,    61,    76
};

static const short yypact[] = {-32768,
    50,   -37,-32768,-32768,-32768,-32768,-32768,   -40,    55,   -31,
    55,-32768,   -23,     9,    13,    61,    14,    23,    24,    38,
-32768,     0,-32768,-32768,-32768,    34,-32768,    35,-32768,    -3,
-32768,-32768,-32768,-32768,-32768,-32768,    -3,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    39,     0,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    41,
    -5,-32768,-32768,-32768,-32768,    33,     3,    -3,-32768,-32768,
-32768,    82,    87,-32768
};

static const short yypgoto[] = {-32768,
    15,-32768,-32768,-32768,   -29,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    78,    28,-32768,-32768
};


#define	YYLAST		90


static const short yytable[] = {    64,
    65,    64,    65,     2,    21,     3,     4,    70,    22,     5,
     6,     7,   -25,   -25,   -25,   -25,   -25,    27,    29,     8,
     9,    10,    11,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    75,    74,    66,    67,    66,    67,    80,    81,    -1,
     2,    30,     3,     4,    31,    38,     5,     6,     7,   -25,
   -25,   -25,   -25,   -25,    39,    40,     8,     9,    10,    11,
    32,    33,    34,    35,    36,    23,    24,    25,    78,    41,
    79,    83,    62,    63,    73,    71,    84,    77,    28,    72
};

static const short yycheck[] = {     5,
     6,     5,     6,     1,    42,     3,     4,    37,    49,     7,
     8,     9,    10,    11,    12,    13,    14,    49,    42,    17,
    18,    19,    20,    24,    25,    26,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
    41,    71,    48,    49,    50,    49,    50,    45,    78,     0,
     1,    43,     3,     4,    42,    42,     7,     8,     9,    10,
    11,    12,    13,    14,    42,    42,    17,    18,    19,    20,
    10,    11,    12,    13,    14,    21,    22,    23,    46,    42,
    48,     0,    49,    49,    44,    47,     0,    73,    11,    62
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

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

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 120 "parser.y"
{ preferences = yyvsp[0].value_line; ;
    break;}
case 2:
#line 122 "parser.y"
{ yyval.value_line = NULL; ;
    break;}
case 3:
#line 124 "parser.y"
{
           line *tmp;
           parse_debug("CONFIG\n");
           if (yyvsp[-1].value_line != NULL) {
               tmp = yyvsp[-1].value_line;
               while (tmp->line_next != NULL) tmp = tmp->line_next;
               tmp->line_next = yyvsp[0].value_line;
               yyval.value_line = yyvsp[-1].value_line;
           } else {
               yyval.value_line = yyvsp[0].value_line;
           }
       ;
    break;}
case 4:
#line 137 "parser.y"
{ yyval.value_line = make_line(OPTION, yyvsp[-1].value_option); ;
    break;}
case 5:
#line 138 "parser.y"
{ yyval.value_line = make_line(CONTEXT, yyvsp[-1].value_context); ;
    break;}
case 6:
#line 139 "parser.y"
{ yyval.value_line = make_line(KEYBINDING, yyvsp[-1].value_keybinding); ;
    break;}
case 7:
#line 140 "parser.y"
{ yyval.value_line = make_line(KEYUNBINDING, yyvsp[-1].value_keyunbinding); ;
    break;}
case 8:
#line 141 "parser.y"
{ yyval.value_line = make_line(MOUSEBINDING, yyvsp[-1].value_mousebinding); ;
    break;}
case 9:
#line 142 "parser.y"
{ yyval.value_line = make_line(MOUSEUNBINDING, yyvsp[-1].value_mouseunbinding); ;
    break;}
case 10:
#line 144 "parser.y"
{
          extern int line_number;
          fprintf(stderr, "XWM: parse error on line %d.  Ignoring statement.\n",
                  line_number);
          yyval.value_line = make_line(INVALID_LINE, NULL);
      ;
    break;}
case 11:
#line 151 "parser.y"
{
            option *opt;
            parse_debug("OPTION\n");
            opt = malloc(sizeof(option));
            if (opt != NULL) {
                opt->option_name = yyvsp[-2].value_int;
                opt->option_value = yyvsp[0].value_type;
            }
            yyval.value_option = opt;
        ;
    break;}
case 12:
#line 162 "parser.y"
{ yyval.value_int = DISPLAYTITLEBAR; ;
    break;}
case 13:
#line 163 "parser.y"
{ yyval.value_int = DEFAULTWORKSPACE; ;
    break;}
case 14:
#line 166 "parser.y"
{
          type *typ;
          typ = malloc(sizeof(type));
          if (typ != NULL) {
              typ->type_type = BOOLEAN;
              typ->type_value.intval = yyvsp[0].value_int;
          }
          yyval.value_type = typ;
      ;
    break;}
case 15:
#line 176 "parser.y"
{
          type *typ;
          typ = malloc(sizeof(type));
          if (typ != NULL) {
              typ->type_type = STRING;
              typ->type_value.stringval = yyvsp[0].value_string;
          }
          yyval.value_type = typ;
      ;
    break;}
case 16:
#line 186 "parser.y"
{
          type *typ;
          typ = malloc(sizeof(type));
          if (typ != NULL) {
              typ->type_type = INTEGER;
              typ->type_value.intval = yyvsp[0].value_int;
          }
          yyval.value_type = typ;
      ;
    break;}
case 17:
#line 196 "parser.y"
{ yyval.value_int = 1; ;
    break;}
case 18:
#line 197 "parser.y"
{ yyval.value_int = 0; ;
    break;}
case 19:
#line 200 "parser.y"
{
             context *cntxt;
             parse_debug("CONTEXT\n");
             cntxt = malloc(sizeof(context));
             if (cntxt != NULL) {
                 cntxt->context_selector = yyvsp[-5].value_int | yyvsp[-4].value_int;
                 cntxt->context_value = yyvsp[-3].value_type;
                 cntxt->context_lines = yyvsp[-1].value_line;
             }
             yyval.value_context = cntxt;
         ;
    break;}
case 20:
#line 212 "parser.y"
{ yyval.value_int = SEL_ISSHAPED; ;
    break;}
case 21:
#line 213 "parser.y"
{ yyval.value_int = SEL_INWORKSPACE; ;
    break;}
case 22:
#line 214 "parser.y"
{ yyval.value_int = SEL_WINDOWNAME; ;
    break;}
case 23:
#line 215 "parser.y"
{ yyval.value_int = SEL_WINDOWCLASS; ;
    break;}
case 24:
#line 216 "parser.y"
{ yyval.value_int = SEL_WINDOWINSTANCE; ;
    break;}
case 25:
#line 218 "parser.y"
{ yyval.value_int = 0; ;
    break;}
case 26:
#line 219 "parser.y"
{ yyval.value_int = SEL_NOT; ;
    break;}
case 27:
#line 220 "parser.y"
{ yyval.value_int = SEL_TRANSIENTFOR; ;
    break;}
case 28:
#line 221 "parser.y"
{ yyval.value_int = SEL_HASTRANSIENT; ;
    break;}
case 29:
#line 224 "parser.y"
{
                keybinding *kb;
                kb = malloc(sizeof(keybinding));
                if (kb != NULL) {
                    kb->keybinding_string = yyvsp[-1].value_string;
                    kb->keybinding_function = yyvsp[0].value_function;
                }
                yyval.value_keybinding = kb;
            ;
    break;}
case 30:
#line 234 "parser.y"
{
                  mousebinding *mb;
                  mb = malloc(sizeof(mousebinding));
                  if (mb != NULL) {
                      mb->mousebinding_string = yyvsp[-1].value_string;
                      mb->mousebinding_location = yyvsp[-2].value_int;
                      mb->mousebinding_function = yyvsp[0].value_function;
                  }
                  yyval.value_mousebinding = mb;
              ;
    break;}
case 31:
#line 245 "parser.y"
{
                  keyunbinding *kub;
                  kub = malloc(sizeof(keyunbinding));
                  if (kub != NULL) {
                      kub->keyunbinding_string = yyvsp[0].value_string;
                  }
                  yyval.value_keyunbinding = kub;
              ;
    break;}
case 32:
#line 254 "parser.y"
{
                    mouseunbinding *mub;
                    mub = malloc(sizeof(mouseunbinding));
                    if (mub != NULL) {
                        mub->mouseunbinding_string = yyvsp[0].value_string;
                        mub->mouseunbinding_location = yyvsp[-1].value_int;
                    }
                    yyval.value_mouseunbinding = mub;
                ;
    break;}
case 33:
#line 264 "parser.y"
{ yyval.value_int = ROOT; ;
    break;}
case 34:
#line 265 "parser.y"
{ yyval.value_int = FRAME; ;
    break;}
case 35:
#line 266 "parser.y"
{ yyval.value_int = TITLEBAR; ;
    break;}
case 36:
#line 269 "parser.y"
{
              function *f = malloc(sizeof(function));
              if (f != NULL) {
                  f->function_type = yyvsp[-3].value_int;
                  f->function_args = yyvsp[-1].value_arglist;
              }
              yyval.value_function = f;
          ;
    break;}
case 37:
#line 278 "parser.y"
{
              function *f = malloc(sizeof(function));
              if (f != NULL) {
                  f->function_type = yyvsp[-2].value_int;
                  f->function_args = NULL;
              }
              yyval.value_function = f;
          ;
    break;}
case 38:
#line 287 "parser.y"
{ yyval.value_int = MOVETOWORKSPACE; ;
    break;}
case 39:
#line 288 "parser.y"
{ yyval.value_int = GOTOWORKSPACE; ;
    break;}
case 40:
#line 289 "parser.y"
{ yyval.value_int = ALTTAB; ;
    break;}
case 41:
#line 290 "parser.y"
{ yyval.value_int = KILLNICELY; ;
    break;}
case 42:
#line 291 "parser.y"
{ yyval.value_int = KILLWITHEXTREMEPREJUDICE; ;
    break;}
case 43:
#line 292 "parser.y"
{ yyval.value_int = LAUNCH; ;
    break;}
case 44:
#line 293 "parser.y"
{ yyval.value_int = FOCUS; ;
    break;}
case 45:
#line 294 "parser.y"
{ yyval.value_int = MAXIMIZE; ;
    break;}
case 46:
#line 295 "parser.y"
{ yyval.value_int = NOP; ;
    break;}
case 47:
#line 296 "parser.y"
{ yyval.value_int = QUOTE; ;
    break;}
case 48:
#line 297 "parser.y"
{ yyval.value_int = MOVEINTERACTIVELY; ;
    break;}
case 49:
#line 298 "parser.y"
{ yyval.value_int = RESIZEINTERACTIVELY; ;
    break;}
case 50:
#line 299 "parser.y"
{ yyval.value_int = MOVERESIZE; ;
    break;}
case 51:
#line 300 "parser.y"
{ yyval.value_int = QUIT; ;
    break;}
case 52:
#line 301 "parser.y"
{ yyval.value_int = BEEP; ;
    break;}
case 53:
#line 302 "parser.y"
{ yyval.value_int = INVOKE; ;
    break;}
case 54:
#line 303 "parser.y"
{ yyval.value_int = SHOWMENU; ;
    break;}
case 55:
#line 304 "parser.y"
{ yyval.value_int = REFRESH; ;
    break;}
case 56:
#line 307 "parser.y"
{
             arglist *tmp;
             arglist *al;
             if (yyvsp[-2].value_arglist != NULL) {
                 al = malloc(sizeof(arglist));
                 if (al != NULL) {
                     al->arglist_arg = yyvsp[0].value_type;
                     al->arglist_next = NULL;
                     tmp = yyvsp[-2].value_arglist;
                     while (tmp->arglist_next != NULL)
                         tmp = tmp->arglist_next;
                     tmp->arglist_next = al;
                 }
             }
             yyval.value_arglist = yyvsp[-2].value_arglist;
         ;
    break;}
case 57:
#line 324 "parser.y"
{
             arglist *al = malloc(sizeof(arglist));
             if (al != NULL) {
                 al->arglist_arg = yyvsp[0].value_type;
                 al->arglist_next = NULL;
             }
             yyval.value_arglist = al;
         ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 333 "parser.y"


line *make_line(int type, void *dollar_one)
{
    line *ln;
    parse_debug("LINE\n");
    ln = malloc(sizeof(line));
    if (ln != NULL) {
        ln->line_type = type;
        ln->line_value.option = dollar_one;
        ln->line_next = NULL;
    }
    return ln;
}
              
int yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}

line *preferences = NULL;

int main(int argc, char **argv)
{
    yyparse();
    printf("preferences = 0x%lx\n", preferences);
}