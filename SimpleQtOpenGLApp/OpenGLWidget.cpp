#include "stdafx.h"

//Qt
#include <QOpenGLShaderProgram>

//local
#include "OpenGLWidget.h"
#include "Vertex.h"
#include "Camera.h"

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
	m_width = 0;
	m_height = 0;

	m_camera = new Camera(QVector3D(0.0f, 0.0f, 3.0f));
	m_deltaTime = 1.0f;
	m_lastX = width() / 2.0f;
	m_lastY = height() / 2.0f;
	m_bFirstMouse = true;

	m_btnPressStatus = OpenGLWidget::ButtonNone;

	setFocusPolicy(Qt::StrongFocus);//用于响应键盘事件
}

OpenGLWidget::~OpenGLWidget()
{
	makeCurrent();

	m_vbo.destroy();
	m_vao.destroy();
	
	delete m_camera;

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
	m_width = w;
	m_height = h;
	
	m_shaderProgram.bind();
	{
		QMatrix4x4 projMatrix;
		projMatrix.perspective(m_camera->getZoom(), 1.0f * m_width / m_height, m_camera->getNear(), m_camera->getFar());
		m_shaderProgram.setUniformValue("projMatrix", projMatrix);
	}
	m_shaderProgram.release();

    glViewport(0.0f, 0.0f, w, h);
}

void OpenGLWidget::paintGL()
{
	m_shaderProgram.bind();
	{
		QMatrix4x4 modelMatrix;
		modelMatrix.setToIdentity();
		m_shaderProgram.setUniformValue("modelMatrix", modelMatrix);

		QMatrix4x4 viewMatrix = m_camera->GetViewMatrix();
		m_shaderProgram.setUniformValue("viewMatrix", viewMatrix);

		QMatrix4x4 projMatrix;
		projMatrix.perspective(m_camera->getZoom(), 1.0f * m_width / m_height, m_camera->getNear(), m_camera->getFar());
		m_shaderProgram.setUniformValue("projMatrix", projMatrix);

		QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]));
	}
	m_shaderProgram.release();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
	switch (event->key()) {
	case Qt::Key_W:
	{
		m_camera->move(FORWARD, m_deltaTime);
		break;
	}
	case Qt::Key_S:
	{
		m_camera->move(BACKWARD, m_deltaTime);
		break;
	}
	case Qt::Key_D:
	{
		m_camera->move(LEFT, m_deltaTime);
		break;
	}
	case Qt::Key_A:
	{
		m_camera->move(RIGHT, m_deltaTime);
		break;
	}
	case Qt::Key_Up:
	{
		m_camera->move(UP, m_deltaTime);
		break;
	}
	case Qt::Key_Down:
	{
		m_camera->move(DOWN, m_deltaTime);
		break;
	}
	default:
	{
		break;
	}
	}
	update();
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
	Q_UNUSED(event);
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button())
	{
		m_lastX = event->pos().x();
		m_lastY = event->pos().y();

		m_btnPressStatus = ButtonLeft;

		update();
	}
	else if (Qt::RightButton == event->button())
	{
		m_lastX = event->pos().x();
		m_lastY = event->pos().y();

		m_btnPressStatus = ButtonRight;

		update();
	}
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (ButtonLeft == m_btnPressStatus)
	{
		int xpos = event->pos().x();
		int ypos = event->pos().y();

		if (m_bFirstMouse)
		{
			m_lastX = xpos;
			m_lastY = ypos;
			m_bFirstMouse = false;
			return;
		}

		int xoffset = m_lastX - xpos;
		int yoffset = ypos - m_lastY;// reversed since y-coordinates go from bottom to top

		m_lastX = xpos;
		m_lastY = ypos;

		m_camera->rotate(xoffset, yoffset);
		
		update();
	}
	else if (ButtonRight == m_btnPressStatus)
	{

	}
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
	QPoint offset = event->angleDelta();
	if (offset.y() > 0)
	{
		m_camera->move(FORWARD, m_deltaTime);
	}
	else
	{
		m_camera->move(BACKWARD, m_deltaTime);
	}
	update();
}

