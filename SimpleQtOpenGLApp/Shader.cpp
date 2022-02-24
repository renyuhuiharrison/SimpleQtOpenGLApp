#include "stdafx.h"

//Qt
#include <QOpenGLFunctions_4_3_Core>
#include <QFile>
#include <QDebug>

//glm
#include <glm/gtc/type_ptr.hpp>

//local
#include "Shader.h"


uint Shader::compileShader(QString _shaderPath, GLint _shaderType)
{

	QFile shaderFile(_shaderPath);
    shaderFile.open(QIODevice::ReadOnly);
	QString shaderCode = shaderFile.readAll();

	std::string stdStrCode = shaderCode.toStdString();
	const char* _vShaderStr = stdStrCode.c_str();

	unsigned int   _shaderID = 0;
	char           _infoLog[512];
	int            _successFlag = 0;

	//±àÒë
    _shaderID = m_glFuncs->glCreateShader(_shaderType);
	m_glFuncs->glShaderSource(_shaderID, 1, &_vShaderStr, NULL);
	m_glFuncs->glCompileShader(_shaderID);

	m_glFuncs->glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &_successFlag);
	if (!_successFlag)
	{
		m_glFuncs->glGetShaderInfoLog(_shaderID, 512, NULL, _infoLog);
		QString errStr(_infoLog);
		qDebug() << errStr;
	}


	return _shaderID;
}

bool Shader::initShader(QString _vertexPath, QString _fragPath, QString _geoPath)
{
	unsigned int _vertexID = -1;
	unsigned int _fragID = -1;
	unsigned int _geoID = -1;

	//±àÒë
	_vertexID = compileShader(_vertexPath, GL_VERTEX_SHADER);
	_fragID = compileShader(_fragPath, GL_FRAGMENT_SHADER);
	if (!_geoPath.isEmpty()) {
		_geoID = compileShader(_geoPath, GL_GEOMETRY_SHADER);
	}
	
	char           _infoLog[512];
	int            _successFlag = 0;

    //Á´½Ó
	m_shaderProgram = m_glFuncs->glCreateProgram();
	if (_vertexID != -1)
	{
		m_glFuncs->glAttachShader(m_shaderProgram, _vertexID);
	}
	if (_fragID != -1)
	{
		m_glFuncs->glAttachShader(m_shaderProgram, _fragID);
	}
	if (_geoID != -1)
	{
		m_glFuncs->glAttachShader(m_shaderProgram, _geoID);
	}
	m_glFuncs->glLinkProgram(m_shaderProgram);

	//»ñÈ¡shaderÁ´½Ó×´Ì¬
    m_glFuncs->glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &_successFlag);
    if (!_successFlag)
    {
        m_glFuncs->glGetProgramInfoLog(m_shaderProgram, 512, NULL, _infoLog);
        QString errStr(_infoLog);
        qDebug() << errStr;

		return false;
	}

    m_glFuncs->glDeleteShader(_vertexID);
	m_glFuncs->glDeleteShader(_fragID);
	if (_geoID != -1) {
		m_glFuncs->glDeleteShader(_geoID);
	}

	return true;
}


void Shader::start()
{
    m_glFuncs->glUseProgram(m_shaderProgram);
}

void Shader::end()
{
    m_glFuncs->glUseProgram(0);
}


void Shader::setMatrix(const char* _name, glm::mat4 _matrix) const
{
    m_glFuncs->glUniformMatrix4fv(m_glFuncs->glGetUniformLocation(m_shaderProgram, _name), 1, GL_FALSE, glm::value_ptr(_matrix));
}

void Shader::setVec3(const char* _name, glm::vec3 _vec3)const
{
    m_glFuncs->glUniform3fv(m_glFuncs->glGetUniformLocation(m_shaderProgram, _name), 1, glm::value_ptr(_vec3));
}

void Shader::setVec4(const char* _name, glm::vec4 _vec4) const
{
	m_glFuncs->glUniform4fv(m_glFuncs->glGetUniformLocation(m_shaderProgram, _name), 1, glm::value_ptr(_vec4));
}

void Shader::setFloat(const char* _name, float _f)const
{
    m_glFuncs->glUniform1f(m_glFuncs->glGetUniformLocation(m_shaderProgram, _name), _f);
}

void Shader::setInt(const char* _name, int _i)const
{
    m_glFuncs->glUniform1i(m_glFuncs->glGetUniformLocation(m_shaderProgram, _name), _i);
}

void Shader::setBool(const char* _name, bool val) const
{
	m_glFuncs->glUniform1i(m_glFuncs->glGetUniformLocation(m_shaderProgram, _name), val);
}
