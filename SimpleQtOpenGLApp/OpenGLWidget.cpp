#include "stdafx.h"

//Qt
#include <QOpenGLShaderProgram>

//local
#include "OpenGLWidget.h"
#include "Vertex.h"
#include "Camera.h"
#include "Mesh.h"

//彩色三角形
static const Vertex tri_vertices[] = {
	Vertex(QVector3D(0.00f,  0.75f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)),
	Vertex(QVector3D(0.75f, -0.75f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)),
	Vertex(QVector3D(-0.75f, -0.75f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f))
};

static const Vertex tri_vertices_2[] = {
	Vertex(QVector3D(0.0f, 0.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f)),
	Vertex(QVector3D(0.0f, 1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f)),
	Vertex(QVector3D(1.0f, 1.0f, -1.0f), QVector3D(0.0f, 0.0f, 1.0f))
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
	m_bLoadMesh = false;

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



void OpenGLWidget::displayTriangle()
{
	m_bLoadMesh = true;

	m_shaderProgram.bind();

	Mesh* mesh = new Mesh();

	for (int i = 0; i < 3; i++)
	{
		mesh->addVertex(tri_vertices[i]);
	}

	m_vbo.bind();
	{
		m_vbo.allocate(mesh->getVertices().constData(), mesh->getVertices().length() * sizeof(Vertex));
	}
	m_vbo.release();

	m_meshes.push_back(mesh);

	m_shaderProgram.release();


#if 0
	m_vbo.bind();
	{
		m_vbo.allocate(tri_vertices, sizeof(tri_vertices));
	}
	m_vbo.release();
#endif
}

void OpenGLWidget::initializeGL()
{
    this->initializeOpenGLFunctions();    

	printContextInformation();

	if (!createShader()) {
		return;
	}

	//设置清屏颜色
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	//清空颜色缓冲区
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	m_vao.create();
	m_vao.bind();

	m_vbo.create();
	m_vbo.bind();

	m_vbo.allocate(nullptr, 0);

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
	if (!m_bLoadMesh)
		return;

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

#if 0
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_vbo_vertex);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tri_vertices) / sizeof(tri_vertices[0]), &tri_vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::getStride(), (void*)Vertex::getPositionOffset());
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::getStride(), (void*)Vertex::getColorOffset());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);


		glBindVertexArray(m_VAO);
		{
			glDrawArrays(GL_TRIANGLES, 0, sizeof(tri_vertices) / sizeof(tri_vertices[0]));
		}
		glBindVertexArray(0);
#endif

		for (int i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i]->draw(&m_shaderProgram);
		}
		 
		
	}
	m_shaderProgram.release();
}

bool OpenGLWidget::createShader()
{
	bool bSuccess = m_shaderProgram.create();
	if (!bSuccess) {
		qDebug() << "shaderProgram create failed!" << m_shaderProgram.log();
		return false;
	}

	bSuccess = m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "vertexShader.vert");
	if (!bSuccess) {
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_shaderProgram.log();
		return false;
	}

	bSuccess = m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "fragmentShader.frag");
	if (!bSuccess) {
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_shaderProgram.log();
		return false;
	}

	bSuccess = m_shaderProgram.link();
	if (!bSuccess) {
		qDebug() << "shaderProgram link failed!" << m_shaderProgram.log();
		return false;
	}

	return bSuccess;
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
	switch (event->key()) {
	case Qt::Key_W: //相机视角向前移动
	{
		m_camera->move(Camera::FORWARD, m_deltaTime);
		break;
	}
	case Qt::Key_S: //相机视角向后移动
	{
		m_camera->move(Camera::BACKWARD, m_deltaTime);
		break;
	}
	case Qt::Key_D: //相机视角向右移动
	{
		m_camera->move(Camera::LEFT, m_deltaTime);
		break;
	}
	case Qt::Key_A: //相机视角向左移动
	{
		m_camera->move(Camera::RIGHT, m_deltaTime);
		break;
	}
	case Qt::Key_Q: //相机视角向上移动
	{
		m_camera->move(Camera::UP, m_deltaTime);
		break;
	}
	case Qt::Key_E: //相机视角向下移动
	{
		m_camera->move(Camera::DOWN, m_deltaTime);
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

	}
	else if (Qt::RightButton == event->button())
	{
		m_lastX = event->pos().x();
		m_lastY = event->pos().y();

		m_btnPressStatus = ButtonRight;

	}
	else if (Qt::MiddleButton == event->button())
	{
		m_lastX = event->pos().x();
		m_lastY = event->pos().y();

		m_btnPressStatus = ButtonMiddle;

	}
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	m_btnPressStatus = ButtonNone;
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (ButtonLeft == m_btnPressStatus) //旋转相机视角
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
		int yoffset = ypos - m_lastY;

		m_lastX = xpos;
		m_lastY = ypos;

		m_camera->rotate(xoffset, yoffset);
		
		update();
	}
	else if (ButtonRight == m_btnPressStatus) //缩放相机视角
	{
		int ypos = event->pos().y();
		int yoffset = ypos - m_lastY;

		if (yoffset < 0)
		{
			m_camera->move(Camera::FORWARD, m_deltaTime);
		}
		else
		{
			m_camera->move(Camera::BACKWARD, m_deltaTime);
		}

		update();
	}
	else if (ButtonMiddle == m_btnPressStatus) //平移相机视角
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
		int yoffset = ypos - m_lastY;

		m_lastX = xpos;
		m_lastY = ypos;

		m_camera->pan(xoffset, yoffset, m_deltaTime);

		update();
	}
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
	QPoint offset = event->angleDelta();
	if (offset.y() > 0)
	{
		m_camera->move(Camera::FORWARD, m_deltaTime);
	}
	else
	{
		m_camera->move(Camera::BACKWARD, m_deltaTime);
	}
	update();
}

void OpenGLWidget::printContextInformation()
{
	QString glType;
	QString glVersion;
	QString glVendor;
	QString glProfile;

	glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
	glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	glVendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));

#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
	switch (format().profile())
	{
		CASE(NoProfile);
		CASE(CoreProfile);
		CASE(CompatibilityProfile);
	}
#undef CASE
	qDebug() << "Vender:" << glVendor;
	qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";

}

