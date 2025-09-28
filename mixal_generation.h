#ifndef MIXAL_GENERATION_H
#define MIXAL_GENERATION_H

#include "symbol_table.h"
#include <stdio.h>



bool is_number(const char* value);
void generate_tables_code(Hash_table hash_table);
void generate_method_code(AST_Node* node, Hash_table hash_table, const char* method, bool is_recursive);
void generate_call_code(AST_Node* node, Hash_table hash_table, const char* method, symbol* func);
void init_locals(AST_Node* node, Hash_table hash_table, const char* method, bool is_recursive);
void generate_mixal(AST_Node* node, Hash_table hash_table);
void generate_expression_code(AST_Node* node, Hash_table hash_table, const char* method, bool is_recursive);
void check_condition(char* value, char* branch, int num);
void generate_stack();
bool recursive_function_exists(Hash_table hash_table);

#endif
