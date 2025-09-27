#include "syndax_tree.h"
#include "log.h"
#include <stdio.h>

static int nodes_created = 0;
static int nodes_freed = 0;

AST_Node* create_node(node_type type, char* value, AST_Node* left, AST_Node* right, AST_Node* head, AST_Node* next, int line_count) {
    //if head is not null it means that we want to add to the list,
    //so we add the next node to the end of the list and return the head after
    if (head != NULL) {
        AST_Node* current = head;
        //current->type = type;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = next;
        return head;
    }
    //otherwise create the node with its children
    AST_Node* new_node = (AST_Node*) malloc(sizeof(AST_Node));
    nodes_created++;
    new_node->type = type;
    new_node->line_count = line_count;
    if (value != NULL) {
        new_node->value = (char*) malloc(strlen(value)+1);
        strcpy(new_node->value, value);
    }
    else
        new_node->value = NULL;
    new_node->left = left;
    new_node->right = right;
    new_node->next = NULL;

    return new_node;
}

void print_AST(AST_Node* node, int spaces) {

    if (node == NULL)
        return;

    for (int i=0; i<spaces; i++) {
        fprintf(file, " ");
    }

    switch (node->type) {
        case NODE_PROGRAM:
            fprintf(file, "PROGRAM");
            break;
        case NODE_METH:
            fprintf(file, "METHOD");
            break;
        case NODE_PARAM:
            fprintf(file, "PARAMS");
            break;
        case NODE_BODY:
            fprintf(file, "BODY");
            break;
        case NODE_DECLS:
            fprintf(file, "DECLARATIONS");
            break;
        case NODE_DECL:
            fprintf(file, "DECLARE");
            break;
        case NODE_ID:
            fprintf(file, "ID");
            break;
        case NODE_STMTS:
            fprintf(file, "STATEMENTS");
            break;
        case NODE_ARGS:
            fprintf(file, "ARGUMENTS");
            break;
        case NODE_STMT:
            fprintf(file, "STATEMENT");
            break;
        case NODE_ASSIGN:
            fprintf(file, "ASSIGN");
            break;
        case NODE_RETURN:
            fprintf(file, "RETURN");
            break;
        case NODE_IF:
            fprintf(file, "IF");
            break;
        case NODE_WHILE:
            fprintf(file, "WHILE");
            break;
        case NODE_BREAK:
            fprintf(file, "BREAK");
            break;
        case NODE_BLOCK:
            fprintf(file, "BLOCK");
            break;
        case NODE_OP:
            fprintf(file, "OP");
            break;
        case NODE_CALL:
            fprintf(file, "FUNCTION CALL");
            break;
        case NODE_NUM:
            fprintf(file,"NUM");
            break;
        case NODE_BOOL:
            fprintf(file, "BOOL");
            break;
        case NODE_ARG:
            fprintf(file, "ARGUMENT");
            break;
        case NODE_TYPE:
            fprintf(file, "TYPE");
            break;
        default:
            fprintf(file, "UNKNOWN");
    }
    if (node->value != NULL) {
        fprintf(file," (%s)", node->value);
    }
    fprintf(file, "\n");

    print_AST(node->left, spaces + 2);
    print_AST(node->right, spaces + 2);
    print_AST(node->next, spaces);
    nodes_freed++;
}

void destroy_AST(AST_Node* node) {
    if (node == NULL)
        return;

    destroy_AST(node->left);
    destroy_AST(node->right);
    destroy_AST(node->next);

    free(node->value);
    free(node);
}


void print_counters() {
    printf("Nodes created: %d\n", nodes_created);
    printf("Nodes freed:   %d\n", nodes_freed);
}

const char* node_type_to_string(node_type type) {
    switch (type) {
        case NODE_PROGRAM: return "NODE_PROGRAM";
        case NODE_METH:    return "NODE_METH";
        case NODE_PARAM:   return "NODE_PARAM";
        case NODE_BODY:    return "NODE_BODY";
        case NODE_DECLS:   return "NODE_DECLS";
        case NODE_DECL:    return "NODE_DECL";
        case NODE_ID:      return "NODE_ID";
        case NODE_STMTS:   return "NODE_STMTS";
        case NODE_ARGS:    return "NODE_ARGS";

        case NODE_ASSIGN:  return "NODE_ASSIGN";
        case NODE_RETURN:  return "NODE_RETURN";
        case NODE_IF:      return "NODE_IF";
        case NODE_WHILE:   return "NODE_WHILE";
        case NODE_BREAK:   return "NODE_BREAK";
        case NODE_BLOCK:   return "NODE_BLOCK";
        case NODE_EMPTY:   return "NODE_EMPTY";

        case NODE_OP:      return "NODE_OP";
        case NODE_CALL:    return "NODE_CALL";
        case NODE_NUM:     return "NODE_NUM";
        case NODE_BOOL:    return "NODE_BOOL";
        case NODE_ARG:     return "NODE_ARG";
        case NODE_TYPE:    return "NODE_TYPE";
        case NODE_STMT:    return "NODE_STMT";

        default:           return "UNKNOWN_NODE";
    }
}