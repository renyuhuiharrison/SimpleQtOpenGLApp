
#include "OpenGLWidget.h"

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>

OpenGLWidget::OpenGLWidget(QWidget*parent)
    : QOpenGLWidget(parent)
{
}

OpenGLWidget::~OpenGLWidget()
{
}



void OpenGLWidget::initializeGL()
{
    //Ϊ��ǰ�����ĳ�ʼ���ṩOpenGL��������
    this->initializeOpenGLFunctions();    
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
}

void OpenGLWidget::paintGL()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

