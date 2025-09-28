#include "mixal_generation.h"

#include <inttypes.h>
#include <ctype.h>

#include "y.tab.h"

int methods = 0;
int temp = 0;
int expr = 0;
int func_temps = 0;
int branches = 0;
int loops = 0;
int argument = 0;

bool is_number(const char* value) {
    for (int i=0; value[i] != '\0'; i++) {
        if (!isdigit(value[i]))
            return false;
    }

    return true;
}

void generate_tables_code(Hash_table hash_table) {

    char buffer[10];

    fprintf(mixal_file, "       ORIG 0\n");
    for (int i=0; i<hash_table.size; i++) {
        symbol* current = hash_table.table[i];

        while (current != NULL) {
            if (current->is_function && !current->is_recursive) {
                if (current->params_num>0) {
                    sprintf(buffer, "TBL%d%d", methods, 1);
                    set_table_name(current, buffer, 1);
                    fprintf(mixal_file, "%s      ORIG *+%d\n",current->params_table,current->params_num);
                }
                if (current->locals_num>0) {
                    sprintf(buffer, "TBL%d%d", methods, 2);
                    set_table_name(current, buffer, 2);
                    fprintf(mixal_file, "%s      ORIG *+%d\n",current->locals_table,current->locals_num);
                }
                methods++;
            }
            current = current->next_symbol;
        }
    }
    methods = 0;
}

bool recursive_function_exists(Hash_table hash_table) {

    for (int i=0; i<hash_table.size; i++) {
        symbol* current = hash_table.table[i];

        while (current != NULL) {
            if (current->is_function && current->is_recursive)
                return true;
             current = current->next_symbol;
        }
    }

    return false;
}

void generate_stack() {
    //initialize and allocate memory for the stack
    fprintf(mixal_file, "       ORIG 1000\n");
    //place that the recursive function call will be store
    //includes declared locals, function parameters and return address
    fprintf(mixal_file, "STACK     ORIG *+200\n");
    //place for the temporary symbols used inside the function, that help with the evaluation of an expression
    fprintf(mixal_file, "VARS     ORIG *+200\n");
    //the stack pointer and the pointer the temporary value
    fprintf(mixal_file, "SP     CON -1\n");
    fprintf(mixal_file, "L     CON -1\n\n");

    //subroutine that push a word to the stack
    fprintf(mixal_file,"PUSH    STJ EXITPUSH\n");
    fprintf(mixal_file,"        LD3 SP\n");
    fprintf(mixal_file,"        INC3 1\n");
    fprintf(mixal_file, "        STA STACK,3\n");
    fprintf(mixal_file, "        ST3 SP\n");
    fprintf(mixal_file, "EXITPUSH JMP *\n\n");

    //subroutine that push a temporary value to the array vars
    fprintf(mixal_file,"PUSHL    STJ EXITPUSHL\n");
    fprintf(mixal_file,"        STA RECA(0:2)\n");
    fprintf(mixal_file,"        ST4 EXPR\n");
    fprintf(mixal_file,"        LDA EXPR\n");
    fprintf(mixal_file,"        ST5 INDEX\n");
    fprintf(mixal_file,"        ADD INDEX\n");
    fprintf(mixal_file,"        STA EXPR\n");
    fprintf(mixal_file,"        LD4 EXPR\n");
    fprintf(mixal_file,"RECA    ENTA *\n");
    fprintf(mixal_file, "EXITPUSHL JMP *\n\n");

    //generate the code for the pop
    fprintf(mixal_file,"POPL    STJ EXITPOPL\n");
    fprintf(mixal_file,"        STA RECPOPL(0:2)\n");
    fprintf(mixal_file,"        LDA L\n");
    fprintf(mixal_file, "        SUB VAR\n");
    fprintf(mixal_file, "        STA L\n");
    fprintf(mixal_file, "        SUB VAR\n");
    fprintf(mixal_file, "        INCA 1\n");
    fprintf(mixal_file, "        STA T\n");
    fprintf(mixal_file, "        LD5 T\n");
    fprintf(mixal_file, "RECPOPL ENTA *\n");
    fprintf(mixal_file, "EXITPOPL JMP *\n\n");

    //generate the code of the pop
    fprintf(mixal_file,"POP    STJ EXITPOP\n");
    fprintf(mixal_file,"        STA RECPOP(0:2)\n");
    fprintf(mixal_file,"        LDA SP\n");
    fprintf(mixal_file, "        SUB LOCALS\n");
    fprintf(mixal_file, "        STA TEMP2\n");
    fprintf(mixal_file, "        LD3 TEMP2\n");
    fprintf(mixal_file, "        LD4 STACK,3(1:2)\n");
    fprintf(mixal_file, "        ST4 EXIT0(1:2)\n");
    fprintf(mixal_file, "        DECA 1\n");
    fprintf(mixal_file, "        SUB PARAMS\n");
    fprintf(mixal_file, "        STA SP\n");
    fprintf(mixal_file, "        SUB PARAMS\n");
    fprintf(mixal_file, "        INCA 1\n");
    fprintf(mixal_file, "        STA TEMP2\n");
    fprintf(mixal_file, "        LD2 TEMP2\n");
    fprintf(mixal_file, "        SUB PARAMS\n");
    fprintf(mixal_file, "        STA TEMP2\n");
    fprintf(mixal_file, "        LD1 TEMP2\n");
    fprintf(mixal_file, "RECPOP  ENTA *\n");
    fprintf(mixal_file, "EXITPOP JMP *\n\n");

    fprintf(mixal_file,"SETINDEX    STJ EXITINDX\n");
    fprintf(mixal_file,"            STA RECAI(0:2)\n");
    fprintf(mixal_file, "           ST5 INDEX\n");
    fprintf(mixal_file, "           ST4 TEMP\n");
    fprintf(mixal_file, "           LDA INDEX\n");
    fprintf(mixal_file, "           ADD TEMP\n");
    fprintf(mixal_file, "           STA INDEX\n");
    fprintf(mixal_file, "           LD6 INDEX\n");
    fprintf(mixal_file, "RECAI     ENTA *\n");
    fprintf(mixal_file, "EXITINDX    JMP *\n\n");
}

void generate_call_code(AST_Node* node, Hash_table hash_table, const char* method, symbol* func) {
    if (node == NULL)
        return;

    switch (node->type) {
        case NODE_ARGS:
            generate_call_code(node->left, hash_table, method, func);
            break;
        case NODE_CALL:
            if (func->is_recursive) {
                if (strcmp(func->name, method) !=0) {
                    //recover the index register that are been used by the function after a recursive function call
                    fprintf(mixal_file, "           ST1 RECI1(0:2)\n");
                    fprintf(mixal_file, "           ST2 RECI2(0:2)\n");
                    fprintf(mixal_file, "           LDA =%d=\n", func->locals_num);
                    fprintf(mixal_file, "           STA LOCALS\n");
                    fprintf(mixal_file, "           LDA =%d=\n", func->params_num);
                    fprintf(mixal_file, "           STA PARAMS\n");
                    fprintf(mixal_file, "           LDA =%d=\n", func->temps_num);
                    fprintf(mixal_file, "           STA VAR\n");
                }
                generate_call_code(node->right, hash_table, method, func);

                fprintf(mixal_file, "           JMP %s\n", func->name);
                if (strcmp(func->name, method) != 0) {
                    fprintf(mixal_file, "RECI1      ENT1 *\n");
                    fprintf(mixal_file, "RECI2      ENT2 *\n");
                }
            }
            else {
                fprintf(mixal_file, "           ENT3 %s\n", func->params_table);
                generate_call_code(node->right, hash_table, method, func);
                fprintf(mixal_file, "           JMP %s\n", func->name);
            }
            break;
        case NODE_ARG:
            generate_expression_code(node->left, hash_table, method, strcmp(func->name, method)==0?true:false);
            if (func->is_recursive)
                fprintf(mixal_file, "           JMP PUSH\n");
            else
                fprintf(mixal_file, "           STA %d,3\n", argument++);
            generate_call_code(node->next, hash_table, method, func);
            break;
        default:
    }
}

void generate_expression_code(AST_Node* node, Hash_table hash_table, const char* method, bool is_recursive) {
    if (node == NULL)
        return;

    switch (node->type) {
        case NODE_OP:
            int expr_temp = expr;
            //store the first operand of the expression in the A register
            generate_expression_code(node->right, hash_table, method, is_recursive);
            if (is_recursive) {
                fprintf(mixal_file, "           LD4 =%d=\n", expr_temp);
                fprintf(mixal_file, "           JMP PUSHL\n");
                fprintf(mixal_file, "           STA VARS,4\n");
            }
            else
                fprintf(mixal_file, "           STA TEMP%d\n", expr_temp);
            expr++;
            func_temps++;
            //generate the right instruction for each operation
            //applying the operation with the left operand and the A register(right operand)
            generate_expression_code(node->left, hash_table, method, is_recursive);
            if (is_recursive) {
                fprintf(mixal_file, "           LD4 =%d=\n", expr_temp);
                fprintf(mixal_file, "           JMP SETINDEX\n");
            }
            if (strcmp(node->value, "+") == 0)
                if (is_recursive)
                    fprintf(mixal_file, "           ADD VARS,6\n");
                else
                    fprintf(mixal_file, "           ADD TEMP%d\n", expr_temp);
            else if (strcmp(node->value, "-") == 0) {
                //in subtrack operation we have store the two operands in opposite order
                //value = right_operand(A register)-left operand(TEMP)
                if (is_recursive)
                    fprintf(mixal_file, "           SUB VARS,6\n");
                else
                    fprintf(mixal_file, "           SUB TEMP%d\n", expr_temp);
            }
            else if (strcmp(node->value, "*") == 0) {
                //mult operation stored in the AX register
                if (is_recursive)
                    fprintf(mixal_file, "           MUL VARS,6\n");
                else
                    fprintf(mixal_file, "           MUL TEMP%d\n", expr_temp);
                //shift all 5 bytes from X register to A register
                fprintf(mixal_file, "           SLAX 5\n");
            }
            else if (strcmp(node->value, "/") == 0) {
                //div operation stored in the A register but the divided is on AX so we shift right
                fprintf(mixal_file, "           SRAX 5\n");
                if (is_recursive)
                    fprintf(mixal_file, "           DIV VARS,6\n");
                else
                    fprintf(mixal_file, "           DIV TEMP%d\n", expr_temp);
                //the remainder is in the X so we set to zero again
                fprintf(mixal_file, "           ENTX 0\n");
            }
            else
                if (is_recursive) {
                    fprintf(mixal_file, "           CMPA VARS,6\n");
                }
                else
                    fprintf(mixal_file, "           CMPA TEMP%d\n", expr_temp);
            break;
        case NODE_ID:
            symbol* var = find_symbol(hash_table, node->value, method);
            if (is_recursive) {
                if (var->is_param) {
                    fprintf(mixal_file, "           ST1 INDEX\n");
                    fprintf(mixal_file, "           LDA INDEX\n");
                    fprintf(mixal_file, "           ADD =%d=\n", var->index_in_table);
                    fprintf(mixal_file, "           STA INDEX\n");
                    fprintf(mixal_file, "           LD6 INDEX\n");
                    fprintf(mixal_file, "           LDA STACK,6\n");
                }
                else {
                    fprintf(mixal_file, "           ST2 INDEX\n");
                    fprintf(mixal_file, "           LDA INDEX\n");
                    fprintf(mixal_file, "           ADD =%d=\n", var->index_in_table);
                    fprintf(mixal_file, "           STA INDEX\n");
                    fprintf(mixal_file, "           LD6 INDEX\n");
                    fprintf(mixal_file, "           LDA STACK,6\n");
                }
            }
            else {
                if (var->is_param)
                    fprintf(mixal_file, "           LDA %d,1\n", var->index_in_table);
                else
                    fprintf(mixal_file, "           LDA %d,2\n", var->index_in_table);
            }
            break;
        case NODE_NUM:
            fprintf(mixal_file, "           LDA =%s=\n", node->value);
            break;
        case NODE_BOOL:
            if (strcmp(node->value,"true") == 0)
                fprintf(mixal_file, "           LDA =1=\n");
            else
                fprintf(mixal_file, "           LDA =2=\n");
            break;
        //case NODE_ARGS:
            //generate_expression_code(node->left, hash_table, method, is_recursive);
            //break;
        case NODE_CALL:
            symbol* func = find_symbol(hash_table, node->left->value, "global");
            argument = 0;
            generate_call_code(node,hash_table, method, func);
            break;
        default:
            break;
    }
}


void init_locals(AST_Node* node, Hash_table hash_table, const char* method, bool is_recursive) {
    if (node == NULL)
        return;

    switch (node->type) {
        case NODE_DECL:
            if (is_recursive) {
                if (node->right == NULL) {
                    fprintf(mixal_file, "           LDA =0=\n");
                    fprintf(mixal_file, "           JMP PUSH\n");
                }
            }
            init_locals(node->right, hash_table, method, is_recursive);
            init_locals(node->next, hash_table, method,is_recursive);
            break;
        case NODE_ASSIGN:
            symbol* var = find_symbol(hash_table, node->left->value, method);
            if (is_recursive) {
                if (var->initialized) {
                    fprintf(mixal_file, "           LDA =%d=\n", var->value);
                    fprintf(mixal_file, "           JMP PUSH\n");
                }
                else {
                    generate_expression_code(node->right, hash_table, method, is_recursive);
                    fprintf(mixal_file, "           JMP PUSH\n");
                }
            }
            else {
                if (var->initialized) {
                    fprintf(mixal_file, "           LDA =%d=\n", var->value);
                    fprintf(mixal_file, "           STA %d,2\n", var->index_in_table);
                }
                else {
                    generate_expression_code(node->right, hash_table, method, is_recursive);
                    fprintf(mixal_file, "           STA %d,2\n", var->index_in_table);
                }
            }
            break;
        default:
            break;
    }
}

void check_condition(char* value, char* branch, int num) {
    if (strcmp(value, "<") == 0)
        fprintf(mixal_file, "           JL %s%d\n",branch, num);
    if (strcmp(value, ">") == 0)
        fprintf(mixal_file, "           JG %s%d\n", branch,num);
    if (strcmp(value, "<=") == 0)
        fprintf(mixal_file, "           JLE %s%d\n", branch,num);
    if (strcmp(value, ">=") == 0)
        fprintf(mixal_file, "           JGE %s%d\n", branch,num);
    if (strcmp(value, "==") == 0)
        fprintf(mixal_file, "           JE %s%d\n", branch,num);
    if (strcmp(value, "!=") == 0)
        fprintf(mixal_file, "           JNE %s%d\n", branch, num);
    else {
        fprintf(mixal_file, "           CMPA =0=\n");
        fprintf(mixal_file, "           JNE %s%d\n", branch, num);
    }
}

void generate_method_code(AST_Node* node, Hash_table hash_table, const char* method, bool is_recursive) {
    if (node == NULL)
        return;

    switch (node->type) {
        case NODE_DECLS:
            if (is_recursive) {
                fprintf(mixal_file, "           LD2 SP\n");
                fprintf(mixal_file, "           INC2 1\n");
            }
            init_locals(node->left, hash_table, method, is_recursive);
            break;
        case NODE_BODY:
            generate_method_code(node->left, hash_table, method, is_recursive);
            generate_method_code(node->right, hash_table, method, is_recursive);
            break;
        case NODE_BLOCK:
            generate_method_code(node->left, hash_table, method, is_recursive);
            break;
        case NODE_STMT:
            generate_method_code(node->left, hash_table, method, is_recursive);
            generate_method_code(node->next, hash_table, method, is_recursive);
            break;
        case NODE_STMTS:
            generate_method_code(node->left, hash_table, method, is_recursive);
            break;
        case NODE_ASSIGN:
            symbol* var = find_symbol(hash_table, node->left->value, method);
            generate_expression_code(node->right, hash_table, method, is_recursive);
            if (is_recursive) {
                if (var->is_param) {
                    fprintf(mixal_file, "           ST1 INDEX\n");
                    fprintf(mixal_file, "           LDA INDEX\n");
                    fprintf(mixal_file, "           ADD =%d=\n", var->index_in_table);
                    fprintf(mixal_file, "           STA INDEX\n");
                    fprintf(mixal_file, "           LD6 INDEX\n");
                    fprintf(mixal_file, "           STA STACK,6\n");
                }
                else {
                    fprintf(mixal_file, "           ST2 INDEX\n");
                    fprintf(mixal_file, "           LDA INDEX\n");
                    fprintf(mixal_file, "           ADD =%d=\n", var->index_in_table);
                    fprintf(mixal_file, "           STA INDEX\n");
                    fprintf(mixal_file, "           LD6 INDEX\n");
                    fprintf(mixal_file, "           STA STACK,6\n");
                }
            }
            else {
                if (var->is_param)
                    fprintf(mixal_file, "           STA %d,1\n", var->index_in_table);
                else
                    fprintf(mixal_file, "           STA %d,2\n", var->index_in_table);
            }
            break;
        case NODE_WHILE:
            int loops_temp = loops;
            temp = loops;
            fprintf(mixal_file, "COND%d", loops_temp);
            generate_expression_code(node->left, hash_table, method, is_recursive);
            check_condition(node->left->value, "LOOP", loops_temp);
            fprintf(mixal_file, "           JMP ENDLOOP%d\n", loops_temp);
            fprintf(mixal_file, "LOOP%d       NOP\n", loops_temp);
            loops++;
            generate_method_code(node->right, hash_table, method, is_recursive);
            fprintf(mixal_file, "           JMP COND%d\n",loops_temp);
            fprintf(mixal_file, "ENDLOOP%d      NOP\n", loops_temp);
            break;
        case NODE_IF:
            //generate code for the condition
            int branch_temp = branches;
            generate_expression_code(node->left, hash_table, method, is_recursive);
            check_condition(node->left->value, "IF", branch_temp);
            fprintf(mixal_file, "           JMP ELSE%d\n", branch_temp);
            fprintf(mixal_file, "IF%d     NOP\n", branches);
            branches++;
            generate_method_code(node->left->next, hash_table, method, is_recursive);
            fprintf(mixal_file, "           JMP ENDIF%d\n", branch_temp);
            fprintf(mixal_file, "ELSE%d     NOP\n", branch_temp);
            generate_method_code(node->left->next->next, hash_table, method, is_recursive);
            fprintf(mixal_file,"ENDIF%d     NOP\n", branch_temp);
            break;
        case NODE_RETURN:
            generate_expression_code(node->left, hash_table, method, is_recursive);
            symbol* function = find_symbol(hash_table, method, "global");
            if (is_recursive) {
                fprintf(mixal_file, "           JMP POPL\n");
                fprintf(mixal_file, "           JMP POP\n");
                fprintf(mixal_file, "           JMP EXIT%d\n", methods);
            }
            else {
                if (function->params_table>0)
                    fprintf(mixal_file, "          JMP REC%d\n", methods);
                else if (function->locals_num>0)
                    fprintf(mixal_file, "          JMP REC%d\n", methods+1);
                else
                    fprintf(mixal_file, "           JMP EXIT%d\n", methods);
                break;
            }
            break;
        case NODE_BREAK:
            fprintf(mixal_file, "           JMP ENDLOOP%d\n", temp);
            break;
        default:
            break;
    }
}

void generate_mixal(AST_Node* node, Hash_table hash_table) {
    if (node == NULL)
        return;

    switch (node->type) {
        case NODE_PROGRAM:
            fprintf(mixal_file, "       ORIG 2000\n");
            generate_mixal(node->left, hash_table);
            fprintf(mixal_file, "       HLT\n");
            fprintf(mixal_file, "       END MAIN\n");
            break;
        case NODE_METH:
            char buffer[11];
            symbol* function = find_symbol(hash_table, node->value, "global");
            //store the return address from where the method was call
            //initialize the return value with 0, RET will store the return value
            fprintf(mixal_file, "\n%s     STJ EXIT%d\n",function->name,methods);
            //allocate so much memory as the number of the parameters and the local variable of the function
            //in two different tables, if there is zero of them then allocate nothing
            //initialize the index register with the base of the two tables
            if (function->is_recursive) {
                if (function->params_num > 0)
                    fprintf(mixal_file, "           LD1 SP\n");
                fprintf(mixal_file, "           LD3 SP\n");
                fprintf(mixal_file, "           LD5 L\n");
                fprintf(mixal_file, "           INC5 1\n");
                fprintf(mixal_file, "           LDA VAR\n");
                fprintf(mixal_file, "           ADD L\n");
                fprintf(mixal_file, "           STA L\n");
                fprintf(mixal_file, "           INC3 1\n");
                fprintf(mixal_file, "           STJ STACK,3\n");
                fprintf(mixal_file, "           ST3 SP\n");
                generate_method_code(node->right, hash_table, function->name, true);
                function->temps_num = func_temps;
                func_temps = 0;
                fprintf(mixal_file, "EXIT%d     JMP *\n", methods);
            }
            else {
                if (function->params_num > 0) {
                    fprintf(mixal_file, "           ST1 REC%d(0:2)\n", methods);
                    fprintf(mixal_file,"            ENT1 %s\n", function->params_table);
                }

                if (function->locals_num > 0) {
                    fprintf(mixal_file, "           ST2 REC%d(0:2)\n", methods+1);
                    fprintf(mixal_file,"            ENT2 %s\n", function->locals_table);
                }

                //generate code for the body of the method
                generate_method_code(node->right, hash_table, function->name, false);
                if (function->params_table>0)
                    fprintf(mixal_file, "REC%d      ENT1 *\n", methods);
                if (function->locals_num>0)
                    fprintf(mixal_file, "REC%d      ENT2 *\n", methods+1);
                fprintf(mixal_file, "EXIT%d     JMP *\n", methods);
            }
            methods++;
            generate_mixal(node->next, hash_table);
            break;
        default:
            break;
    }
}
