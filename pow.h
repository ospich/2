#pragma once

#include "rational.h"

template<class Number>
Number IntegerPow(Number base, int exponent) {
    Number result = 1;
    bool negative = exponent < 0;
    exponent = std::abs(exponent);
    
    while (exponent > 0) {
        if (exponent & 1) result *= base;
        exponent >>= 1;
        base *= base;
    }
    
    return negative ? 1 / result : result;
}

inline Rational Pow(Rational lhs, Rational rhs) {
    if (rhs.GetDenominator() != 1) std::abort();
    int exponent = rhs.GetNumerator();
    return (exponent >= 0)
        ? Rational(IntegerPow(lhs.GetNumerator(), exponent), IntegerPow(lhs.GetDenominator(), exponent))
        : Rational(IntegerPow(lhs.GetDenominator(), -exponent), IntegerPow(lhs.GetNumerator(), -exponent));
}