#include "../headers/calculator.h"
#include <string.h>
#include <ctype.h>

bool is_number(char * num)
{
    if(num == NULL) return false;

    bool dot_found = false;
    while (*num != '\0')
    {
        if(dot_found && *num == '.') return false;
        if(!dot_found) {
            dot_found = true;
            continue;
        }
        if(!isdigit(*num)) return false;
        num += 1;
    }
    return true;
}

token_t get_token(char ** expr)
{
    token_t current_token;
    if(isdigit(**expr))
    {
        char buffer[MAX_FLOAT_LENGTH] = {0};
        bool dot_found = false;
        int idx = 0;
        while (**expr == '.' || isdigit(**expr))
        {
            if(dot_found && **expr == '.') 
            {
                current_token.type = UNKNOWN;
                return current_token;
            }
            if(**expr == '.') dot_found = true;
            buffer[idx] = **expr;
            (*expr)++;
            idx += 1;
        }
        current_token.type = OPERAND;
        current_token.value = strtof(buffer,NULL);
        // printf("\t BUFFER: %s \n", buffer,NULL);
        // printf("\t STRTOF: %f \n", strtof(buffer,NULL));
        return current_token;
    }
    else if(**expr == '(') current_token.type = LB;
    else if(**expr == ')') current_token.type = RB;
    else if(is_operation(**expr)) current_token.type = OPERATOR;
    else if (**expr == ' ') current_token.type = SPACE;
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

/**
 * @brief Get the result of expresion expr
 * note:    it supports only (op E E), where 
 *          op -> operation
 *          E -> expresion
 * @param expr 
 * @param err_msg_buffer 
 * @return float 
 */
float get_result(char * expr, char * err_msg_buffer[ERR_MSG_LENGTH])
{
    *err_msg_buffer[0] = '\0';
    if(is_number(expr)) return strtof(expr,NULL);
    token_t current_token;

    while (*expr != '\0')
    {
        current_token = get_token(&expr); 
    }
    return 0;
}
