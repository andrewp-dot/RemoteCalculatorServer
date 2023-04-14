#ifndef BIN_CALC_H
#define BIN_CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fraction.h"

#define ERR_MSG_LENGTH 128
#define MAX_FLOAT_LENGTH 100

#define ERR -10
typedef enum token_type{
    LB,
    RB,
    OPERATOR,
    OPERAND,
    SPACE,
    UNKNOWN,
    NEWLINE
}token_type_t;

typedef struct token{
    token_type_t type;
    int value;
    char sym;
}token_t;

typedef enum operation{
    ADD,
    SUB,
    MUL,
    DIV,
    NOT_SUPPORTED
}operation_t;

bool is_number(char * num);
bool is_operation(char op);
operation_t get_operation(char sym);
token_t get_token(char ** expr);
frac_t compute(frac_t op1,frac_t op2,char operator);
frac_t get_result(char ** expr);
#endif
