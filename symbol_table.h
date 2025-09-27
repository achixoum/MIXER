#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "syndax_tree.h"
#include "log.h"

extern int SYMBOL_TABLE_SIZE;
extern bool has_error;

//structure of a symbol for the symbol table
typedef struct Symbol {
      char* name;
      char* function_scope;
      struct Symbol* next_symbol;
      char type[4];
      bool is_function;
      bool is_variable;
      bool is_recursive;
      int value;
      int index_in_table;
      int method_index;
      int params_num;
      int locals_num;
      int temps_num;
      char* locals_table;
      char* params_table;
      bool is_param;
      bool initialized;
}symbol;

typedef struct symbol_table{
      int size;
      int symbols_counter;
      int method_counter;
      symbol** table;
}Hash_table;


void init_hash_table(Hash_table* hash_table);
int make_hash_key(const char* symb, const char* function_scope);
symbol* new_symbol(const char* name,const char* function_scope);
void add_symbol(Hash_table* hash_table, symbol* symb);
symbol* find_symbol(const Hash_table hash_table,const char* name, const char* function_scope);
void update_value(symbol* symb, int value);
AST_Node* build_symbol_table(AST_Node* node, Hash_table* hash_table, char* function_scope, bool inside_declare, bool is_param, bool inside_assign, bool function_call);
int decide_expression(AST_Node* node, Hash_table hash_table, char* function_scope, bool* is_initialized);
void print_symbol_table(Hash_table* hash_table);
void add_param_to_method(Hash_table hash_table, char* name, char* function_scope);
int decide_params(AST_Node* node);
void check_break(AST_Node* node, bool enter_loop);
void check_main(Hash_table hash_table);
void set_table_name(symbol* symb, char* name,int table);
void add_local_to_method(Hash_table hash_table, char* name, char* function_scope);
void destroy_symbol_table(Hash_table* hash_table);
void free_symbol(symbol* symb);

#endif