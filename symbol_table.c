#include "symbol_table.h"

int SYMBOL_TABLE_SIZE=0;
int loop_depth = 0;
//variables to count the number of local variables and parameters of a method
int params = 0;
int locals = 0;
bool has_error = false;

void error(const char* s) {
    fputs(s,stderr);
    putc('\n',stderr);
    has_error = true;
}

void init_hash_table(Hash_table* hash_table) {
    hash_table->table =(symbol**) malloc(SYMBOL_TABLE_SIZE * sizeof(symbol*));
    for (int i=0; i<SYMBOL_TABLE_SIZE; i++) {
        hash_table->table[i] = NULL;
    }
    hash_table->method_counter = 0;
    hash_table->symbols_counter = 0;
    hash_table->size = SYMBOL_TABLE_SIZE;
}

///takes the strings name and scope of the symbol and iterates them,
///adding each character value of the string to the @hash_value then returns
///the index that the symbol will be placed in the table based on the symbol_size
int make_hash_key(const char* name, const char* function_scope) {
    int hash_value=0;

    for(int i=0; name[i] != '\0'; i++) {
        hash_value += name[i];
    }
    for (int i=0; i<function_scope[i] != '\0'; i++) {
        hash_value += function_scope[i];
    }
    return hash_value%SYMBOL_TABLE_SIZE;
}

symbol* new_symbol(const char* name, const char* function_scope) {
    symbol* symb = (symbol*) malloc(sizeof(symbol));

    symb->name = (char*) malloc(strlen(name)+1);
    symb->function_scope = (char* ) malloc(strlen(function_scope)+1);
    strcpy(symb->name, name);
    strcpy(symb->function_scope, function_scope);
    symb->next_symbol = NULL;
    strcpy(symb->type, "int");
    symb->value = 0;
    symb->is_function = false;
    symb->is_variable = false;
    symb->params_num = 0;
    symb->locals_num = 0;
    symb->is_param = false;
    symb->initialized = true;
    symb->index_in_table = 0;
    symb->locals_table = NULL;
    symb->params_table = NULL;
    symb->is_recursive = false;
    symb->temps_num = 0;

    return symb;
}

symbol* find_symbol(const Hash_table hash_table,const char* name,const char* function_scope) {
    int hash_key = make_hash_key(name, function_scope);

    symbol* current = hash_table.table[hash_key];
    while (current != NULL) {
        if (strcmp(current->name, name) != 0)
            current = current->next_symbol;
        else
            return current;
    }

    return current;
}

void add_symbol(Hash_table* hash_table, symbol* symb) {
    int hash_key = make_hash_key(symb->name, symb->function_scope);

    if (strcmp(symb->function_scope, "global") == 0) {
        symb->method_index = hash_table->method_counter;
        hash_table->method_counter++;
    }

    symbol* current = hash_table->table[hash_key];
    symb->next_symbol = current;
    hash_table->table[hash_key] = symb;
    hash_table->symbols_counter++;
}

void update_value(symbol* symb, int value) {
    symb->value = value;
}

///this function is used for checking if we divide by zero in an expression
///also evaluates the result of an expression and return it
///@is_intialized parameter is used when we evaluate an expression that contains other variables(id)
///this parameter will be false if this case happens so that we will not assign the result of the expression when declaring a variable
int decide_expression(AST_Node* node, Hash_table hash_table, char* function_scope,bool* is_initialized) {
    if (node == NULL)
        return 0;

    switch (node->type) {
        case NODE_ID: {
            symbol* symb = find_symbol(hash_table, node->value, function_scope);
            if (symb != NULL) {
                *is_initialized = false;
                return symb->value;
            }
            return -1;
        }
        case NODE_NUM:
            return atoi(node->value);
        case NODE_BOOL:
            if (strcmp(node->value, "true") == 0)
                return 1;
            return 0;
        case NODE_OP: {
            if (strcmp(node->value,"+") == 0)
                return decide_expression(node->left, hash_table, function_scope, is_initialized) + decide_expression(node->right, hash_table, function_scope, is_initialized);
            if (strcmp(node->value,"-") == 0)
                return decide_expression(node->left, hash_table, function_scope, is_initialized) - decide_expression(node->right, hash_table, function_scope, is_initialized);
            if (strcmp(node->value,"*") == 0)
                return decide_expression(node->left, hash_table, function_scope, is_initialized) * decide_expression(node->right, hash_table, function_scope, is_initialized);
            if (strcmp(node->value,"/") == 0) {
                int x = decide_expression(node->right, hash_table, function_scope, is_initialized);
                if (x==0) {
                    char result[256];
                    sprintf(result, "Line.%d |  Error: Division by zero\n", node->line_count);
                    error(result);
                    exit(-1);
                }
                return decide_expression(node->left, hash_table, function_scope,is_initialized) / x;
            }
        }
        default:
            return 0;
    }
}

///this function counts the number of arguments of a function call
int decide_params(AST_Node* node) {
    if (node == NULL)
        return 0;

    switch (node->type) {
        case NODE_ARG:
            return decide_params(node->next) + 1;
        default:
            break;
    }
}

void add_param_to_method(Hash_table hash_table, char* name, char* function_scope) {
    symbol* symb = find_symbol(hash_table, name , function_scope);
    symb->params_num++;
}

void add_local_to_method(Hash_table hash_table, char* name, char* function_scope) {
    symbol* symb = find_symbol(hash_table, name , function_scope);
    symb->locals_num++;
}

void check_break(AST_Node* node, bool enter_loop) {
    if (node == NULL)
        return;

    switch (node->type) {
        case NODE_PROGRAM:
            check_break(node->left, false);
            break;
        case NODE_METH:
            check_break(node->right,false);
            check_break(node->next, false);
            break;
        case NODE_BODY:
            check_break(node->right, false);
            break;
        case NODE_WHILE:
            loop_depth++;
            check_break(node->left, true);
            check_break(node->right, true);
            loop_depth--;
            break;
        case NODE_IF:
            check_break(node->left->next, enter_loop?true:false);
            break;
        case NODE_BREAK:
            if (!enter_loop) {
                char result[256];
                sprintf(result, "Line.%d |   Error: 'break' statement is not within loop\n", node->line_count);
                error(result);
            }
            break;
        case NODE_BLOCK:
            check_break(node->left, enter_loop?true:false);
            break;
        case NODE_STMT:
            check_break(node->left, enter_loop?true:false);
            check_break(node->next, enter_loop?true:false);
            break;
        case NODE_STMTS:
            check_break(node->left, enter_loop?true:false);
            break;
        default:
    }
}

void check_main(Hash_table hash_table) {
    symbol* main = find_symbol(hash_table, "main", "global");
    if (main == NULL) {
        char result[256];
        sprintf(result, "Error: 'main' function is missing\n");
        error(result);
    }
}

///builds the symbol_table as long with the sematic analysis by traversing the AST
///the boolean parameters helping us to catch some cases in the recursive traverse
///for example the @inside_declare variable is set to true when we are declaring variables so that the addition of symbols to the table happens only in declaration
AST_Node* build_symbol_table(AST_Node* node, Hash_table* hash_table, char* function_scope, bool inside_declare, bool is_param, bool inside_assign, bool is_function_call){
    if (node == NULL)
        return NULL;

    char* function = NULL;
    bool declare=false;
    bool param = false;
    bool assign = false;
    bool function_call = false;
    bool is_initialized=true;
    symbol* symb;
    switch (node->type) {
        case NODE_METH: {
            if (find_symbol(*hash_table, node->value,"global") != NULL) {
                char result[256];
                sprintf(result, "Line.%d |   Error: function '%s' has already been defined or declared\n",node->line_count, node->value);
                error(result);
            }
            else {
                params = 0;
                locals = 0;
                symb = new_symbol(node->value, "global");
                symb->is_function = true;
                symb->is_variable = false;
                add_symbol(hash_table, symb);
                function = (char*) malloc(strlen(node->value)+1);
                strcpy(function, node->value);
            }
            break;
        }
        case NODE_PARAM:
            param = true;
            break;
        case NODE_DECL:
            declare = true;
            break;
        case NODE_ID: {
            if (inside_declare || is_param) {
                symbol* temp = find_symbol(*hash_table, node->value,function_scope);
                if (temp != NULL) {
                    if (!inside_assign) {
                        char result[256];
                        sprintf(result, "Line.%d |   Error: variable '%s' has already been defined or declared in '%s'\n",node->line_count, node->value, function_scope);
                        error(result);
                    }
                }
                else {
                    symb = new_symbol(node->value, function_scope);
                    symb->is_function = false;
                    symb->is_variable = true;
                    symb->index_in_table = params;
                    add_symbol(hash_table, symb);
                    if (is_param) {
                        add_param_to_method(*hash_table, function_scope, "global");
                        symb->is_param = true;
                        symb->index_in_table = params;
                        params++;
                    }
                    else {
                        add_local_to_method(*hash_table, function_scope, "global");
                        symb->index_in_table = locals;
                        locals++;
                    }
                }
                declare = inside_declare?true:false;
                param = is_param?true:false;
            }
            else if (!is_function_call) {
                symb = find_symbol(*hash_table, node->value, function_scope);
                if (symb == NULL) {
                    char result[256];
                    sprintf(result, "Line.%d |   Error: undeclared variable '%s' in method '%s'\n",node->line_count, node->value, function_scope);
                    error(result);
                }
            }
            break;
        }
        case NODE_ASSIGN:
            if (inside_declare) {
                int right_value = decide_expression(node->right, *hash_table, function_scope, &is_initialized);
                symbol* left = find_symbol(*hash_table, node->left->value, function_scope);
                if (left == NULL) {
                    symb = new_symbol(node->left->value, function_scope);
                    symb->is_function = false;
                    symb->is_variable = true;
                    symb->index_in_table = locals;
                    if (is_initialized)
                        symb->value = right_value;
                    else
                        symb->initialized = is_initialized;
                    add_symbol(hash_table,symb);
                    add_local_to_method(*hash_table, function_scope, "global");
                    locals++;
                }
                else {
                    char result[256];
                    sprintf(result, "Line.%d |   Error: variable '%s' has already been defined or declared in '%s'\n",node->line_count, node->left->value, function_scope);
                    error(result);
                }
                assign = true;
                declare = true;
            }
            break;
        case NODE_OP:
            decide_expression(node,*hash_table, function_scope, &is_initialized);
            declare = inside_declare?true:false;
            assign = inside_assign?true:false;
            break;
        case NODE_CALL:
            symb = find_symbol(*hash_table, node->left->value, "global");
            if (symb == NULL) {
                char result[256];
                sprintf(result, "Line.%d |   Error: undeclared function '%s'\n",node->line_count, node->left->value);
                error(result);
            }
            else if (decide_params(node->right->left) != symb->params_num) {
                char result[256];
                sprintf(result, "Line.%d |   Error: wrong number of parameters passed for function '%s'\n",node->line_count, symb->name);
                error(result);
            }
            if (symb)
                if (strcmp(symb->name, function_scope) == 0)
                    symb->is_recursive = true;
            function_call = true;
            break;
        case NODE_STMTS:
            declare = false;
            param = false;
        case NODE_STMT:
            break;
        case NODE_BREAK:
            break;
        case NODE_NUM:
            declare = inside_declare?true:false;
            break;
        case NODE_TYPE:
            declare = inside_declare?true:false;
            param = is_param?true:false;
            break;
        default:
            declare = inside_declare?true:false;
            break;
    }
    char* temp;
    if (function == NULL) {
        temp = (char*) malloc(strlen(function_scope)+1);
        strcpy(temp, function_scope);
    }
    else {
        temp = (char*) malloc(strlen(function)+1);
        strcpy(temp, function);
    }

    build_symbol_table(node->left, hash_table, temp, declare, param, assign, function_call);
    build_symbol_table(node->right, hash_table, temp, declare, param, assign, function_call);
    build_symbol_table(node->next, hash_table, temp, declare, param, assign, function_call);
    free(temp);
    free(function);
}

void print_symbol_table(Hash_table* hash_table) {
    fprintf(file,"\n-------------SYMBOL TABLE--------------\n");
    fprintf(file,"Total symbols: %d\n", hash_table->symbols_counter);

    for (int i=0; i<hash_table->size; i++) {
        symbol* current = hash_table->table[i];
        if (current == NULL)
            continue;

        fprintf(file,"Bucket %d:\n", i);

        while (current != NULL) {
            fprintf(file,"  Name: %s | Scope: %s | Type: %s | ", current->name, current->function_scope, current->type);

            if (current->is_function) {
                fprintf(file,"Kind: Function | Parameters: %d Locals: %d Recursive: %s ",current->params_num, current->locals_num, current->is_recursive?"true":"false");
            }
            else if (current->is_param) {
                fprintf(file,"Kind: Parameter | Index in table: %d ",current->index_in_table);
            }
            else if (current->is_variable) {
                fprintf(file,"Kind: Variable | Index in table: %d ", current->index_in_table);
            }

            fprintf(file,"Value: %d \n", current->value);

            current = current->next_symbol;
        }
    }
}

void set_table_name(symbol* symb, char* name, int table) {
    if (table == 1) {
        symb->params_table = (char* ) malloc(strlen(name) + 1);
        strcpy(symb->params_table, name);
    }
    else {
        symb->locals_table = (char* ) malloc(strlen(name) + 1);
        strcpy(symb->locals_table, name);
    }
}

void free_symbol(symbol* symb) {
    free(symb->function_scope);
    free(symb->locals_table);
    free(symb->name);
    free(symb->params_table);
}

void destroy_symbol_table(Hash_table* hash_table) {

    for (int i=0; i<SYMBOL_TABLE_SIZE; i++) {
        symbol* current = hash_table->table[i];
        while (current != NULL) {
            symbol* temp = current;
            current = current->next_symbol;
            free_symbol(temp);
            free(temp);
        }
    }
}