#include "../headers/fraction.h"

void print_frac(frac_t f)
{
    if(f.denominator == 1) fprintf(stdout,"%d",f.numerator);
    else if(f.denominator == 0) fprintf(stdout,"ERR");
    else fprintf(stdout,"%d/%d",f.numerator,f.denominator);
    return;
}

int find_gcd(int num1, int num2)
{
    if(num1 == 0 || num2 == 0) return 1;
    
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

frac_t num_to_frac(int num)
{
    frac_t result;
    result.numerator = num;
    result.denominator = 1;
    return result;
}

void to_basic_shape(frac_t * f)
{
    int gcd = find_gcd(f->numerator,f->denominator);
    (*f).numerator /= gcd;
    (*f).denominator /= gcd;
}

frac_t inverted_fraction(frac_t f)
{
    frac_t inverted = {.numerator = f.denominator, .denominator = f.numerator};
    return inverted;
}

void num_to_string(int num,char * str)
{
    int length = 0;
    int rem, n;

    if(num == 0)
    {
        strcpy(str,"0");
        return;
    }
    n = num;
    while (n != 0)
    {
        length++;
        n /= 10;
    }

    for (int idx = 0; idx < length; idx++)
    {
        rem = num % 10;
        num = num / 10;
        str[length - (idx + 1)] = rem + '0';
    }

    str[length] = '\0';
    
}

void frac_to_string(frac_t f, char * str)
{
    char numerator[MAX_NUM_LENGTH];
    char denominator[MAX_NUM_LENGTH];
    if(f.denominator == 0) strcpy(str,"0/0");
    else if(f.denominator == 1) 
    {
        num_to_string(f.numerator,numerator);
        strcpy(str,numerator);
    }
    else
    {
        num_to_string(f.numerator,numerator);
        num_to_string(f.denominator,denominator);
        char whole_frac[MAX_NUM_LENGTH];
        strcat(whole_frac,numerator);
        strcat(whole_frac,"/");
        strcat(whole_frac,denominator);
        strcat(str,whole_frac);
    }
}

void frac_to_string_floored(frac_t f,char * str)
{
    if(f.denominator == 0) return;
    int floored_result = f.numerator / f.denominator;
    char floored_result_string[MAX_NUM_LENGTH] = {0,};
    num_to_string(floored_result,floored_result_string);
    strcpy(str,floored_result_string);
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
    result.numerator = op1.numerator * op2.numerator;
    result.denominator = op1.denominator * op2.denominator;
    to_basic_shape(&result);
    return result;
}

frac_t frac_div(frac_t op1,frac_t op2)
{   
    return frac_mul(op1,inverted_fraction(op2));
}
