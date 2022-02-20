#pragma once

#include <QVector>


class QOpenGLFunctions_4_3_Core;
class Vertex;
class Mesh
{
public:
	Mesh(QOpenGLFunctions_4_3_Core* glFuncs, QVector<Vertex> vertices, QVector<GLuint> indices);
	~Mesh();

	void draw();

private:
	void setupMesh();

private:
	QOpenGLFunctions_4_3_Core* m_glFuncs;

	QVector<Vertex> m_vertices;
	QVector<GLuint> m_indices;

	GLuint m_vao;
	GLuint m_vbo;

};

