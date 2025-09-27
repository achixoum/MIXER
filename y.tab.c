#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 2 "syd.y"
 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "syndax_tree.h"
#include "log.h"
#include "symbol_table.h"
#include "mixal_generation.h"

#define DEBUG 0

/* ----------------------------------------------------------- */
/* --------- Declarations for Syntax-Semantic Analysis ------- */
/* ----------------------------------------------------------- */

int yyparse();
int yylex();
AST_Node* root;
Hash_table symbol_table;
int line_count = 1;

void yyerror(const char* s) {
    fprintf(stderr, "Line.%d |     Syntax Error\n", line_count);
    has_error = true;
}


#line 32 "syd.y"
typedef union
{
    int   integer;
    char*  string;
    char  character;
    AST_Node* node;
} YYSTYPE;
#line 49 "y.tab.c"
#define FALSE 257
#define TRUE 258
#define IF 259
#define ELSE 260
#define WHILE 261
#define INT 262
#define RETURN 263
#define BREAK 264
#define NUM 265
#define LE 266
#define GE 267
#define EQ 268
#define NE 269
#define ID 270
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    0,    1,    1,    2,    4,    4,    6,    6,    3,
    5,    7,    7,    8,    8,    9,    9,   12,   12,   12,
   10,   10,   11,   11,   11,   11,   11,   11,   11,   14,
   13,   15,   17,   16,   16,   23,   23,   23,   23,   23,
   23,   18,   18,   24,   24,   19,   19,   25,   25,   20,
   20,   20,   20,   20,   20,   20,   21,   21,   22,   22,
};
short yylen[] = {                                         2,
    1,    0,    2,    1,    6,    3,    0,    4,    0,    1,
    4,    2,    0,    2,    0,    4,    6,    3,    5,    0,
    2,    0,    2,    3,    7,    5,    2,    1,    1,    3,
    3,    1,    1,    3,    1,    1,    1,    1,    1,    1,
    1,    3,    1,    1,    1,    3,    1,    1,    1,    3,
    1,    1,    2,    1,    1,    4,    2,    0,    3,    0,
};
short yydefred[] = {                                      0,
   10,    0,    1,    0,    0,    3,    0,    0,    0,    0,
    0,    0,    0,    5,    0,   22,    0,    8,    0,    0,
    0,    0,    0,    0,    0,   22,   11,   29,   32,   21,
    0,   28,    0,    0,    0,    0,   55,   54,   52,    0,
    0,    0,   51,    0,    0,    0,    0,   47,   27,    0,
   23,    0,    0,    0,    0,    0,    0,    0,   53,   24,
    0,   36,   39,   40,   41,   44,   45,   37,   38,    0,
    0,   48,   49,    0,   30,   31,    0,    0,   16,    0,
    0,   50,    0,    0,    0,    0,   46,    0,   18,    0,
    0,   26,   56,    0,    0,   17,    0,   59,   19,   25,
};
short yydgoto[] = {                                       2,
    3,    4,    5,    9,   14,   10,   16,   17,   21,   19,
   30,   55,   31,   32,   43,   44,   45,   46,   47,   48,
   83,   84,   70,   71,   74,
};
short yysindex[] = {                                   -237,
    0,    0,    0, -237, -244,    0,   20,    0,    8, -237,
  -60, -205,    0,    0,   31,    0, -237,    0,    5, -194,
    0,   37,   38,  -38,   24,    0,    0,    0,    0,    0,
   25,    0,   18,  -20,  -38,  -38,    0,    0,    0,  -38,
 -180,    0,    0,   27,   47,  -14,   -8,    0,    0,   13,
    0,  -38, -182,  -38,   30,   49,   51,   52,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -38,
  -38,    0,    0,  -38,    0,    0,   10,   50,    0,   21,
   21,    0,   55,  -38,  -39,   -8,    0,  -38,    0,   40,
 -163,    0,    0,   57,   50,    0,   21,    0,    0,    0,
};
short yyrindex[] = {                                    100,
    0,    0,    0,  102,    0,    0,    0,   -5,    0,    0,
    0,    0,  -56,    0,   62,    0,    0,    0,    0,    0,
   -3,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   45,    0,    0,    0,    0,    0,    0,
    0,  -32,    0,    0,    0,    9,  -27,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -40,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   45,   45,    0,    0,
    0,    0,    0,    0,   11,  -22,    0,    0,    0,    0,
    0,    0,    0,   64,   45,    0,    0,    0,    0,    0,
};
short yygindex[] = {                                      0,
  103,    0,   56,    0,    0,    0,    0,    0,    0,   80,
  -23,  -33,    0,    0,    1,    7,    0,   39,   41,   34,
    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 291
short yytable[] = {                                      60,
   58,   40,   13,   66,   60,   67,   41,   33,   32,   32,
   32,   32,   32,   43,   32,   43,   43,   43,   42,   33,
   42,   42,   42,   53,    1,    7,   32,   32,   66,   32,
   67,   43,   43,   72,   43,    7,   42,   42,   73,   42,
   54,   56,   57,   89,   90,   68,   58,   69,   11,   35,
   33,   34,   35,   53,   34,   12,   91,   92,   76,    8,
   78,   99,   13,   28,   15,   12,   13,   35,   13,   34,
   88,   28,   20,  100,   18,   34,   35,   36,   52,   28,
   33,   33,   49,   51,   59,   60,   61,   77,   79,   80,
   94,   81,   82,   53,   95,   93,   97,   33,   96,    2,
   98,    4,    6,   20,   57,   50,    6,   87,   85,    0,
    0,   86,    0,    0,    0,    0,    0,    0,    0,   12,
    0,   12,    0,    0,    0,    0,    0,   26,    0,   27,
    0,    0,    0,    0,    0,   26,    0,   75,    0,    0,
    0,    0,    0,   26,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   13,    0,   13,   15,   13,   13,    0,    0,
    0,    0,    0,   13,    0,    0,   60,   60,   37,   38,
    0,    0,    0,    0,   60,    0,   39,    0,    0,   60,
    0,   42,    0,   32,   32,   32,   32,    0,   43,   43,
   43,   43,    0,   42,   42,   42,   42,    0,    0,    0,
    0,   62,   63,   64,   65,   12,    9,   12,   14,   12,
   12,    0,    0,   22,    0,   23,   12,   24,   25,    0,
    0,   22,    0,   23,   29,   24,   25,    0,    0,   22,
    0,   23,   29,   24,   25,    0,    0,    0,    0,    0,
   29,
};
short yycheck[] = {                                      40,
   41,   40,   59,   43,   45,   45,   45,   40,   41,   42,
   43,   44,   45,   41,   47,   43,   44,   45,   41,   19,
   43,   44,   45,   44,  262,  270,   59,   60,   43,   62,
   45,   59,   60,   42,   62,   41,   59,   60,   47,   62,
   61,   35,   36,   77,   78,   60,   40,   62,   41,   41,
   50,   41,   44,   44,   44,   59,   80,   81,   52,   40,
   54,   95,  123,   59,  270,   10,  123,   59,  125,   59,
   61,   59,   17,   97,   44,  270,   40,   40,   61,   59,
   80,   81,   59,   59,  265,   59,   40,  270,   59,   41,
   84,   41,   41,   44,   88,   41,  260,   97,   59,    0,
   44,    0,   41,   59,   41,   26,    4,   74,   70,   -1,
   -1,   71,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  123,
   -1,  125,   -1,   -1,   -1,   -1,   -1,  123,   -1,  125,
   -1,   -1,   -1,   -1,   -1,  123,   -1,  125,   -1,   -1,
   -1,   -1,   -1,  123,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  259,   -1,  261,  262,  263,  264,   -1,   -1,
   -1,   -1,   -1,  270,   -1,   -1,  257,  258,  257,  258,
   -1,   -1,   -1,   -1,  265,   -1,  265,   -1,   -1,  270,
   -1,  270,   -1,  266,  267,  268,  269,   -1,  266,  267,
  268,  269,   -1,  266,  267,  268,  269,   -1,   -1,   -1,
   -1,  266,  267,  268,  269,  259,  262,  261,  262,  263,
  264,   -1,   -1,  259,   -1,  261,  270,  263,  264,   -1,
   -1,  259,   -1,  261,  270,  263,  264,   -1,   -1,  259,
   -1,  261,  270,  263,  264,   -1,   -1,   -1,   -1,   -1,
  270,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 270
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'","'*'","'+'","','","'-'",0,"'/'",0,0,0,0,0,0,0,0,0,0,0,
"';'","'<'","'='","'>'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,"FALSE","TRUE","IF","ELSE","WHILE","INT","RETURN","BREAK",
"NUM","LE","GE","EQ","NE","ID",
};
char *yyrule[] = {
"$accept : Program",
"Program : Meth_list",
"Program :",
"Meth_list : Meth Meth_list",
"Meth_list : Meth",
"Meth : Type ID '(' Params ')' Body",
"Params : Formals Type ID",
"Params :",
"Formals : Formals Type ID ','",
"Formals :",
"Type : INT",
"Body : '{' Decls Stmts '}'",
"Decls : Decl_list Decl",
"Decls :",
"Decl_list : Decl_list Decl",
"Decl_list :",
"Decl : Type ID Vars ';'",
"Decl : Type ID '=' Expr Vars ';'",
"Vars : ',' ID Vars",
"Vars : ',' ID '=' Expr Vars",
"Vars :",
"Stmts : Stmts Stmt",
"Stmts :",
"Stmt : Assign ';'",
"Stmt : RETURN Expr ';'",
"Stmt : IF '(' Expr ')' Stmt ELSE Stmt",
"Stmt : WHILE '(' Expr ')' Stmt",
"Stmt : BREAK ';'",
"Stmt : Block",
"Stmt : ';'",
"Block : '{' Stmts '}'",
"Assign : Location '=' Expr",
"Location : ID",
"Method : ID",
"Expr : Add_expr Relop Add_expr",
"Expr : Add_expr",
"Relop : LE",
"Relop : '<'",
"Relop : '>'",
"Relop : GE",
"Relop : EQ",
"Relop : NE",
"Add_expr : Add_expr Addop Term",
"Add_expr : Term",
"Addop : '+'",
"Addop : '-'",
"Term : Term Mulop Factor",
"Term : Factor",
"Mulop : '*'",
"Mulop : '/'",
"Factor : '(' Expr ')'",
"Factor : Location",
"Factor : NUM",
"Factor : '-' NUM",
"Factor : TRUE",
"Factor : FALSE",
"Factor : Method '(' Actuals ')'",
"Actuals : Args Expr",
"Actuals :",
"Args : Args Expr ','",
"Args :",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 410 "syd.y"


int main(void) {
   init_file("lex_parser_logs.txt");

   yyparse();

   fprintf(file, "\n----------------AST------------------\n");

   print_AST(root, 0);

   init_hash_table(&symbol_table);

   build_symbol_table(root,&symbol_table, "global", false, false, false, false);

   check_main(symbol_table);

   check_break(root, false);

   print_symbol_table(&symbol_table);

   if (!has_error) {
       init_file("mixal.mixal");

       generate_tables_code(symbol_table);

       if (recursive_function_exists(symbol_table))
           generate_stack(symbol_table);

       generate_mixal(root, symbol_table);
   }

   destroy_AST(root);

   destroy_symbol_table(&symbol_table);

   close_files();

}
#line 335 "y.tab.c"
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 55 "syd.y"
{
        fprintf(file,"Program -> Meth_list\n");
        yyval.node = create_node(NODE_PROGRAM, NULL, yyvsp[0].node, NULL, NULL, NULL, line_count);
        root = yyval.node;
    }
break;
case 2:
#line 60 "syd.y"
{
        root = NULL;
    }
break;
case 3:
#line 66 "syd.y"
{
        fprintf(file,"Meth_list -> Meth Meth_list\n");
        yyval.node = create_node(NODE_PROGRAM, NULL, NULL, NULL, yyvsp[-1].node, yyvsp[0].node, -1);
    }
break;
case 4:
#line 70 "syd.y"
{
        fprintf(file,"Meth_list -> Meth\n");
        yyval.node = yyvsp[0].node;
    }
break;
case 5:
#line 77 "syd.y"
{
        fprintf(file,"Meth -> Type ID ( Params ) Body\n");
        yyval.node = create_node(NODE_METH, yyvsp[-4].string, yyvsp[-2].node, yyvsp[0].node, NULL, NULL, line_count);
        SYMBOL_TABLE_SIZE++;
    }
break;
case 6:
#line 85 "syd.y"
{
        fprintf(file,"Params -> Formals Type ID\n");
        AST_Node* head = create_node(NODE_ID, yyvsp[0].string, yyvsp[-1].node, NULL, NULL, NULL, line_count);
        if (yyvsp[-2].node == NULL) {
            yyval.node = create_node(NODE_PARAM, NULL, head, NULL, NULL, NULL, line_count);
            create_node(NODE_PARAM, NULL, NULL, NULL, head, NULL, -1);
        }
        else {
            yyval.node = create_node(NODE_PARAM, NULL, yyvsp[-2].node, NULL, NULL, NULL, line_count);
            create_node(NODE_PARAM, NULL, NULL, NULL, yyvsp[-2].node, head, -1);
        }
        SYMBOL_TABLE_SIZE++;
    }
break;
case 7:
#line 98 "syd.y"
{
        yyval.node = NULL;
    }
break;
case 8:
#line 104 "syd.y"
{
        fprintf(file,"Formals -> Formals Type ID ,\n");
        AST_Node* param = create_node(NODE_ID, yyvsp[-1].string, yyvsp[-2].node, NULL, NULL, NULL, line_count);
        if (yyvsp[-3].node == NULL)
            yyval.node = param;
        else
            yyval.node = create_node(NODE_ID, NULL, NULL, NULL, yyvsp[-3].node, param, -1);
        SYMBOL_TABLE_SIZE++;
    }
break;
case 9:
#line 113 "syd.y"
{
        yyval.node = NULL;
    }
break;
case 10:
#line 119 "syd.y"
{
        fprintf(file,"Type -> INT\n");
        yyval.node = create_node(NODE_TYPE, "int", NULL, NULL, NULL, NULL, line_count);
    }
break;
case 11:
#line 126 "syd.y"
{
        fprintf(file,"Body -> { Decls Stmts }\n");
        AST_Node* stmts = create_node(NODE_STMTS, NULL, yyvsp[-1].node, NULL, NULL, NULL, line_count);
        yyval.node = create_node(NODE_BODY, NULL, yyvsp[-2].node, stmts, NULL, NULL, line_count);
    }
break;
case 12:
#line 134 "syd.y"
{
        fprintf(file,"Decls -> Decl_list Decl\n");
        if (yyvsp[-1].node == NULL) {
            yyval.node = create_node(NODE_DECLS, NULL, yyvsp[0].node , NULL, NULL, NULL, line_count);
        }
        else {
            yyval.node = create_node(NODE_DECLS, NULL, yyvsp[-1].node, NULL, NULL, NULL, line_count);
            create_node(NODE_DECLS, NULL, NULL, NULL, yyvsp[-1].node, yyvsp[0].node, -1);
        }
    }
break;
case 13:
#line 144 "syd.y"
{
        yyval.node = NULL;
    }
break;
case 14:
#line 150 "syd.y"
{
        fprintf(file,"Decl_list -> Decl_list Decl\n");
        if (yyvsp[-1].node == NULL)
            yyval.node = yyvsp[0].node;
        else
            create_node(NODE_DECL, NULL, NULL, NULL, yyvsp[-1].node, yyvsp[0].node, -1);
    }
break;
case 15:
#line 157 "syd.y"
{
        yyval.node = NULL;
    }
break;
case 16:
#line 163 "syd.y"
{
        fprintf(file,"Decl -> Type ID Vars ;\n");
        AST_Node* node = create_node(NODE_ID, yyvsp[-2].string, NULL, NULL, NULL, NULL, line_count);
        if (yyvsp[-1].node == NULL) {
            yyval.node = create_node(NODE_DECL, NULL, yyvsp[-3].node, node, NULL, NULL, line_count);
            create_node(NODE_DECL, NULL, NULL, NULL, node, NULL, -1);
        }
        else {
            yyval.node = create_node(NODE_DECL, NULL, yyvsp[-3].node, node, NULL, NULL, line_count);
            create_node(NODE_DECL, NULL, NULL, NULL, node, yyvsp[-1].node, -1);
        }
        SYMBOL_TABLE_SIZE++;
    }
break;
case 17:
#line 176 "syd.y"
{
        fprintf(file,"Decl -> Type ID = Expr Vars ;\n");
        AST_Node* id = create_node(NODE_ID, yyvsp[-4].string, NULL, NULL, NULL, NULL, line_count);
        AST_Node* node = create_node(NODE_ASSIGN, NULL, id, yyvsp[-2].node, NULL, NULL, line_count);
        if (yyvsp[-1].node == NULL) {
            yyval.node = create_node(NODE_DECL, NULL, yyvsp[-5].node, node, NULL, NULL, line_count);
            create_node(NODE_DECL, NULL, NULL, NULL, node, NULL, -1);
        }
        else {
            yyval.node = create_node(NODE_DECL, NULL, yyvsp[-5].node, node, NULL, NULL, line_count);
            create_node(NODE_ASSIGN, NULL, NULL, NULL, node, yyvsp[-1].node, -1);
        }
        SYMBOL_TABLE_SIZE++;
    }
break;
case 18:
#line 193 "syd.y"
{
        fprintf(file,"Vars -> , ID Vars\n");
        AST_Node* var = create_node(NODE_ID, yyvsp[-1].string, NULL, NULL, NULL, NULL, line_count);
        if (yyvsp[0].node == NULL)
            yyval.node = var;
        else
            yyval.node = create_node(NODE_ID, NULL, NULL, NULL, var, yyvsp[0].node, -1);
    }
break;
case 19:
#line 201 "syd.y"
{
        fprintf(file,"Vars -> , ID = Expr Vars\n");
        AST_Node* id = create_node(NODE_ID, yyvsp[-3].string, NULL, NULL, NULL, NULL, line_count);
        AST_Node* var = create_node(NODE_ASSIGN, NULL, id, yyvsp[-1].node, NULL, NULL, line_count);
        if (yyvsp[0].node == NULL) {
            yyval.node = var;
        }
        else {
            yyval.node = create_node(NODE_ASSIGN, NULL, NULL, NULL, var, yyvsp[0].node, -1);
        }
    }
break;
case 20:
#line 212 "syd.y"
{
        yyval.node = NULL;
    }
break;
case 21:
#line 218 "syd.y"
{
        fprintf(file,"Stmts -> Stmts Stmt\n");
        AST_Node* stmt = create_node(NODE_STMT, NULL, yyvsp[0].node, NULL, NULL, NULL, line_count);
        if (yyvsp[-1].node == NULL)
            yyval.node = stmt;
        else
            yyval.node = create_node(NODE_STMT, NULL, NULL, NULL, yyvsp[-1].node, stmt, -1);
    }
break;
case 22:
#line 226 "syd.y"
{
        yyval.node = NULL;
    }
break;
case 23:
#line 232 "syd.y"
{
        fprintf(file,"Stmt -> Assign ;\n");
        yyval.node = yyvsp[-1].node;
    }
break;
case 24:
#line 236 "syd.y"
{
        fprintf(file,"Stmt -> RETURN Expr ;\n");
        yyval.node = create_node(NODE_RETURN, NULL, yyvsp[-1].node, NULL, NULL, NULL, line_count);
    }
break;
case 25:
#line 240 "syd.y"
{
        fprintf(file,"Stmt -> IF ( Expr ) Stmt ELSE Stmt\n");
        AST_Node* head = create_node(NODE_IF, NULL, NULL, NULL, yyvsp[-4].node, NULL, line_count);
        yyval.node = create_node(NODE_IF, NULL, head, NULL, NULL, NULL, line_count);
        create_node(NODE_IF, NULL, NULL, NULL, head, yyvsp[-2].node, -1);
        create_node(NODE_IF, NULL, NULL, NULL, head, yyvsp[0].node, -1);
    }
break;
case 26:
#line 247 "syd.y"
{
        fprintf(file,"Stmt -> WHILE ( Expr ) Stmt\n");
        yyval.node = create_node(NODE_WHILE, NULL, yyvsp[-2].node, yyvsp[0].node, NULL, NULL, line_count);
    }
break;
case 27:
#line 251 "syd.y"
{
        fprintf(file,"Stmt -> BREAK ;\n");
        yyval.node = create_node(NODE_BREAK, NULL, NULL, NULL, NULL, NULL, line_count);
    }
break;
case 28:
#line 255 "syd.y"
{
        fprintf(file,"Stmt -> Block\n");
        yyval.node = yyvsp[0].node;
    }
break;
case 29:
#line 259 "syd.y"
{
        fprintf(file,"Stmt -> ; (empty stmt)\n");
        yyval.node = NULL;
    }
break;
case 30:
#line 266 "syd.y"
{
        fprintf(file,"Block -> { Stmts }\n");
        yyval.node = create_node(NODE_BLOCK, NULL, yyvsp[-1].node, NULL, NULL, NULL, line_count);
    }
break;
case 31:
#line 273 "syd.y"
{
        fprintf(file,"Assign -> Location = Expr\n");
        yyval.node = create_node(NODE_ASSIGN, NULL, yyvsp[-2].node, yyvsp[0].node, NULL, NULL, line_count);
    }
break;
case 32:
#line 280 "syd.y"
{
        fprintf(file,"Location -> ID\n");
        yyval.node = create_node(NODE_ID, yyvsp[0].string, NULL, NULL, NULL, NULL, line_count);
    }
break;
case 33:
#line 287 "syd.y"
{
        fprintf(file,"Method -> ID\n");
        yyval.node = create_node(NODE_ID, yyvsp[0].string, NULL, NULL, NULL, NULL, line_count);
    }
break;
case 34:
#line 294 "syd.y"
{
        fprintf(file,"Expr -> Add_expr Relop Add_expr\n");
        yyval.node = create_node(NODE_OP, yyvsp[-1].string, yyvsp[-2].node, yyvsp[0].node, NULL, NULL, line_count);
    }
break;
case 35:
#line 298 "syd.y"
{
        fprintf(file,"Expr -> Add_expr\n");
        yyval.node = yyvsp[0].node;
    }
break;
case 36:
#line 306 "syd.y"
{ yyval.string = yyvsp[0].string; }
break;
case 37:
#line 307 "syd.y"
{ yyval.string = strdup("<"); }
break;
case 38:
#line 308 "syd.y"
{ yyval.string = strdup(">"); }
break;
case 39:
#line 309 "syd.y"
{ yyval.string = yyvsp[0].string; }
break;
case 40:
#line 310 "syd.y"
{ yyval.string = yyvsp[0].string; }
break;
case 41:
#line 311 "syd.y"
{ yyval.string = yyvsp[0].string; }
break;
case 42:
#line 315 "syd.y"
{
        fprintf(file,"Add_expr -> Add_expr Addop Term\n");
        yyval.node = create_node(NODE_OP, yyvsp[-1].string, yyvsp[-2].node, yyvsp[0].node, NULL, NULL, line_count);
    }
break;
case 43:
#line 319 "syd.y"
{
        fprintf(file,"Add_expr -> Term\n");
        yyval.node = yyvsp[0].node;
    }
break;
case 44:
#line 326 "syd.y"
{ yyval.string = strdup("+"); }
break;
case 45:
#line 327 "syd.y"
{ yyval.string = strdup("-"); }
break;
case 46:
#line 331 "syd.y"
{
        fprintf(file,"Term -> Term Mulop Factor\n");
        yyval.node = create_node(NODE_OP, yyvsp[-1].string, yyvsp[-2].node, yyvsp[0].node, NULL, NULL, line_count);
    }
break;
case 47:
#line 335 "syd.y"
{
        fprintf(file,"Term -> Factor\n");
        yyval.node = yyvsp[0].node;
    }
break;
case 48:
#line 342 "syd.y"
{ yyval.string = strdup("*"); }
break;
case 49:
#line 343 "syd.y"
{ yyval.string = strdup("/"); }
break;
case 50:
#line 347 "syd.y"
{
        fprintf(file,"Factor -> ( Expr )\n");
        yyval.node = yyvsp[-1].node;
    }
break;
case 51:
#line 351 "syd.y"
{
        fprintf(file,"Factor -> Location\n");
        yyval.node = yyvsp[0].node;
    }
break;
case 52:
#line 355 "syd.y"
{
        fprintf(file,"Factor -> NUM\n");
        yyval.node = create_node(NODE_NUM, yyvsp[0].string, NULL, NULL ,NULL, NULL, line_count);
    }
break;
case 53:
#line 359 "syd.y"
{
        fprintf(file,"Factor -> NUM\n");
        yyval.node = create_node(NODE_NUM, strncat(strdup("-"),yyvsp[0].string, strlen(yyvsp[0].string)), NULL, NULL ,NULL, NULL, line_count);
    }
break;
case 54:
#line 363 "syd.y"
{
        fprintf(file,"Factor -> TRUE\n");
        yyval.node = create_node(NODE_BOOL, "true", NULL, NULL, NULL ,NULL, line_count);
    }
break;
case 55:
#line 367 "syd.y"
{
        fprintf(file,"Factor -> FALSE\n");
        yyval.node = create_node(NODE_BOOL,"false", NULL, NULL, NULL ,NULL, line_count);
    }
break;
case 56:
#line 371 "syd.y"
{
        fprintf(file,"Factor -> Method ( Actuals )\n");
        yyval.node = create_node(NODE_CALL, NULL, yyvsp[-3].node, yyvsp[-1].node, NULL, NULL, line_count);
    }
break;
case 57:
#line 378 "syd.y"
{
        fprintf(file,"Actuals -> Args Expr\n");
        AST_Node* node = create_node(NODE_ARG, NULL, yyvsp[0].node, NULL, NULL, NULL, line_count);
        if (yyvsp[-1].node == NULL) {
            yyval.node = create_node(NODE_ARGS, NULL, node, NULL, NULL, NULL, line_count);
            /*create_node(NODE_ARG, NULL, NULL, NULL, node, NULL);*/
        }
        else {
            yyval.node = create_node(NODE_ARGS, NULL, yyvsp[-1].node, NULL, NULL, NULL, line_count);
            create_node(NODE_ARG, NULL, NULL, NULL, yyvsp[-1].node, node, -1);
        }
    }
break;
case 58:
#line 390 "syd.y"
{
        yyval.node = NULL;
    }
break;
case 59:
#line 396 "syd.y"
{
        fprintf(file,"Args -> Args Expr ,\n");
        AST_Node* arg = create_node(NODE_ARG, NULL, yyvsp[-1].node, NULL, NULL, NULL, line_count);
        if (yyvsp[-2].node == NULL)
            yyval.node = arg;
        else
            yyval.node = create_node(NODE_ARG, NULL, NULL, NULL, yyvsp[-2].node, arg, -1);
    }
break;
case 60:
#line 404 "syd.y"
{
        yyval.node = NULL;
    }
break;
#line 932 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
