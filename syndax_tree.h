#ifndef SYNDAX_TREE
#define SYNDAX_TREE

#include <stdlib.h>
#include <string.h>


typedef enum {
    //root and structure
    NODE_PROGRAM,  //node for the program (stores the list of methods)
    NODE_METH,     //node for the method (stores body and params)
    NODE_PARAM,    //node for the parameters (stores list of types and ids)
    NODE_BODY,     //node for the body (stores declarations and statements)
    NODE_DECLS,    //node for the declarations(stores the list of declarations)
    NODE_DECL,     //node for the declaration (stores list of id)
    NODE_ID,      //node for the variables (stores the id)
    NODE_STMTS,    //node for the statements of a function(store list of statements)
    NODE_ARGS,     //node for the arguments in a function call(stores list of arguments)

    //statements
    NODE_ASSIGN,
    NODE_RETURN,
    NODE_IF,
    NODE_WHILE,
    NODE_BREAK,
    NODE_BLOCK,
    NODE_EMPTY,

    //Expressions
    NODE_OP,
    NODE_CALL,
    NODE_NUM,
    NODE_BOOL,
    NODE_ARG,
    NODE_TYPE,
    NODE_STMT
}node_type;

//abstract syntax tree node can have two children or multiple children
//for that reason the struct has another variable call next that represents a list of those children
typedef struct AST_Node {
    node_type type;
    int line_count;
    char* value;
    struct AST_Node* left;
    struct AST_Node* right;
    struct AST_Node* next;
}AST_Node;


AST_Node* create_node(node_type type, char* value, AST_Node* left, AST_Node* right, AST_Node* head, AST_Node* next, int line_count);
void print_AST(AST_Node* node, int spaces);
void destroy_AST(AST_Node* root);
void print_counters();
const char* node_type_to_string(node_type type);
void error(const char* s);

#endif