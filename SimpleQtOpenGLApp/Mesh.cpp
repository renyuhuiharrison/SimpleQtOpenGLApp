#include "stdafx.h"

//Qt
#include <QOpenGLFunctions_3_3_Core>

//local
#include "Vertex.h"
#include "Mesh.h"


#define SHADER_POSITION_ANCHOR		0
#define SHADER_NORMAL_ANCHOR		1
//#define SHADER_TEXCOORD_ANCHOR		2

Mesh::Mesh(QOpenGLFunctions_3_3_Core* glFuncs, QVector<Vertex> vertices, QVector<GLuint> indices) :
	m_glFuncs(glFuncs)
{
	m_vertices = vertices;
	m_indices = indices;

	setupMesh();
}

Mesh::~Mesh()
{
	m_vertices.clear();
	m_indices.clear();

	//m_glFuncs->initializeOpenGLFunctions();

	if (m_vbo != 0) {
		m_glFuncs->glDeleteBuffers(1, &m_vbo);
	}

	if (m_vao != 0) {
		m_glFuncs->glDeleteVertexArrays(1, &m_vao);
	}

}

void Mesh::draw()
{
	m_glFuncs->glBindVertexArray(m_vao);
	m_glFuncs->glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	//m_glFuncs->glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	m_glFuncs->glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	if (m_vertices.empty())
	{
		return;
	}

	//绑定VAO
	m_glFuncs->glGenVertexArrays(1, &m_vao);
	m_glFuncs->glBindVertexArray(m_vao);

	//将顶点缓冲区VBO#0绑定至顶点属性#0
	GLuint vbo_vertex = 0;
	m_glFuncs->glGenBuffers(1, &vbo_vertex);
	m_glFuncs->glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	m_glFuncs->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

	//将法向缓冲区VBO#1绑定至顶点属性#1
	GLuint vbo_normal = 0;
	m_glFuncs->glGenBuffers(1, &vbo_normal);
	m_glFuncs->glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	m_glFuncs->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);



	m_glFuncs->glEnableVertexAttribArray(SHADER_NORMAL_ANCHOR);
	m_glFuncs->glVertexAttribPointer(SHADER_NORMAL_ANCHOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));

	//m_glFuncs->glEnableVertexAttribArray(SHADER_TEXCOORD_ANCHOR);
	//m_glFuncs->glVertexAttribPointer(SHADER_TEXCOORD_ANCHOR, 2, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)offsetof(ffVertex, m_texCoord));

	//EBO
	GLuint ebo = 0;
	m_glFuncs->glGenBuffers(1, &ebo);
	m_glFuncs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	m_glFuncs->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

	m_glFuncs->glEnableVertexAttribArray(SHADER_POSITION_ANCHOR);
	m_glFuncs->glVertexAttribPointer(SHADER_POSITION_ANCHOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//解绑VAO
	m_glFuncs->glBindVertexArray(0);

}
