#include "../headers/calculator.h"
#include <string.h>
#include <ctype.h>

/**
 * TODO:
 * Edit: compute function
 * Edit: get_result function
 * add support for multiple operands
 * improve syntax checking
 */

/* temporary functions */
void print_token_type(token_t token)
{
    if(token.type == LB) printf("LB: ");
    else if(token.type == RB) printf("RB: ");
    else if(token.type == OPERATOR) printf("OPERATOR: ");
    else if(token.type == OPERAND) printf("OPERAND: ");
    else if(token.type == SPACE) printf("SPACE: ");
    else if(token.type == UNKNOWN) printf("UNKNOWN: ");
    else if(token.type == NEWLINE) printf("NEWLINE: ");
    else printf("ERR: ");
    if(token.type == OPERAND) printf("Value: %d\n",token.value);
    else printf("c: %c, decimal: %d\n",token.sym,token.sym);
}

bool is_number(char * num)
{
    if(num == NULL) return false;

    if(*num == '-') num += 1;
    while (*num != ' ' && *num != ')')
    {
        if(!isdigit(*num)) return false;
        num += 1;
    }
    return true;
}

bool is_operation(char op)
{
    switch (op)
    {
    case '+':
    case '-':
    case '*':
    case '/':
        return true;
    default:
        return false;
    }
}

operation_t get_operation(char sym)
{
    switch (sym)
    {
    case '+':
        return ADD;
    case '-':
        return SUB;
    case '*':
        return MUL;
    case '/':
        return DIV;
    default:
        return NOT_SUPPORTED;
    }
}

token_t get_token(char ** expr)
{
    token_t current_token;
    if(isdigit(**expr))
    {
        char buffer[MAX_FLOAT_LENGTH] = {0};
        int idx = 0;
        while (isdigit(**expr)) 
        {
            buffer[idx] = **expr;
            (*expr)++;
            idx += 1;
        }
        current_token.type = OPERAND;
        current_token.value = atoi(buffer);
        return current_token;
    }
    else if(**expr == '(') current_token.type = LB;
    else if(**expr == ')') current_token.type = RB;
    else if(is_operation(**expr)) current_token.type = OPERATOR;
    else if (**expr == ' ') current_token.type = SPACE;
    else if (**expr == '\n') current_token.type = NEWLINE;
    else
    {
        current_token.type = UNKNOWN;
        (*expr)++;
        return current_token;
    }
    current_token.sym = **expr;
    (*expr)++;
    return current_token;
}

frac_t compute(frac_t op1,frac_t op2,char operator)
{
    switch (get_operation(operator))
    {
    case ADD:
        return frac_add(op1,op2);
    case SUB:
        return frac_sub(op1,op2);
    case MUL:
        return frac_mul(op1,op2);
    case DIV:
        return frac_div(op1,op2);
    default: 
        return ERR_FRAC;
    }
}

/**
 * @brief Get the result of expresion expr
 * note:    it supports only (op E E), where 
 *          op -> operation
 *          E -> expresion
 * note2:   need to check if the first char in expr is '(', then call get_result
 * @param expr 
 * @param err_msg_buffer 
 * @return float 
 */
frac_t get_result(char ** expr)
{
    // printf("EXPR: %s\n",*expr);
    if(is_number(*expr)) return num_to_frac(atoi(*expr));

    bool LB_found = false;
    bool RB_found = false;

    int operand_num = 0;
    //use dynamic arr for multiple operands support
    frac_t operands[2];
    operands[0] = ERR_FRAC;
    operands[1] = ERR_FRAC;
    char operator = 0;
    
    token_t current_token = get_token(expr);
    
    while (current_token.type != NEWLINE)
    {
        switch (current_token.type)
        {
        case LB:
            if(LB_found) 
            {
                return ERR_FRAC;
            }
            else LB_found = true;

            current_token = get_token(expr);
            if(current_token.type != OPERATOR) return ERR_FRAC;
            break;
        case OPERATOR:
            operator = current_token.sym;
            current_token = get_token(expr);
            if(current_token.type != SPACE)  return ERR_FRAC;
            break;
        case SPACE:
            current_token = get_token(expr);
            if(current_token.type != OPERAND && current_token.type != LB) return ERR_FRAC;
            if(current_token.type == LB)
            {
                operands[operand_num] = get_result(expr);
                if(operands[operand_num].denominator == 0) return ERR_FRAC;
                current_token = get_token(expr);  
                operand_num += 1;
            }
            break;
        case OPERAND:
            if(operand_num >= 2) 
            {
                printf("Too many operands\n"); return ERR_FRAC;
            }
            operands[operand_num] = num_to_frac(current_token.value);
            operand_num += 1;
            current_token = get_token(expr);
            if(operand_num == sizeof(operands)/sizeof(frac_t)) 
            {
                if(current_token.type != RB) return ERR_FRAC;
            }
            if(current_token.type != SPACE && current_token.type != RB ) return ERR_FRAC;
            break;
        case RB:
            if(RB_found)
            {
                return ERR_FRAC;
            }
            RB_found = true; 
            // if(operand_num == sizeof(operands)/sizeof(frac_t)) 
            // {
            //     current_token = get_token(expr);
            //     printf("before comp: %d\n",current_token.sym);
            //     current_token = get_token(expr);
            //     printf("before comp: %d\n",current_token.sym);
            // }
            frac_t result = compute(operands[0],operands[1],operator);
            int floor_result = result.numerator / result.denominator;
            if(floor_result >= 0) return result;
            return ERR_FRAC;
        default:
            return ERR_FRAC;
        }
    }
    return compute(operands[0],operands[1],operator);
}
