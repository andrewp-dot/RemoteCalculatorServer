#include "../headers/fraction.h"
#include <stdio.h>
#include <stdlib.h>

void print_frac(frac_t f)
{
    fprintf(stdout,"%d/%d",f.numerator,f.denominator);
}

int find_gcd(int num1, int num2)
{
    num1 = ( num1 > 0) ? num1 : -num1;
    num2 = ( num2 > 0) ? num2 : -num2;

    while(num1!=num2)
    {
        if(num1 > num2)
            num1 -= num2;
        else
            num2 -= num1;
    }

    return num1;
}

int find_lcm(int num1, int num2,int gcd)
{
    return (num1 * num2) / gcd;
}

void to_same_dominator(frac_t * op1, frac_t *op2)
{
    const int op1_original_denominator =  op1->denominator;
    (*op1).numerator *= op2->denominator;
    (*op1).denominator *= op2->denominator;

    (*op2).numerator *= op1_original_denominator;
    (*op2).denominator *= op1_original_denominator;
}

void to_basic_shape(frac_t * f)
{
    int gcd = find_gcd(f->numerator,f->denominator);
    (*f).numerator /= gcd;
    (*f).denominator /= gcd;
}

void invert_fraction(frac_t * f)
{
    int temp = f->numerator;
    (*f).numerator = (*f).denominator;
    (*f).denominator = temp;
}

frac_t frac_add(frac_t op1,frac_t op2)
{
    frac_t result;
    to_same_dominator(&op1, &op2);
    result.denominator = op1.denominator;
    result.numerator = op1.numerator + op2.numerator;
    to_basic_shape(&result);
    return result;
}

frac_t frac_sub(frac_t op1,frac_t op2)
{
    frac_t result;
    to_same_dominator(&op1, &op2);
    result.denominator = op1.denominator;
    result.numerator = op1.numerator - op2.numerator;
    to_basic_shape(&result);
    return result;
}

frac_t frac_mul(frac_t op1,frac_t op2)
{
    frac_t result;
    result.numerator = op1.numerator * op2.denominator;
    result.denominator = op1.denominator * op2.denominator;
    to_basic_shape(&result);
    return result;
}

frac_t frac_div(frac_t op1,frac_t op2)
{
    invert_fraction(&op2);
    return frac_mul(op1,op2);
}
