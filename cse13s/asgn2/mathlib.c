#include "mathlib.h"

#include <math.h>

#define EPSILON 1e-14

//
// Code from Eugene's 1/19/2021 section
// Returns the absolute value of x
//
// x: A double to input into the absolute value. |x|
//
static inline double Abs(double x) {
    return x < 0 ? -x : x;
}

//
// Returns the modulus of two doubles.
//
// num: A double that goes into the division numerator
// den: A dobule that goes into the division denominator
//
double Fmod(double num, double den) {
    return num - den * (double) ((int) (num / den));
}

//
// Inspired by Eugene's 1/19/2021 section
// Returns the sin(x)
//
// x: A doubles to input into the sine function
//
double Sin(double x) {
    x = Fmod(x, 2 * M_PI);
    double numerator = x;
    double denominator = 1.0;
    double term = numerator / denominator;
    double sum = term;
    for (double k = 3.0; Abs(term) > EPSILON; k += 2.0) {
        numerator *= x * x * -1;
        denominator *= (k - 1) * k;
        term = numerator / denominator;
        sum += term;
    }
    return sum;
}

//
// Returns cos(x)
//
// x: A double to input into the cosine function
//
double Cos(double x) {
    x = Fmod(x, 2 * M_PI);
    double numerator = 1.0;
    double denominator = 1.0;
    double term = numerator / denominator;
    double sum = term;
    for (double k = 2.0; Abs(term) > EPSILON; k += 2.0) {
        numerator *= x * x * -1;
        denominator *= (k - 1) * k;
        term = numerator / denominator;
        sum += term;
    }
    return sum;
}

//
// Returns tan(x)
//
// x: A double to input into the tangent function
//
double Tan(double x) {
    return Sin(x) / Cos(x);
}

//
// Returns e^x
//
// x: A double to input into the exponent function
//
double Exp(double x) {
    double curr = 1.0, new = 1.0, sum = 1.0;
    for (double k = 1.0; Abs(new) > EPSILON; k += 1) {
        curr = x / k;
        new = new *curr;
        sum += new;
    }
    return sum;
}

//
// Code from 1/15/2021 lecture slides
// Returns log(x)
//
// x: A double to input into the logarithmic function
//
double Log(double x) {
    double y = 1.0;
    double p = Exp(y);
    while (Abs(p - x) > EPSILON) {
        y += (x - p) / p;
        p = Exp(y);
    }
    return y;
}
