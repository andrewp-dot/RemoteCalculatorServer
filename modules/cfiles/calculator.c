#include "../headers/calculator.h"
#include "../headers/calc_stack.h"
#include <string.h>
#include <ctype.h>

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
    while (*num != ' ' && *num != ')' && *num != '\0')
    {
        if(!isdigit(*num))
        {
            return false;
        }
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
        char buffer[MAX_STACK_SIZE] = {0};
        int idx = 0;
        while (isdigit(**expr)) 
        {
            buffer[idx] = **expr;
            (*expr) += 1;
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
    }
    current_token.sym = **expr;
    (*expr) += 1;
    return current_token;
}

frac_t compute(calc_stack_t * stack,operation_t operator)
{
    // calcs_print(stack);
    if(size(stack) < MIN_OPERANDS) return ERR_FRAC;
    frac_t result = ERR_FRAC;

    switch (operator)
    {
    case ADD:
        result = calcs_pop(stack);
        while (!is_empty(stack))
        {
            result = frac_add(result,calcs_pop(stack));
            if(result.denominator == 0) return ERR_FRAC;
        }
        return result;
    case SUB:
        calcs_reverse(stack);
        result = calcs_pop(stack);
        while (!is_empty(stack))
        {
            result = frac_sub(result,calcs_pop(stack));
            if(result.denominator == 0) return ERR_FRAC;
        }
        return result;
    case MUL:
        result = calcs_pop(stack);
        while (!is_empty(stack))
        {
            result = frac_mul(result,calcs_pop(stack));
            if(result.denominator == 0) return ERR_FRAC;
        }
        return result;
    case DIV:
        calcs_reverse(stack);
        result = calcs_pop(stack);
        while (!is_empty(stack))
        {
            result = frac_div(result,calcs_pop(stack));
            if(result.denominator == 0) return ERR_FRAC;
        }
        return result;
    default:
        break;
    }
    return result;
}


frac_t get_result(char ** expr)
{
    if(is_number(*expr)) return num_to_frac(atoi(*expr));

    bool RB_found = false;
    calc_stack_t stack;
    init_calc_stack(&stack);
    operation_t operator = NOT_SUPPORTED;

    //abort
    token_t current_token = get_token(expr);
    
    while (current_token.type != NEWLINE)
    {
        switch (current_token.type)
        {
        case LB:
            // printf("LB\n");
            current_token = get_token(expr);
            if(current_token.type != OPERATOR) return ERR_FRAC;
            break;
        case OPERATOR:
            // printf("OPERATOR\n");
            operator = get_operation(current_token.sym);
            current_token = get_token(expr);
            if(current_token.type != SPACE) return ERR_FRAC;
            break;
        case SPACE:
            // printf("SPACE\n");
            current_token = get_token(expr);
            if(current_token.type != OPERAND && current_token.type != LB) return ERR_FRAC;
            if(current_token.type == LB)
            {
                frac_t res = get_result(expr);
                if(!calcs_push(&stack,res)) return ERR_FRAC;      
                current_token = get_token(expr);
            }
            break;
        case OPERAND:
            // printf("OPERAND\n");
            if(!calcs_push(&stack,num_to_frac(current_token.value))) return ERR_FRAC;            
            current_token = get_token(expr);
            if(current_token.type != SPACE && current_token.type != RB) return ERR_FRAC;
            break;
        case RB: 
            // printf("RB\n");
            if(RB_found) 
            {
                return ERR_FRAC;
            }
            else RB_found = true;
            if(current_token.type != RB && current_token.type != SPACE && current_token.type != NEWLINE) return ERR_FRAC;
            return compute(&stack,operator);

        default:
            return ERR_FRAC;
        }
    }
    return ERR_FRAC;
}

