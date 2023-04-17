#include "../headers/calc_stack.h"

void print_type_in_stack(token_type_t type)
{
    if(type == LB) printf("LB:\n");
    else if(type == RB) printf("RB:\n");
    else if(type == OPERATOR) printf("OPERATOR:\n");
    else if(type == OPERAND) printf("OPERAND:\n");
    else if(type == SPACE) printf("SPACE:\n");
    else if(type == UNKNOWN) printf("UNKNOWN:\n");
    else if(type == NEWLINE) printf("NEWLINE:\n");
    else printf("ERR:\n");
}

void init_calc_stack(calc_stack_t * stack)
{
    for (int i = 0; i < MAX_STACK_SIZE; i++)
    {
        stack->data[i] = ERR_FRAC;
    }
    stack->top = EMPTY;
}

bool calcs_push(calc_stack_t * stack,frac_t value)
{
    if(stack->top == MAX_STACK_SIZE) return false;

    int next = stack->top + 1;
    stack->data[next] = value;
    stack->top += 1;

    return true;
}

frac_t calcs_pop(calc_stack_t * stack)
{
    if(is_empty(stack)) return ERR_FRAC;
    frac_t res = stack->data[stack->top];
    //remove
    stack->data[stack->top] = ERR_FRAC;
    //decrement top
    stack->top -= 1;
    return res;
}

void calcs_dispose(calc_stack_t *stack)
{
    for (int i = stack->top; i > EMPTY; i--)
    {
        stack->data[i] = ERR_FRAC;   
    }
    stack->top = EMPTY;
}

bool is_empty(calc_stack_t *stack)
{
    return stack->top == EMPTY;
}

frac_t top(calc_stack_t *stack)
{
    return stack->data[stack->top];
}

void calcs_reverse(calc_stack_t *stack)
{
    int N = stack->top + 1;
    int limit = N/2;
    frac_t tmp;
    for(int idx = 0; idx < limit; idx++)
    {
        tmp.numerator = stack->data[idx].numerator;
        tmp.denominator = stack->data[idx].denominator;

        stack->data[idx].numerator = stack->data[N-idx-1].numerator;
        stack->data[idx].denominator = stack->data[N-idx-1].denominator;

        stack->data[N-idx-1].numerator = tmp.numerator;
        stack->data[N-idx-1].denominator = tmp.denominator;
    }
}

void calcs_print(calc_stack_t *stack)
{
    printf("\n____CALC_STACK____\n");
    for (int i = stack->top; i > EMPTY; i--)
    {
        printf("STACK: ");
        print_frac(stack->data[i]);
        printf("\n");
        
    }
    printf("____CALC_STACK_END__\n");
}

int size(calc_stack_t * stack)
{
    return stack->top + 1;
}

