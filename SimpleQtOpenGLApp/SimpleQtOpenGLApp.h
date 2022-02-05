#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SimpleQtOpenGLApp.h"

class SimpleQtOpenGLApp : public QMainWindow
{
    Q_OBJECT

public:
    SimpleQtOpenGLApp(QWidget *parent = Q_NULLPTR);

private:
    Ui::SimpleQtOpenGLAppClass ui;
};
