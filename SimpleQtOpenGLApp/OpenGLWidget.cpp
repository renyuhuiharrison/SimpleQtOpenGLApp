#include "stdafx.h"

//glm
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

//local
#include "OpenGLWidget.h"
#include "Shader.h"
#include "Vertex.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"

//�׹�
glm::vec4 globalAmbient(0.7f, 0.7f, 0.7f, 1.0f);
glm::vec4 lightAmbient(0.2f, 0.2f, 0.2f, 1.0f);
glm::vec4 lightDiffuse(0.5f, 0.5f, 0.5f, 1.0f);
glm::vec4 lightSpecular(1.0f, 1.0f, 1.0f, 1.0f);

//�ƽ����
//glm::vec4 goldAmbient(0.2473f, 0.1995f, 0.0745f, 1.0f);
//glm::vec4 goldDiffuse(0.7516f, 0.6065f, 0.2265f, 1.0f);
//glm::vec4 goldSpecular(0.6283f, 0.5559f, 0.3661f, 1.0f);
//float goldShiness = 51.2f;


OpenGLWidget::OpenGLWidget(QWidget*parent) : 
	QOpenGLWidget(parent)
{
	m_width = 0;
	m_height = 0;

	m_camera = new Camera();
	m_camera->lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_camera->setSpeed(0.05f);

	m_lastX = width() / 2.0f;
	m_lastY = height() / 2.0f;

	m_bFirstMouse = true;
	m_bLoadTriangle = false;
	m_bLoadSun = false;
	m_bModelHighlight = false;
	m_bVertexNormal = false;

	m_modelImported = nullptr;
	m_shaderModel = nullptr;
	m_shaderSun = nullptr;
	m_shaderHighlight = nullptr;
	m_shaderVertexNormal = nullptr;

	m_lightPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	m_btnPressStatus = OpenGLWidget::ButtonNone;

	m_appDirPathName = QCoreApplication::applicationDirPath();

	setFocusPolicy(Qt::StrongFocus);//������Ӧ�����¼�

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

	if (m_shaderModel)
	{
		delete m_shaderModel;
		m_shaderModel = nullptr;
	}

	if (m_shaderSun)
	{
		delete m_shaderSun;
		m_shaderSun = nullptr;
	}

	if (m_shaderHighlight)
	{
		delete m_shaderHighlight;
		m_shaderHighlight = nullptr;
	}

	if (m_shaderVertexNormal)
	{
		delete m_shaderVertexNormal;
		m_shaderVertexNormal = nullptr;
	}
}

void OpenGLWidget::importModel(QString fileName)
{
	makeCurrent();
	m_modelImported = new Model(m_glFuncs, fileName);
	doneCurrent();
}


void OpenGLWidget::clearScene()
{
	deleteModel();
	deleteAllMeshes();

	update();
}

void OpenGLWidget::setModelHighlight(bool val)
{
	val ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
	m_bModelHighlight = val;
}

void OpenGLWidget::setVertexNormalVisible(bool val)
{
	if (m_modelImported && m_modelImported->isLoadSuccess())
	{
		if (!m_shaderVertexNormal) {
			m_shaderVertexNormal = new Shader(m_glFuncs);
			if (!m_shaderVertexNormal->initShader(m_vertNormalVertShaderPath,
				m_vertNormalFragShaderPath,
				m_vertNormalGeoShaderPath)) {
				return;
			}
		}

		m_bVertexNormal = val;

	}
}

void OpenGLWidget::displayTriangle()
{
	QVector<Vertex> vertices;
	Vertex v1, v2, v3;

	//��������
	v1.m_position = glm::vec3(0.0f, 1.0f, 0.0f);
	v2.m_position = glm::vec3(1.0f, 0.0f, 0.0f);
	v3.m_position = glm::vec3(-1.0f, 0.0f, 0.0f);

	//���㶥�㷨��
	v1.m_normal = glm::cross((v2.m_position - v1.m_position), (v3.m_position - v1.m_position));
	v2.m_normal = glm::cross((v1.m_position - v2.m_position), (v3.m_position - v2.m_position));
	v3.m_normal = glm::cross((v1.m_position - v3.m_position), (v2.m_position - v3.m_position));

	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	//��������
	QVector<GLuint> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	QVector<Texture> textures;
	Material material;

	makeCurrent();
	Mesh* mesh = new Mesh(m_glFuncs, vertices, indices, textures, material);
	doneCurrent();
	m_meshes.push_back(mesh);

	m_bLoadTriangle = true;
}

void OpenGLWidget::initializeGL()
{
    this->initializeOpenGLFunctions();    

	m_glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();

	printContextInformation();

	makeCurrent();

	loadSun();

	m_shaderModel = new Shader(m_glFuncs);
	if (!m_shaderModel->initShader(m_modelPhongVertShaderPath, m_modelPhongFragShaderPath)) {
		return;
	}

	m_shaderHighlight = new Shader(m_glFuncs);
	if (!m_shaderHighlight->initShader(m_modelHighlightVertShaderPath, m_modelHighlightFragShaderPath)) {
		return;
	}

	doneCurrent();
}

void OpenGLWidget::resizeGL(int w, int h)
{
	m_width = w;
	m_height = h;
	
    glViewport(0.0f, 0.0f, w, h);
}

void OpenGLWidget::paintGL()
{
	//���������޳�
	glEnable(GL_CULL_FACE);

	//����ģ�����
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);

	//������Ȳ���
	glEnable(GL_DEPTH_TEST);

	//����������ɫ
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	//�����ɫ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_camera->update();

	//ģ�;���
	glm::mat4 sunMatrix = m_modelSun->getMatrix();

	//����̫��λ��
	m_lightPosition[0] = sunMatrix[3][0];
	m_lightPosition[1] = sunMatrix[3][1];
	m_lightPosition[2] = sunMatrix[3][2];

	//��ͼ����
	glm::mat4 viewMatrix = m_camera->getMatrix();

	//ͶӰ����
	glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.1f, 100.0f);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);

	//��Ⱦ̫��
	if (m_bLoadSun && m_modelSun->isLoadSuccess())
	{
		m_shaderSun->start();
		{
			m_shaderSun->setMatrix("modelMatrix", sunMatrix);
			m_shaderSun->setMatrix("viewMatrix", viewMatrix);
			m_shaderSun->setMatrix("projMatrix", projMatrix);
			m_modelSun->draw(m_shaderModel);
		}
		m_shaderSun->end();
	}


	glm::mat4 modelMatrix(1.0f);

	//MV�������ת�þ��������任������
	glm::mat4 invTrMatrix = glm::transpose(glm::inverse(viewMatrix * modelMatrix));

	glm::vec3 viewPosition = m_camera->getPosition();

	//��Ⱦģ��
	if (m_modelImported && m_modelImported->isLoadSuccess())
	{
		m_shaderModel->start();
		{
			//����ģ��-��ͼ-ͶӰ����
			m_shaderModel->setMatrix("modelMatrix", modelMatrix);
			m_shaderModel->setMatrix("viewMatrix", viewMatrix);
			m_shaderModel->setMatrix("projMatrix", projMatrix);
			m_shaderModel->setMatrix("normalMatrix", invTrMatrix);
			m_shaderModel->setVec3("viewPosition", viewPosition);

			//���ù���
			m_lightPosView = glm::vec3(viewMatrix * glm::vec4(m_lightPosition, 1.0));

			m_shaderModel->setVec4("globalAmbient", globalAmbient);
			m_shaderModel->setVec4("light.ambient", lightAmbient);
			m_shaderModel->setVec4("light.diffuse", lightDiffuse);
			m_shaderModel->setVec4("light.specular", lightSpecular);
			m_shaderModel->setVec3("light.position", m_lightPosView);

			//���ù���˥��ϵ��
			m_shaderModel->setFloat("light.kc", 1.0f);
			m_shaderModel->setFloat("light.kl", 0.09f);
			m_shaderModel->setFloat("light.kq", 0.032f);

			//��Ⱦ����
			for (int i = 0; i < m_meshes.size(); i++)
			{
				m_meshes[i]->draw(m_shaderModel);
			}


			//��Ⱦ�����ģ��
			if (m_bModelHighlight)
			{
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			}

			m_modelImported->draw(m_shaderModel);


			//���Ƹ�����Ե
			if (m_bModelHighlight)
			{
				m_shaderHighlight->start();
				{
					glStencilMask(0x00);
					glDisable(GL_DEPTH_TEST);
					glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

					//������������1.05��
					modelMatrix = glm::scale(modelMatrix, glm::vec3(1.05f, 1.05f, 1.05f));
					m_shaderHighlight->setMatrix("modelMatrix", modelMatrix);
					m_shaderHighlight->setMatrix("viewMatrix", viewMatrix);
					m_shaderHighlight->setMatrix("projMatrix", projMatrix);

					m_modelImported->draw(m_shaderModel);
				}
				m_shaderHighlight->end();

				glStencilFunc(GL_ALWAYS, 1, 0xFF);
			}
		}
		m_shaderModel->end();
	}


	//���Ʒ���
	if (m_bVertexNormal)
	{
		m_shaderVertexNormal->start();
		{
			m_shaderVertexNormal->setMatrix("modelMatrix", modelMatrix);
			m_shaderVertexNormal->setMatrix("viewMatrix", viewMatrix);
			m_shaderVertexNormal->setMatrix("projMatrix", projMatrix);

			m_modelImported->draw(m_shaderVertexNormal);
		}

		m_shaderVertexNormal->end();
	}
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
	switch (event->key()) {
	case Qt::Key_W: //����ӽ���ǰ�ƶ�
	{
		m_camera->move(CAMERA_MOVE::MOVE_FRONT);
		break;
	}
	case Qt::Key_S: //����ӽ�����ƶ�
	{
		m_camera->move(CAMERA_MOVE::MOVE_BACK);
		break;
	}
	case Qt::Key_D: //����ӽ������ƶ�
	{
		m_camera->move(CAMERA_MOVE::MOVE_RIGHT);
		break;
	}
	case Qt::Key_A: //����ӽ������ƶ�
	{
		m_camera->move(CAMERA_MOVE::MOVE_LEFT);
		break;
	}
	case Qt::Key_Q: //����ӽ������ƶ�
	{
		m_camera->move(CAMERA_MOVE::MOVE_UP);
		break;
	}
	case Qt::Key_E: //����ӽ������ƶ�
	{
		m_camera->move(CAMERA_MOVE::MOVE_DOWN);
		break;
	}
	case Qt::Key_I: //�ƹ���ǰ�ƶ�
	{
		m_modelSun->move(Model::MOVE_FRONT);
		break;
	}
	case Qt::Key_K: //�ƹ�����ƶ�
	{
		m_modelSun->move(Model::MOVE_BACK);
		break;
	}
	case Qt::Key_J: //�ƹ������ƶ�
	{
		m_modelSun->move(Model::MOVE_LEFT);
		break;
	}
	case Qt::Key_L: //�ƹ������ƶ�
	{
		m_modelSun->move(Model::MOVE_RIGHT);
		break;
	}
	case Qt::Key_U: //�ƹ������ƶ�
	{
		m_modelSun->move(Model::MOVE_UP);
		break;
	}
	case Qt::Key_O: //����ӽ������ƶ�
	{
		m_modelSun->move(Model::MOVE_DOWN);
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
	if (ButtonLeft == m_btnPressStatus) //��ת����ӽ�
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
	else if (ButtonRight == m_btnPressStatus) //��������ӽ�
	{
		int ypos = event->pos().y();
		int yoffset = ypos - m_lastY;

		m_lastY = ypos;

		if (yoffset < 0)
		{
			m_camera->move(CAMERA_MOVE::MOVE_FRONT);
		}
		else
		{
			m_camera->move(CAMERA_MOVE::MOVE_BACK);
		}

		update();
	}
	else if (ButtonMiddle == m_btnPressStatus) //ƽ������ӽ�
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

		m_camera->pan(xoffset, yoffset);

		update();
	}
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
	QPoint offset = event->angleDelta();
	if (offset.y() > 0)
	{
		m_camera->move(CAMERA_MOVE::MOVE_FRONT);
	}
	else
	{
		m_camera->move(CAMERA_MOVE::MOVE_BACK);
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
	if (m_modelImported)
	{
		delete m_modelImported;
		m_modelImported = nullptr;
	}
}

void OpenGLWidget::loadSun()
{
	QString sunFilePath = m_appDirPathName + m_modelSunRelPath;

	m_modelSun = new Model(m_glFuncs, sunFilePath);

	m_shaderSun = new Shader(m_glFuncs);

	if (!m_shaderSun->initShader(m_sunVShaderPath, m_sunFShaderPath)) {
		return;
	}

	m_bLoadSun = true;
}



