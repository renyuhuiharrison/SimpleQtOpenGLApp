#include "stdafx.h"

#include "SimpleQtOpenGLApp.h"

SimpleQtOpenGLApp::SimpleQtOpenGLApp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	initActions();
}

void SimpleQtOpenGLApp::initActions()
{
	connect(ui.actionDisplayTriangle, &QAction::triggered, [=]()
		{
			ui.openglWidget->displayTriangle();
		}
	);
}
