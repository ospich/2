#include "mainwindow.h"
#include "controller.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;

    // Контроллеры для всех типов
    Controller<double> double_controller;
    Controller<float> float_controller;
    Controller<uint8_t> byte_controller;
    Controller<int> int_controller;
    Controller<int64_t> int64_controller;
    Controller<size_t> size_t_controller;
    Controller<Rational> rational_controller;

    // Обработчик смены типа
    w.SetControllerCallback([&](ControllerType type) {
        switch (type) {
            case ControllerType::DOUBLE: double_controller.BindWithMainWindow(&w); break;
            case ControllerType::FLOAT: float_controller.BindWithMainWindow(&w); break;
            case ControllerType::UINT8_T: byte_controller.BindWithMainWindow(&w); break;
            case ControllerType::INT: int_controller.BindWithMainWindow(&w); break;
            case ControllerType::INT64_T: int64_controller.BindWithMainWindow(&w); break;
            case ControllerType::SIZE_T: size_t_controller.BindWithMainWindow(&w); break;
            case ControllerType::RATIONAL: rational_controller.BindWithMainWindow(&w); break;
        }
    });

    // По умолчанию: double
    double_controller.BindWithMainWindow(&w);
    w.show();
    return a.exec();
}