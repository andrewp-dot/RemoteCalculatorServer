#ifndef FRAC_H
#define FRAC_H

typedef struct fraction{
    int numerator;
    int denominator;
}frac_t;

/* auxiliary functions */
void print_frac(frac_t f);
int find_gcd(int num1, int num2);
int find_lcm(int num1, int num2,int gcd);
void to_same_dominator(frac_t * op1, frac_t *op2);

/* Cosmetic changes */
void to_basic_shape(frac_t * f);
void invert_fraction(frac_t * f);

/* Operations with fractions*/
frac_t frac_add(frac_t op1,frac_t op2);
frac_t frac_sub(frac_t op1,frac_t op2);
frac_t frac_mul(frac_t op1,frac_t op2);
frac_t frac_div(frac_t op1,frac_t op2);

#endif