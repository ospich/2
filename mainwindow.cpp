#include "mainwindow.h"
#include <QPushButton>
#include <QComboBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      digit_cb_(nullptr),
      operation_cb_(nullptr),
      control_cb_(nullptr),
      controller_cb_(nullptr) 
{
    ui->setupUi(this);

    // Инициализация комбобокса
    ui->cmb_controller->addItems({"double", "float", "uint8_t", "int", "int64_t", "size_t", "Rational"});
    connect(ui->cmb_controller, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::OnControllerChanged);

    // Подключение цифровых кнопок (0-9)
    for (int i = 0; i <= 9; ++i) {
        auto btn = findChild<QPushButton*>(QString("tb_%1").arg(i));
        if (btn) connect(btn, &QPushButton::clicked, this, &MainWindow::OnDigitButtonClicked);
    }

    // Подключение кнопок операций
    const std::vector<std::pair<QString, Operation>> ops = {
        {"tb_add", Operation::ADDITION},
        {"tb_sub", Operation::SUBTRACTION},
        {"tb_mul", Operation::MULTIPLICATION},
        {"tb_div", Operation::DIVISION},
        {"tb_pow", Operation::POWER}
    };
    for (auto& [name, op] : ops) {
        if (auto btn = findChild<QPushButton*>(name)) {
            btn->setProperty("op", QVariant::fromValue(static_cast<int>(op)));
            connect(btn, &QPushButton::clicked, this, &MainWindow::OnOperationButtonClicked);
        }
    }

    // Подключение управляющих кнопок
    const std::vector<std::pair<QString, ControlKey>> ctrls = {
        {"tb_eq", ControlKey::EQUALS},
        {"tb_clr", ControlKey::CLEAR},
        {"tb_mem_save", ControlKey::MEM_SAVE},
        {"tb_mem_recall", ControlKey::MEM_LOAD},
        {"tb_mem_clear", ControlKey::MEM_CLEAR},
        {"tb_extra", ControlKey::EXTRA_KEY},
        {"tb_sign", ControlKey::PLUS_MINUS},
        {"tb_backspace", ControlKey::BACKSPACE}
    };
    for (auto& [name, ck] : ctrls) {
        if (auto btn = findChild<QPushButton*>(name)) {
            btn->setProperty("ck", QVariant::fromValue(static_cast<int>(ck)));
            connect(btn, &QPushButton::clicked, this, &MainWindow::OnControlButtonClicked);
        }
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

// Методы отображения
void MainWindow::SetInputText(const std::string& text) {
    ui->l_result->setStyleSheet("");
    ui->l_result->setText(QString::fromStdString(text));
}

void MainWindow::SetErrorText(const std::string& text) {
    ui->l_result->setStyleSheet("color: red;");
    ui->l_result->setText(QString::fromStdString(text));
}

void MainWindow::SetFormulaText(const std::string& text) {
    ui->l_formula->setText(QString::fromStdString(text));
}

void MainWindow::SetMemText(const std::string& text) {
    ui->l_memory->setText(QString::fromStdString(text));
}

void MainWindow::SetExtraKey(const std::optional<std::string>& key) {
    if (key) {
        ui->tb_extra->setText(QString::fromStdString(*key));
        ui->tb_extra->show();
    } else {
        ui->tb_extra->hide();
    }
}

// Слоты
void MainWindow::OnDigitButtonClicked() {
    auto btn = qobject_cast<QPushButton*>(sender());
    int key = btn->text().toInt();
    if (digit_cb_) digit_cb_(key);
}

void MainWindow::OnOperationButtonClicked() {
    auto btn = qobject_cast<QPushButton*>(sender());
    Operation op = static_cast<Operation>(btn->property("op").toInt());
    if (operation_cb_) operation_cb_(op);
}

void MainWindow::OnControlButtonClicked() {
    auto btn = qobject_cast<QPushButton*>(sender());
    ControlKey ck = static_cast<ControlKey>(btn->property("ck").toInt());
    if (control_cb_) control_cb_(ck);
}

void MainWindow::OnControllerChanged(int index) {
    Q_UNUSED(index);
    QString text = ui->cmb_controller->currentText();
    ControllerType ct = ControllerType::DOUBLE;
    if (text == "double") ct = ControllerType::DOUBLE;
    else if (text == "float") ct = ControllerType::FLOAT;
    else if (text == "uint8_t") ct = ControllerType::UINT8_T;
    else if (text == "int") ct = ControllerType::INT;
    else if (text == "int64_t") ct = ControllerType::INT64_T;
    else if (text == "size_t") ct = ControllerType::SIZE_T;
    else if (text == "Rational") ct = ControllerType::RATIONAL;
    if (controller_cb_) controller_cb_(ct);
}