#pragma once

#include <QMainWindow>
#include <functional>
#include <optional>
#include <string>
#include "ui_mainwindow.h"
#include "enums.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    // Методы отображения
    void SetInputText(const std::string& text);
    void SetErrorText(const std::string& text);
    void SetFormulaText(const std::string& text);
    void SetMemText(const std::string& text);
    void SetExtraKey(const std::optional<std::string>& key);

    // Callback-сеттеры
    void SetDigitKeyCallback(std::function<void(int)> cb) { digit_cb_ = cb; }
    void SetProcessOperationKeyCallback(std::function<void(Operation)> cb) { operation_cb_ = cb; }
    void SetProcessControlKeyCallback(std::function<void(ControlKey)> cb) { control_cb_ = cb; }
    void SetControllerCallback(std::function<void(ControllerType)> cb) { controller_cb_ = cb; }

private slots:
    void OnDigitButtonClicked();
    void OnOperationButtonClicked();
    void OnControlButtonClicked();
    void OnControllerChanged(int index);

private:
    Ui::MainWindow* ui;
    std::function<void(int)> digit_cb_;
    std::function<void(Operation)> operation_cb_;
    std::function<void(ControlKey)> control_cb_;
    std::function<void(ControllerType)> controller_cb_;
};