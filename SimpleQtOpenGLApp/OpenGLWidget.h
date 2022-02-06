#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

class Camera;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:

	//按下鼠标键的状态
	enum ButtonPressStatus : int
	{
		ButtonNone = 0,  //没有按下任何键
		ButtonRight,
		ButtonLeft,
		ButtonMiddle,
		ButtonRightLeft  //左键和右键同时按下
	};

public:
    explicit OpenGLWidget(QWidget*parent = nullptr);
    ~OpenGLWidget();

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

	void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

private:

	int m_width;
	int m_height;
	
	QOpenGLShaderProgram m_shaderProgram;

	QOpenGLBuffer m_vbo; //顶点缓存对象（Vertex Buffer Object）
	QOpenGLVertexArrayObject m_vao; //顶点数组对象（Vertex Array Object）

	Camera* m_camera;
	float	m_lastX;
	float	m_lastY;
	bool	m_bFirstMouse;
	bool	m_bLButtonPressed;
	bool	m_bRButtonPressed;

	ButtonPressStatus m_btnPressStatus;

	int		m_frame;
	float	m_deltaTime;  // time between current frame and last frame
	float	m_lastFrame;
};

