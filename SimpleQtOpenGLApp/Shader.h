#pragma once

#include <QString>

#include"glm/glm.hpp"

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
	bool initShader(QString _vertexPath, QString _fragPath);
	void start();
	void end();

	void setMatrix(const char* _name, glm::mat4 _matrix)const;
	void setVec3(const char* _name, glm::vec3 _vec3)const;
	void setVec4(const char* _name, glm::vec4 _vec4)const;
	void setFloat(const char* _name, float _f)const;
	void setInt(const char* _name, int _i)const;
};

