
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
    //为当前上下文初始化提供OpenGL函数解析
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

