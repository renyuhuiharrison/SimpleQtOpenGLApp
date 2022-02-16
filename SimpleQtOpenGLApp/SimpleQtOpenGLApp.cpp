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

	QString defaultDir("D:/models/Obj_models");

	//导入模型
	connect(ui.actionImportModel, &QAction::triggered, [=]()
		{
			QString fileName = QFileDialog::getOpenFileName(this, 
				"open file", 
				defaultDir,
				("obj(*.obj);;Allfile(*.*)"));

			QFileInfo fileInfo(fileName);
			if (!fileInfo.exists()){
				return;
			}
			ui.openglWidget->clearScene();
			ui.openglWidget->importModel(fileName);
		}
	);

	//清空场景
	connect(ui.actionClearScene, &QAction::triggered, [=]()
		{
			ui.openglWidget->clearScene();
		}
	);
}
