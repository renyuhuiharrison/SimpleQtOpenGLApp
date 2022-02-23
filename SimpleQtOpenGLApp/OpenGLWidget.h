#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>

#include"glm/glm.hpp"

class Camera;
class Mesh;
class Model;
class Shader;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT
public:

	//����������״̬
	enum ButtonPressStatus : int
	{
		ButtonNone = 0,  //û�а����κμ�
		ButtonRight,
		ButtonLeft,
		ButtonMiddle
	};

public:
    explicit OpenGLWidget(QWidget*parent = nullptr);
    ~OpenGLWidget();

	void displayTriangle();
	
	void importModel(QString fileName);

	void clearScene();

	void setModelHighlight(bool val);

protected:
	//Sets up the OpenGL resources and state. 
	//Gets called once before the first time resizeGL() or paintGL() is called.
	void initializeGL() Q_DECL_OVERRIDE;

	//Sets up the OpenGL viewport, projection, etc. 
	//Gets called whenever the widget has been resized 
	//and also when it is shown for the first time because all newly created widgets get a resize event automatically.
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;

	//Renders the OpenGL scene. Gets called whenever the widget needs to be updated
	void paintGL() Q_DECL_OVERRIDE;

	//���̺�������Ӧ�¼�
	void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

private:
	//��ӡopengl�����Ϣ
	void printContextInformation();

	void deleteAllMeshes();
	void deleteModel();

	void loadSun();


private:
	//Gouraud shader
	const QString m_modelGouraudVertShaderFilePath = ":/Shaders/ModelGouraud.vert";
	const QString m_modelGouraudFragShaderFilePath = ":/Shaders/ModelGouraud.frag";

	//Phong shader
	const QString m_modelPhongVertShaderFilePath = ":/Shaders/ModelPhong.vert";
	const QString m_modelPhongFragShaderFilePath = ":/Shaders/ModelPhong.frag";

	const QString m_sunVShaderFilePath = ":/Shaders/SunVertexShader.vert";
	const QString m_sunFShaderFilePath = ":/Shaders/SunFragmentShader.frag";

	const QString m_modelSunFileRelPath = "/Models/sun.obj";

	const QString m_modelHighlightVertShaderFilePath = ":/Shaders/ModelHighlight.vert";
	const QString m_modelHighlightFragShaderFilePath = ":/Shaders/ModelHighlight.frag";

	int m_width; //���ڿ��
	int m_height;  //���ڸ߶�

	QString m_appDirPathName; //��������·��

	Shader* m_shaderModel;
	Shader* m_shaderSun;
	Shader* m_shaderHighlight;
	QOpenGLFunctions_4_3_Core* m_glFuncs;

	Camera* m_camera;
	float	m_lastX;
	float	m_lastY;
	bool	m_bFirstMouse;

	ButtonPressStatus m_btnPressStatus;

	bool	m_bLoadTriangle;
	bool	m_bLoadSun; //�Ƿ����sunģ��
	bool	m_bModelHighlight; //ģ���Ƿ����

	QVector<Mesh*> m_meshes;

	Model* m_modelImported;

	Model* m_modelSun;

	glm::vec3 m_lightPosition;
	glm::vec3 m_lightPosView;
};

