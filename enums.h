#pragma once

enum class Operation {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    POWER
};

enum class ControlKey {
    EQUALS,     // Кнопка "Равно"
    CLEAR,      // Кнопка "C"
    MEM_SAVE,   // Кнопка "MS"
    MEM_LOAD,   // Кнопка "ML"
    MEM_CLEAR,  // Кнопка "MC"
    PLUS_MINUS, // Кнопка "+-"
    BACKSPACE,  // Кнопка "⌫"
    EXTRA_KEY   // Дополнительная кнопка
};

enum class ControllerType {
    UINT8_T,
    INT,
    INT64_T,
    SIZE_T,
    DOUBLE,
    FLOAT,
    RATIONAL
};