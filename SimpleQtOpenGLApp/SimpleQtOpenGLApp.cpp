#include "stdafx.h"

//Qt
#include <QFileDialog>
#include <QFileInfo>

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

	//����ģ��
	connect(ui.actionImportModel, &QAction::triggered, [=]()
		{
			QString fileName = QFileDialog::getOpenFileName(this, 
				"open file", 
				" ", 
				("obj(*.obj);;Allfile(*.*)"));

			QFileInfo fileInfo(fileName);
			if (!fileInfo.exists()){
				return;
			}

			ui.openglWidget->importModel(fileName);
		}
	);

	//��ճ���
	connect(ui.actionClearScene, &QAction::triggered, [=]()
		{
			ui.openglWidget->clearScene();
		}
	);
}
