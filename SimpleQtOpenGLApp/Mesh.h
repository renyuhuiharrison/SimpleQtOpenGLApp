#pragma once

#include <QVector>

#include "Vertex.h"

class QOpenGLFunctions_3_3_Core;

class Mesh
{
public:
	Mesh(QOpenGLFunctions_3_3_Core* glFuncs, QVector<Vertex> vertices, QVector<GLuint> indices);
	~Mesh();

	void draw();

private:
	void setupMesh();

private:
	QOpenGLFunctions_3_3_Core* m_glFuncs;

	QVector<Vertex> m_vertices;
	QVector<GLuint> m_indices;

	GLuint m_vao;
	GLuint m_vbo;

};

