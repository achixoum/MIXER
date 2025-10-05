%{
 
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


%}

%union
{
    int   integer;
    char*  string;
    char  character;
    AST_Node* node;
}

%token <integer> FALSE, TRUE, IF, ELSE, WHILE, INT, RETURN, BREAK
%token <string> NUM, LE GE EQ NE
%token <character> '(' ')' '{' '}' ';' ','
%token <character> '*' '/' '+' '-' '=' '<' '>'
%token <string> ID

%type <node> Program Meth_list Meth Type Params Body Formals Decls Decl_list Decl Stmts Stmt Vars Assign Block Location Expr
%type <node> Method Add_expr Term Factor Actuals Args
%type <string> Relop Addop Mulop

%start Program

%%

Program
    :Meth_list {
        fprintf(file,"Program -> Meth_list\n");
        $$ = create_node(NODE_PROGRAM, NULL, $1, NULL, NULL, NULL, line_count);
        root = $$;
    }
    | {
        root = NULL;
    }
    ;

Meth_list:
    Meth Meth_list {
        fprintf(file,"Meth_list -> Meth Meth_list\n");
        $$ = create_node(NODE_PROGRAM, NULL, NULL, NULL, $1, $2, -1);
    }
    | Meth {
        fprintf(file,"Meth_list -> Meth\n");
        $$ = $1;
    }
    ;

Meth:
    Type ID '(' Params ')' Body {
        fprintf(file,"Meth -> Type ID ( Params ) Body\n");
        $$ = create_node(NODE_METH, $2, $4, $6, NULL, NULL, line_count);
        SYMBOL_TABLE_SIZE++;
    }
    ;

Params:
    Formals Type ID {
        fprintf(file,"Params -> Formals Type ID\n");
        AST_Node* head = create_node(NODE_ID, $3, $2, NULL, NULL, NULL, line_count);
        if ($1 == NULL) {
            $$ = create_node(NODE_PARAM, NULL, head, NULL, NULL, NULL, line_count);
            create_node(NODE_PARAM, NULL, NULL, NULL, head, NULL, -1);
        }
        else {
            $$ = create_node(NODE_PARAM, NULL, $1, NULL, NULL, NULL, line_count);
            create_node(NODE_PARAM, NULL, NULL, NULL, $1, head, -1);
        }
        SYMBOL_TABLE_SIZE++;
    }
    | {
        $$ = NULL;
    }
    ;

Formals:
    Formals Type ID ',' {
        fprintf(file,"Formals -> Formals Type ID ,\n");
        AST_Node* param = create_node(NODE_ID, $3, $2, NULL, NULL, NULL, line_count);
        if ($1 == NULL)
            $$ = param;
        else
            $$ = create_node(NODE_ID, NULL, NULL, NULL, $1, param, -1);
        SYMBOL_TABLE_SIZE++;
    }
    | {
        $$ = NULL;
    }
    ;

Type:
    INT {
        fprintf(file,"Type -> INT\n");
        $$ = create_node(NODE_TYPE, "int", NULL, NULL, NULL, NULL, line_count);
    }
    ;

Body:
    '{' Decls Stmts '}' {
        fprintf(file,"Body -> { Decls Stmts }\n");
        AST_Node* stmts = create_node(NODE_STMTS, NULL, $3, NULL, NULL, NULL, line_count);
        $$ = create_node(NODE_BODY, NULL, $2, stmts, NULL, NULL, line_count);
    }
    ;

Decls:
    Decl_list Decl {
        fprintf(file,"Decls -> Decl_list Decl\n");
        if ($1 == NULL) {
            $$ = create_node(NODE_DECLS, NULL, $2 , NULL, NULL, NULL, line_count);
        }
        else {
            $$ = create_node(NODE_DECLS, NULL, $1, NULL, NULL, NULL, line_count);
            create_node(NODE_DECLS, NULL, NULL, NULL, $1, $2, -1);
        }
    }
    | {
        $$ = NULL;
    }
    ;

Decl_list:
    Decl_list Decl {
        fprintf(file,"Decl_list -> Decl_list Decl\n");
        if ($1 == NULL)
            $$ = $2;
        else
            create_node(NODE_DECL, NULL, NULL, NULL, $1, $2, -1);
    }
    | {
        $$ = NULL;
    }
    ;

Decl:
    Type ID Vars ';' {
        fprintf(file,"Decl -> Type ID Vars ;\n");
        AST_Node* node = create_node(NODE_ID, $2, NULL, NULL, NULL, NULL, line_count);
        if ($3 == NULL) {
            $$ = create_node(NODE_DECL, NULL, $1, node, NULL, NULL, line_count);
            create_node(NODE_DECL, NULL, NULL, NULL, node, NULL, -1);
        }
        else {
            $$ = create_node(NODE_DECL, NULL, $1, node, NULL, NULL, line_count);
            create_node(NODE_DECL, NULL, NULL, NULL, node, $3, -1);
        }
        SYMBOL_TABLE_SIZE++;
    }
    | Type ID '=' Expr Vars ';' {
        fprintf(file,"Decl -> Type ID = Expr Vars ;\n");
        AST_Node* id = create_node(NODE_ID, $2, NULL, NULL, NULL, NULL, line_count);
        AST_Node* node = create_node(NODE_ASSIGN, NULL, id, $4, NULL, NULL, line_count);
        if ($5 == NULL) {
            $$ = create_node(NODE_DECL, NULL, $1, node, NULL, NULL, line_count);
            create_node(NODE_DECL, NULL, NULL, NULL, node, NULL, -1);
        }
        else {
            $$ = create_node(NODE_DECL, NULL, $1, node, NULL, NULL, line_count);
            create_node(NODE_ASSIGN, NULL, NULL, NULL, node, $5, -1);
        }
        SYMBOL_TABLE_SIZE++;
    }
    ;

Vars:
    ',' ID Vars {
        fprintf(file,"Vars -> , ID Vars\n");
        AST_Node* var = create_node(NODE_ID, $2, NULL, NULL, NULL, NULL, line_count);
        if ($3 == NULL)
            $$ = var;
        else
            $$ = create_node(NODE_ID, NULL, NULL, NULL, var, $3, -1);
    }
    | ',' ID '=' Expr Vars {
        fprintf(file,"Vars -> , ID = Expr Vars\n");
        AST_Node* id = create_node(NODE_ID, $2, NULL, NULL, NULL, NULL, line_count);
        AST_Node* var = create_node(NODE_ASSIGN, NULL, id, $4, NULL, NULL, line_count);
        if ($5 == NULL) {
            $$ = var;
        }
        else {
            $$ = create_node(NODE_ASSIGN, NULL, NULL, NULL, var, $5, -1);
        }
    }
    | {
        $$ = NULL;
    }
    ;

Stmts:
    Stmts Stmt {
        fprintf(file,"Stmts -> Stmts Stmt\n");
        AST_Node* stmt = create_node(NODE_STMT, NULL, $2, NULL, NULL, NULL, line_count);
        if ($1 == NULL)
            $$ = stmt;
        else
            $$ = create_node(NODE_STMT, NULL, NULL, NULL, $1, stmt, -1);
    }
    | {
        $$ = NULL;
    }
    ;

Stmt:
    Assign ';' {
        fprintf(file,"Stmt -> Assign ;\n");
        $$ = $1;
    }
    | RETURN Expr ';' {
        fprintf(file,"Stmt -> RETURN Expr ;\n");
        $$ = create_node(NODE_RETURN, NULL, $2, NULL, NULL, NULL, line_count);
    }
    | IF '(' Expr ')' Stmt ELSE Stmt {
        fprintf(file,"Stmt -> IF ( Expr ) Stmt ELSE Stmt\n");
        AST_Node* head = create_node(NODE_IF, NULL, NULL, NULL, $3, NULL, line_count);
        $$ = create_node(NODE_IF, NULL, head, NULL, NULL, NULL, line_count);
        create_node(NODE_IF, NULL, NULL, NULL, head, $5, -1);
        create_node(NODE_IF, NULL, NULL, NULL, head, $7, -1);
    }
    | WHILE '(' Expr ')' Stmt {
        fprintf(file,"Stmt -> WHILE ( Expr ) Stmt\n");
        $$ = create_node(NODE_WHILE, NULL, $3, $5, NULL, NULL, line_count);
    }
    | BREAK ';' {
        fprintf(file,"Stmt -> BREAK ;\n");
        $$ = create_node(NODE_BREAK, NULL, NULL, NULL, NULL, NULL, line_count);
    }
    | Block {
        fprintf(file,"Stmt -> Block\n");
        $$ = $1;
    }
    | ';' {
        fprintf(file,"Stmt -> ; (empty stmt)\n");
        $$ = NULL;
    }
    ;

Block:
    '{' Stmts '}' {
        fprintf(file,"Block -> { Stmts }\n");
        $$ = create_node(NODE_BLOCK, NULL, $2, NULL, NULL, NULL, line_count);
    }
    ;

Assign:
    Location '=' Expr {
        fprintf(file,"Assign -> Location = Expr\n");
        $$ = create_node(NODE_ASSIGN, NULL, $1, $3, NULL, NULL, line_count);
    }
    ;

Location:
    ID {
        fprintf(file,"Location -> ID\n");
        $$ = create_node(NODE_ID, $1, NULL, NULL, NULL, NULL, line_count);
    }
    ;

Method:
    ID {
        fprintf(file,"Method -> ID\n");
        $$ = create_node(NODE_ID, $1, NULL, NULL, NULL, NULL, line_count);
    }
    ;

Expr:
    Add_expr Relop Add_expr {
        fprintf(file,"Expr -> Add_expr Relop Add_expr\n");
        $$ = create_node(NODE_OP, $2, $1, $3, NULL, NULL, line_count);
    }
    | Add_expr {
        fprintf(file,"Expr -> Add_expr\n");
        $$ = $1;
    }
    ;


Relop:
    LE { $$ = $1; }
    | '<' { $$ = strdup("<"); }
    | '>' { $$ = strdup(">"); }
    | GE { $$ = $1; }
    | EQ { $$ = $1; }
    | NE { $$ = $1; }
    ;

Add_expr:
    Add_expr Addop Term {
        fprintf(file,"Add_expr -> Add_expr Addop Term\n");
        $$ = create_node(NODE_OP, $2, $1, $3, NULL, NULL, line_count);
    }
    | Term {
        fprintf(file,"Add_expr -> Term\n");
        $$ = $1;
    }
    ;

Addop:
    '+' { $$ = strdup("+"); }
    | '-' { $$ = strdup("-"); }
    ;

Term:
    Term Mulop Factor {
        fprintf(file,"Term -> Term Mulop Factor\n");
        $$ = create_node(NODE_OP, $2, $1, $3, NULL, NULL, line_count);
    }
    | Factor {
        fprintf(file,"Term -> Factor\n");
        $$ = $1;
    }
    ;

Mulop:
    '*' { $$ = strdup("*"); }
    | '/' { $$ = strdup("/"); }
    ;

Factor:
    '(' Expr ')' {
        fprintf(file,"Factor -> ( Expr )\n");
        $$ = $2;
    }
    | Location {
        fprintf(file,"Factor -> Location\n");
        $$ = $1;
    }
    | NUM {
        fprintf(file,"Factor -> NUM\n");
        $$ = create_node(NODE_NUM, $1, NULL, NULL ,NULL, NULL, line_count);
    }
    | '-' NUM {
        fprintf(file,"Factor -> NUM\n");
        $$ = create_node(NODE_NUM, strncat(strdup("-"),$2, strlen($2)), NULL, NULL ,NULL, NULL, line_count);
    }
    | TRUE {
        fprintf(file,"Factor -> TRUE\n");
        $$ = create_node(NODE_BOOL, "true", NULL, NULL, NULL ,NULL, line_count);
    }
    | FALSE {
        fprintf(file,"Factor -> FALSE\n");
        $$ = create_node(NODE_BOOL,"false", NULL, NULL, NULL ,NULL, line_count);
    }
    | Method '(' Actuals ')' {
        fprintf(file,"Factor -> Method ( Actuals )\n");
        $$ = create_node(NODE_CALL, NULL, $1, $3, NULL, NULL, line_count);
    }
    ;

Actuals:
    Args Expr {
        fprintf(file,"Actuals -> Args Expr\n");
        AST_Node* node = create_node(NODE_ARG, NULL, $2, NULL, NULL, NULL, line_count);
        if ($1 == NULL) {
            $$ = create_node(NODE_ARGS, NULL, node, NULL, NULL, NULL, line_count);
            //create_node(NODE_ARG, NULL, NULL, NULL, node, NULL);
        }
        else {
            $$ = create_node(NODE_ARGS, NULL, $1, NULL, NULL, NULL, line_count);
            create_node(NODE_ARG, NULL, NULL, NULL, $1, node, -1);
        }
    }
    | {
        $$ = NULL;
    }
    ;

Args:
    Args Expr ',' {
        fprintf(file,"Args -> Args Expr ,\n");
        AST_Node* arg = create_node(NODE_ARG, NULL, $2, NULL, NULL, NULL, line_count);
        if ($1 == NULL)
            $$ = arg;
        else
            $$ = create_node(NODE_ARG, NULL, NULL, NULL, $1, arg, -1);
    }
    | {
        $$ = NULL;
    }
    ;

%%


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
           generate_stack();

       generate_mixal(root, symbol_table);
   }

   destroy_AST(root);

   destroy_symbol_table(&symbol_table);

   close_files();

}
