#pragma once

#include <QMatrix4x4>
#include <QVector3D>

class QOpenGLFunctions_3_3_Core;

class Shader
{
private:
	unsigned int m_shaderProgram;
	QOpenGLFunctions_3_3_Core* m_glFuncs;

public:
	Shader(QOpenGLFunctions_3_3_Core* glFuncs) : m_glFuncs(glFuncs)
	{
		m_shaderProgram = 0;
	}
	~Shader()
	{

	}
	bool initShader(const char* _vertexPath, const char* _fragPath);
	void start();
	void end();

	void setMatrix(const std::string & _name , QMatrix4x4 _matrix)const;
	void setVec3(const std::string& _name, QVector3D _vec3)const;
	void setFloat(const std::string& _name, float _f)const;
	void setInt(const std::string& _name, int _i)const;
};

