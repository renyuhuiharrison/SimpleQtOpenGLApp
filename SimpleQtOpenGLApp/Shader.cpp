#include "stdafx.h"


//Qt
#include <QOpenGLFunctions_3_3_Core>
#include <QFile>
#include <QDebug>

//local
#include "Shader.h"


bool Shader::initShader(QString _vertexPath, QString _fragPath)
{
	QFile vertexFile(_vertexPath);
    vertexFile.open(QIODevice::ReadOnly);
    QString vertexCode = vertexFile.readAll();

	QFile fragFile(_fragPath);
    fragFile.open(QIODevice::ReadOnly);
	QString fragCode = fragFile.readAll();

    std::string stdStrVCode = vertexCode.toStdString();
    const char* _vShaderStr = stdStrVCode.c_str();

	std::string stdStrFCode = fragCode.toStdString();
	const char* _fShaderStr = stdStrFCode.c_str();

    //shaderµÄ±àÒëÁ´½Ó
    unsigned int   _vertexID = 0, _fragID = 0;
    char           _infoLog[512];
    int            _successFlag = 0;

    //±àÒë
    _vertexID = m_glFuncs->glCreateShader(GL_VERTEX_SHADER);
    m_glFuncs->glShaderSource(_vertexID, 1, &_vShaderStr, NULL);
    m_glFuncs->glCompileShader(_vertexID);

    m_glFuncs->glGetShaderiv(_vertexID, GL_COMPILE_STATUS, &_successFlag);
    if (!_successFlag)
    {
        m_glFuncs->glGetShaderInfoLog(_vertexID, 512, NULL, _infoLog);
        std::string errStr(_infoLog);
        qDebug() << _infoLog;

		return false;
	}

    _fragID = m_glFuncs->glCreateShader(GL_FRAGMENT_SHADER);
    m_glFuncs->glShaderSource(_fragID, 1, &_fShaderStr, NULL);
    m_glFuncs->glCompileShader(_fragID);

    m_glFuncs->glGetShaderiv(_fragID, GL_COMPILE_STATUS, &_successFlag);
    if (!_successFlag)
    {
        m_glFuncs->glGetShaderInfoLog(_fragID, 512, NULL, _infoLog);
        std::string errStr(_infoLog);
        qDebug() << _infoLog;

        return false;
    }

    //Á´½Ó
    m_shaderProgram = m_glFuncs->glCreateProgram();
    m_glFuncs->glAttachShader(m_shaderProgram, _vertexID);
    m_glFuncs->glAttachShader(m_shaderProgram, _fragID);
    m_glFuncs->glLinkProgram(m_shaderProgram);

    m_glFuncs->glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &_successFlag);
    if (!_successFlag)
    {
        m_glFuncs->glGetProgramInfoLog(m_shaderProgram, 512, NULL, _infoLog);
        std::string errStr(_infoLog);
        qDebug() << _infoLog;

		return false;
	}
    m_glFuncs->glDeleteShader(_vertexID);
    m_glFuncs->glDeleteShader(_fragID);

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

void Shader::setMatrix(const std::string& _name, QMatrix4x4 _matrix)const
{
    m_glFuncs->glUniformMatrix4fv(m_glFuncs->glGetUniformLocation(m_shaderProgram, _name.c_str()) , 1 , GL_FALSE , _matrix.data());
}

void Shader::setVec3(const std::string& _name, QVector3D _vec3)const
{
	float* vecArr = new float[3];
	for (int i = 0; i < 3; ++i) {
        vecArr[i] = _vec3[i];
	}
    m_glFuncs->glUniform3fv(m_glFuncs->glGetUniformLocation(m_shaderProgram, _name.c_str()), 1, vecArr);
}

void Shader::setFloat(const std::string& _name, float _f)const
{
    m_glFuncs->glUniform1f(m_glFuncs->glGetUniformLocation(m_shaderProgram, _name.c_str()), _f);
}

void Shader::setInt(const std::string& _name, int _i)const
{
    m_glFuncs->glUniform1i(m_glFuncs->glGetUniformLocation(m_shaderProgram, _name.c_str()), _i);
}