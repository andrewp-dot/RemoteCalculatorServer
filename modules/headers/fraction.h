#ifndef FRAC_H
#define FRAC_H

typedef struct fraction{
    int numerator;
    int denominator;
}frac_t;

#define ERR_FRAC (frac_t){.numerator = 0,.denominator = 0};

/* auxiliary functions */
void print_frac(frac_t f);
int find_gcd(int num1, int num2);
int find_lcm(int num1, int num2,int gcd);
void to_same_dominator(frac_t * op1, frac_t *op2);
frac_t num_to_frac(int num);

/* Cosmetic changes */
void to_basic_shape(frac_t * f);
frac_t inverted_fraction(frac_t f);
int convert_to_number(frac_t f);

/* Operations with fractions*/
frac_t frac_add(frac_t op1,frac_t op2);
frac_t frac_sub(frac_t op1,frac_t op2);
frac_t frac_mul(frac_t op1,frac_t op2);
frac_t frac_div(frac_t op1,frac_t op2);

#endif