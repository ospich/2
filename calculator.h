#pragma once

#include <optional>
#include <string>
#include <type_traits>
#include <cmath>
#include "pow.h"
#include "rational.h"

using Error = std::string;

template<typename Number>
class Calculator {
public:
    Calculator() : number_(Number{}), mem_(std::nullopt) {}

    void Set(const Number& value) { number_ = value; }
    Number GetNumber() const { return number_; }

    void Save() { mem_ = number_; }
    void Load() { if (mem_) number_ = *mem_; }
    void ClearMem() { mem_.reset(); }
    bool GetHasMem() const { return mem_.has_value(); }

    std::optional<Error> Add(const Number& b) {
        number_ = number_ + b;
        return std::nullopt;
    }

    std::optional<Error> Sub(const Number& b) {
        number_ = number_ - b;
        return std::nullopt;
    }

    std::optional<Error> Mul(const Number& b) {
        number_ = number_ * b;
        return std::nullopt;
    }

    std::optional<Error> Div(const Number& b) {
        if constexpr (std::is_integral_v<Number> || std::is_same_v<Number, Rational>) {
            if (b == Number{0}) return "Division by zero";
        }
        number_ = number_ / b;
        return std::nullopt;
    }

    std::optional<Error> Pow(const Number& exp) {
        if (number_ == Number{0} && exp == Number{0}) return "Zero power to zero";

        if constexpr (std::is_floating_point_v<Number>) {
            number_ = std::pow(number_, exp);
        } else if constexpr (std::is_integral_v<Number>) {
            if (exp < 0) return "Integer negative power";
            number_ = IntegerPow(number_, static_cast<int>(exp));
        } else if constexpr (std::is_same_v<Number, Rational>) {
            if (exp.GetDenominator() != 1) return "Fractional power is not supported";
            number_ = ::Pow(number_, exp);
        }
        return std::nullopt;
    }

private:
    Number number_;
    std::optional<Number> mem_;
};