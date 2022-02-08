#include "stdafx.h"

//Qt
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_3_3_Core>

//local
#include "Mesh.h"

Mesh::Mesh()
{
	//m_vao = new QOpenGLVertexArrayObject();
	//m_vao->create();

	m_numVertices = 0;
}

Mesh::~Mesh()
{

}

QOpenGLBuffer* Mesh::createBuffer()
{
	QOpenGLBuffer buffer;
	buffer.create();
	m_buffers.push_back(buffer);

	//return &m_buffers.back();
	return &m_buffers[m_buffers.size() - 1];
}

void Mesh::addVertex(Vertex v)
{
	m_vertices.push_back(v);
}

void Mesh::draw(QOpenGLShaderProgram* program)
{
	QOpenGLFunctions_3_3_Core* funcs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	funcs->glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}
