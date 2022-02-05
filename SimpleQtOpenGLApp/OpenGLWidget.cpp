#include "stdafx.h"

//Qt
#include <QOpenGLShaderProgram>

//local
#include "OpenGLWidget.h"
#include "Vertex.h"


//创建一个彩色三角形
static const Vertex vertices[] = {
  Vertex(QVector3D(0.00f,  0.75f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)),
  Vertex(QVector3D(0.75f, -0.75f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)),
  Vertex(QVector3D(-0.75f, -0.75f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f))
};

OpenGLWidget::OpenGLWidget(QWidget*parent) : 
	QOpenGLWidget(parent),
	m_vbo(QOpenGLBuffer::VertexBuffer)
{
}

OpenGLWidget::~OpenGLWidget()
{
	makeCurrent();

	m_vbo.destroy();
	m_vao.destroy();

	doneCurrent();
}



void OpenGLWidget::initializeGL()
{
    //为当前上下文初始化提供OpenGL函数解析
    this->initializeOpenGLFunctions();    

	//设置清屏颜色
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	//清空颜色缓冲区
	glClear(GL_COLOR_BUFFER_BIT);

	m_vao.create();
	m_vao.bind();
	
	m_vbo.create();
	m_vbo.bind();

	bool bSuccess = m_shaderProgram.create();
	if (!bSuccess) {
		qDebug() << "shaderProgram create failed!" << m_shaderProgram.log();
		return;
	}

	bSuccess = m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "vertexShader.vert");
	if (!bSuccess) {
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_shaderProgram.log();
		return;
	}

	bSuccess = m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "fragmentShader.frag");
	if (!bSuccess) {
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_shaderProgram.log();
		return;
	}

	bSuccess = m_shaderProgram.link();
	if (!bSuccess) {
		qDebug() << "shaderProgram link failed!" << m_shaderProgram.log();
		return;
	}

	m_vbo.allocate(vertices, sizeof(vertices));

	m_shaderProgram.enableAttributeArray(0);
	m_shaderProgram.enableAttributeArray(1);

	GLuint vPosition = m_shaderProgram.attributeLocation("VertexPosition");
	m_shaderProgram.setAttributeBuffer(vPosition, GL_FLOAT, Vertex::getPositionOffset(), 3, Vertex::getStride());
	
	GLuint vColor = m_shaderProgram.attributeLocation("VertexColor");
	m_shaderProgram.setAttributeBuffer(vColor, GL_FLOAT, Vertex::getColorOffset(), 3, Vertex::getStride());

	m_vao.release();
	m_vbo.release(); 
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
}

void OpenGLWidget::paintGL()
{
	m_shaderProgram.bind();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]));
	m_shaderProgram.release();
}

