#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE


class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget*parent = nullptr);
    ~OpenGLWidget();

protected:
	//Sets up the OpenGL resources and state. 
	//Gets called once before the first time resizeGL() or paintGL() is called.
	void initializeGL() override;

	//Sets up the OpenGL viewport, projection, etc. 
	//Gets called whenever the widget has been resized 
	//and also when it is shown for the first time because all newly created widgets get a resize event automatically.
	void resizeGL(int w, int h) override;

	//Renders the OpenGL scene. Gets called whenever the widget needs to be updated
	void paintGL() override;

private:
	QOpenGLShaderProgram* m_shaderProgram;
};

