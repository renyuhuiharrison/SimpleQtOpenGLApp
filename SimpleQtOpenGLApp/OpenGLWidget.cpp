#include "stdafx.h"

//Qt
#include <QOpenGLShaderProgram>

//local
#include "OpenGLWidget.h"



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
    
    m_shaderProgram = new QOpenGLShaderProgram(QOpenGLContext::currentContext());
	bool bSuccess = m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "vertexShader.glsl");
	if (!bSuccess) {
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_shaderProgram->log();
		return;
	}

	bSuccess = m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "fragmentShader.glsl");
	if (!bSuccess) {
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_shaderProgram->log();
		return;
	}

	m_shaderProgram->link();
    m_shaderProgram->bind();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
}

void OpenGLWidget::paintGL()
{
	QOpenGLFunctions* funs = QOpenGLContext::currentContext()->functions();

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

	static GLfloat const Vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
	};

	int vertexLocation = m_shaderProgram->attributeLocation("aPos");

	m_shaderProgram->enableAttributeArray(vertexLocation);
	m_shaderProgram->setAttributeArray(vertexLocation, Vertices, 3);
	funs->glDrawArrays(GL_TRIANGLES, 0, 3);
	m_shaderProgram->disableAttributeArray(vertexLocation);
}

