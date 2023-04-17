#ifndef CALC_STACK_H
#define CALC_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"
#include "tcp_module.h"

#define MAX_STACK_SIZE TCP_LIMIT/2
#define EMPTY -1

typedef struct calc_stack {
    frac_t data[MAX_STACK_SIZE];
    int top;
}calc_stack_t;

void init_calc_stack(calc_stack_t *stack);
bool calcs_push(calc_stack_t * stack,frac_t value);
frac_t calcs_pop(calc_stack_t *stack);
void calcs_dispose(calc_stack_t *stack);
bool is_empty(calc_stack_t *stack);
calc_stack_t peek(calc_stack_t *stack);
void calcs_reverse(calc_stack_t *stack);
void calcs_print(calc_stack_t *stack);
int size(calc_stack_t * stack);
#endif
