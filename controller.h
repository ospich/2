#pragma once

#include <string>
#include <sstream>
#include <functional>
#include <optional>
#include <iomanip>
#include "calculator.h"
#include "mainwindow.h"
#include "rational.h"

template<class T>
std::string ToString(T number) {
    std::ostringstream tmp_str;
    tmp_str << number;
    return tmp_str.str();
}

template<>
std::string ToString(std::uint8_t number) {
    return std::to_string(static_cast<int>(number));
}

template<>
std::string ToString(Rational number) {
    number = Rational(number.GetNumerator(), number.GetDenominator());
    if (number.GetDenominator() == 1) {
        return std::to_string(number.GetNumerator());
    }
    return std::to_string(number.GetNumerator()) + "/" + std::to_string(number.GetDenominator());
}

template<>
std::string ToString(double number) {
    std::ostringstream tmp_str;
    tmp_str << number;
    std::string s = tmp_str.str();
    size_t dot_pos = s.find('.');
    if (dot_pos != std::string::npos) {
        s = s.substr(0, s.find_last_not_of('0') + 1);
        if (s.back() == '.') s.pop_back();
    }
    return s;
}

template<class T>
T FromString(const std::string& number) {
    std::istringstream tmp_str{number};
    T result{};
    tmp_str >> result;
    return result;
}

template<>
std::uint8_t FromString<std::uint8_t>(const std::string& number) {
    return static_cast<std::uint8_t>(std::stoi(number));
}

template<class Number>
class Controller {
public:
    Controller() = default;

    void BindWithMainWindow(MainWindow* main_window) {
        view_ = main_window;
        if (!view_) return;

        view_->SetDigitKeyCallback([this](int key) { PressDigitKey(key); });
        view_->SetProcessOperationKeyCallback([this](Operation key) { ProcessOperationKey(key); });
        view_->SetProcessControlKeyCallback([this](ControlKey key) { ProcessControlKey(key); });

        RevealView();
    }

private:
    void PressDigitKey(int digit) {
        AddChar(static_cast<char>(digit + '0'));
    }

    void ProcessOperationKey(Operation operation) {
        switch (operation) {
            case Operation::ADDITION:
                OnOperation([this](Number x) { return calculator_.Add(x); }, " + ");
                break;
            case Operation::SUBTRACTION:
                OnOperation([this](Number x) { return calculator_.Sub(x); }, " − ");
                break;
            case Operation::MULTIPLICATION:
                OnOperation([this](Number x) { return calculator_.Mul(x); }, " × ");
                break;
            case Operation::DIVISION:
                OnOperation([this](Number x) { return calculator_.Div(x); }, " ÷ ");
                break;
            case Operation::POWER:
                OnOperation([this](Number x) { return calculator_.Pow(x); }, " ^ ");
                break;
        }
    }

    void ProcessControlKey(ControlKey key) {
        switch (key) {
            case ControlKey::EQUALS: {
                if (!operation_ || !input_as_number_) return;

                std::string formula = ToString(calculator_.GetNumber()) + operation_name_ + ToString(active_number_) + " = ";
                auto error = operation_(active_number_);

                if (error) {
                    SetErrorInView(*error);
                    break;
                }

                SetFormulaInView(formula);
                SetInputAsNumber(calculator_.GetNumber());
                operation_ = {};
                break;
            }
            case ControlKey::CLEAR:
                SetInputAsNumber(Number{});
                SetFormulaInView("");
                operation_ = {};
                break;
            case ControlKey::MEM_SAVE:
                mem_ = active_number_;
                SetMemInView("M");
                break;
            case ControlKey::MEM_LOAD:
                if (mem_) SetInputAsNumber(*mem_);
                break;
            case ControlKey::MEM_CLEAR:
                mem_ = std::nullopt;
                SetMemInView("");
                break;
            case ControlKey::PLUS_MINUS:
                active_number_ = -active_number_;
                SetInputAsNumber(active_number_);
                break;
            case ControlKey::BACKSPACE:
                if (!input_.empty()) SetInputAsString(input_.substr(0, input_.size() - 1));
                break;
            case ControlKey::EXTRA_KEY:
                DoExtraAction();
                break;
        }
    }

    void OnOperation(std::function<std::optional<Error>(Number)> action, std::string text) {
        calculator_.Set(active_number_);
        operation_name_ = text;
        operation_ = action;
        input_.clear();
        SetFormulaInView(ToString(calculator_.GetNumber()) + text);
    }

    void SetInputAsString(const std::string& new_input) {
        input_as_number_ = false;
        input_ = new_input;
        active_number_ = FromString<Number>(input_);
        UpdateInputInView(new_input);
    }

    void SetInputAsNumber(Number number) {
        input_as_number_ = true;
        input_.clear();
        active_number_ = number;
        UpdateInputInView(ToString(active_number_));
    }

    void AddChar(char character) {
        SetInputAsString(input_ + character);
    }

    void DoExtraAction() {
        if constexpr (std::is_same_v<Number, Rational>) {
            if (input_.find('/') == std::string::npos) AddChar('/');
        } else if constexpr (std::is_floating_point_v<Number>) {
            if (input_.find('.') == std::string::npos) AddChar('.');
        }
    }

    void UpdateInputInView(const std::string& string) {
        if (view_) view_->SetInputText(string);
        text_ = string;
    }

    void SetErrorInView(const std::string& error) {
        if (view_) view_->SetErrorText(error);
    }

    void SetFormulaInView(const std::string& formula) {
        if (view_) view_->SetFormulaText(formula);
        formula_ = formula;
    }

    void SetMemInView(const std::string& mem) {
        if (view_) view_->SetMemText(mem);
        mem_text_ = mem;
    }

    void RevealView() {
        if (!view_) return;
        view_->SetInputText(text_);
        view_->SetFormulaText(formula_);
        view_->SetMemText(mem_text_);
        view_->SetExtraKey(GetExtraKey());
    }

    std::optional<std::string> GetExtraKey() const {
        if constexpr (std::is_same_v<Number, Rational>) return "/";
        else if constexpr (std::is_floating_point_v<Number>) return ".";
        else return std::nullopt;
    }

private:
    std::function<std::optional<Error>(Number)> operation_;
    std::string operation_name_;
    Calculator<Number> calculator_;
    Number active_number_{};
    std::string input_;
    std::optional<Number> mem_;
    bool input_as_number_ = true;
    MainWindow* view_ = nullptr;
    std::string text_;
    std::string formula_;
    std::string mem_text_;
};