#pragma once

#include <QVector>
#include <QOpenGLVertexArrayObject>

#include "Vertex.h"

class QOpenGLBuffer;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

class Mesh
{
public:
	Mesh();
	~Mesh();

	QOpenGLBuffer* createBuffer();

	void addVertex(Vertex v);

	void draw(QOpenGLShaderProgram* program);

	//QOpenGLVertexArrayObject* getVAO() const
	//{
	//	return m_vao;
	//}


	QVector<Vertex> getVertices()
	{
		return m_vertices;
	}

private:
	QVector<QOpenGLBuffer> m_buffers;
	QOpenGLVertexArrayObject m_vao;

	QVector<Vertex> m_vertices;
	int m_numVertices;
};

