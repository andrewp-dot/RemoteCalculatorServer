#ifndef BIN_CALC_H
#define BIN_CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ERR_MSG_LENGTH 128
#define MAX_FLOAT_LENGTH 100
typedef enum token_type{
    LB,
    RB,
    OPERATOR,
    OPERAND,
    SPACE,
    UNKNOWN,
}token_type_t;

typedef struct token{
    token_type_t type;
    float value;
    char sym;
}token_t;

token_t get_token(char ** expr);
bool is_number(char * num);
bool is_operation(char op);
float get_result(char * expr,char * err_msg_buffer[ERR_MSG_LENGTH]);

#endif
