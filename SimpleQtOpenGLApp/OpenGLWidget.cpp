#include "stdafx.h"

//local
#include "OpenGLWidget.h"
#include "Shader.h"
#include "Vertex.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"


OpenGLWidget::OpenGLWidget(QWidget*parent) : 
	QOpenGLWidget(parent)
{
	m_width = 0;
	m_height = 0;

	m_camera = new Camera(QVector3D(0.0f, 0.0f, 3.0f));
	m_deltaTime = 1.0f;
	m_lastX = width() / 2.0f;
	m_lastY = height() / 2.0f;
	m_bFirstMouse = true;
	m_bLoadMesh = false;
	m_model = nullptr;
	m_shader = nullptr;

	m_btnPressStatus = OpenGLWidget::ButtonNone;


	setFocusPolicy(Qt::StrongFocus);//用于响应键盘事件

}

OpenGLWidget::~OpenGLWidget()
{
	deleteAllMeshes();

	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}

	deleteModel();

	if (m_shader)
	{
		delete m_shader;
		m_shader = nullptr;
	}
}

void OpenGLWidget::importModel(QString fileName)
{
	makeCurrent();
	m_model = new Model(m_glFuncs, fileName);
	doneCurrent();
	m_bLoadMesh = true;
}


void OpenGLWidget::clearScene()
{
	deleteModel();
	deleteAllMeshes();

	update();
}

void OpenGLWidget::displayTriangle()
{
	QVector<Vertex> vertices;
	Vertex v1, v2, v3;
	v1.m_position = glm::vec3(0.0f, 0.75f, 1.0f);
	v2.m_position = glm::vec3(0.75f, -0.75f, 1.0f);
	v3.m_position = glm::vec3(-0.75f, -0.75f, 1.0f);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	QVector<GLuint> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	makeCurrent();
	Mesh* mesh = new Mesh(m_glFuncs, vertices, indices);
	doneCurrent();
	m_meshes.push_back(mesh);

	m_bLoadMesh = true;
}

void OpenGLWidget::initializeGL()
{
    this->initializeOpenGLFunctions();    

	m_glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

	printContextInformation();

	m_shader = new Shader(m_glFuncs);
	

	if (!m_shader->initShader(m_modelVShaderFilePath, m_modelFShaderFilePath)) {
		return;
	}
}

void OpenGLWidget::resizeGL(int w, int h)
{
	m_width = w;
	m_height = h;
	
    glViewport(0.0f, 0.0f, w, h);
}

void OpenGLWidget::paintGL()
{
	//设置清屏颜色
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	//清空颜色缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//开启深度测试
	glEnable(GL_DEPTH_TEST);

	if (!m_bLoadMesh)
		return;

	QMatrix4x4 modelMatrix;
	modelMatrix.setToIdentity();

	QMatrix4x4 viewMatrix = m_camera->GetViewMatrix();

	QMatrix4x4 projMatrix;
	projMatrix.perspective(m_camera->getZoom(), 1.0f * m_width / m_height, m_camera->getNear(), m_camera->getFar());

	m_shader->start();
	m_shader->setMatrix("modelMatrix", modelMatrix);
	m_shader->setMatrix("viewMatrix", viewMatrix);
	m_shader->setMatrix("projMatrix", projMatrix);

	//渲染网格
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->draw();
	}

	//渲染模型
	if (m_model && m_model->isLoadSuccess())
	{
		m_model->draw();
	}

	m_shader->end();
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

void OpenGLWidget::deleteAllMeshes()
{
	QVector<Mesh*>().swap(m_meshes);
}

void OpenGLWidget::deleteModel()
{
	if (m_model)
	{
		delete m_model;
		m_model = nullptr;
	}
}

