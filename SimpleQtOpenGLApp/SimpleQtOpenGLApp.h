#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SimpleQtOpenGLApp.h"

class SimpleQtOpenGLApp : public QMainWindow
{
    Q_OBJECT

public:
    SimpleQtOpenGLApp(QWidget *parent = Q_NULLPTR);

private:
    void initActions();


private:
    Ui::SimpleQtOpenGLAppClass ui;
};
