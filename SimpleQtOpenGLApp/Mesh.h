#pragma once

#include <QVector>


class QOpenGLFunctions_4_3_Core;
struct Vertex;
struct Texture;
class Shader;

struct Material {
	//环境光
	glm::vec4 Ka = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//漫反射
	glm::vec4 Kd = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	//镜反射
	glm::vec4 Ks = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
};

class Mesh
{
public:
	Mesh(QOpenGLFunctions_4_3_Core* glFuncs, 
		QVector<Vertex> vertices, 
		QVector<GLuint> indices, 
		QVector<Texture> textures,
		Material material);
	~Mesh();

	void draw(Shader* shader);

private:
	void setupMesh();

private:
	QOpenGLFunctions_4_3_Core* m_glFuncs;

	QVector<Vertex> m_vertices;
	QVector<GLuint> m_indices;
	QVector<Texture> m_textures;
	Material m_material;

	GLuint m_vao;
	GLuint m_vbo;

	GLuint m_uniformBlockIndex;
};

