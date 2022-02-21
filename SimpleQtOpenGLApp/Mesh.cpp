#include "stdafx.h"

//Qt
#include <QOpenGLFunctions_4_3_Core>

//local
#include "Vertex.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

#define SHADER_POSITION_ANCHOR		0
#define SHADER_NORMAL_ANCHOR		1
#define SHADER_TEXCOORD_ANCHOR		2

Mesh::Mesh(QOpenGLFunctions_4_3_Core* glFuncs, 
	QVector<Vertex> vertices, 
	QVector<GLuint> indices, 
	QVector<Texture> textures,
	Material material) :
	m_glFuncs(glFuncs),
	m_material(material)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;

	m_uniformBlockIndex = 0;

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

void Mesh::draw(Shader* shader)
{
	//uint numDiffuse = 1;
	//uint numSpecular = 1;
	//for (uint i = 0; i < m_textures.size(); i++)
	//{
	//	m_glFuncs->glActiveTexture(GL_TEXTURE0 + i);

	//	QString typeName = m_textures[i].type;
	//	QString numStr;
	//	if (TEXTURE_DIFFUSE_STR == typeName)
	//	{
	//		numStr = QString::number(numDiffuse++);
	//	}
	//	else if (TEXTURE_SPECULAR_STR == typeName)
	//	{
	//		numStr = QString::number(numSpecular++);
	//	}

	//	shader->setFloat((QString("material.") + typeName + numStr).toStdString().c_str(), i);
	//	m_glFuncs->glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	//}
	
	shader->setBool("bTexture2D", m_textures.size() > 0 ? true : false);

	m_glFuncs->glActiveTexture(GL_TEXTURE0);

	m_glFuncs->glBindVertexArray(m_vao);

	m_glFuncs->glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uniformBlockIndex, 0, sizeof(Material));
	m_glFuncs->glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

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

	//将顶点缓冲区VBO绑定至顶点属性
	GLuint vbo = 0;
	m_glFuncs->glGenBuffers(1, &vbo);
	m_glFuncs->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	m_glFuncs->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
	

	m_glFuncs->glGenBuffers(1, &m_uniformBlockIndex);
	m_glFuncs->glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBlockIndex);
	m_glFuncs->glBufferData(GL_UNIFORM_BUFFER, sizeof(m_material), (void*)(&m_material), GL_STATIC_DRAW);

	//EBO
	GLuint ebo = 0;
	m_glFuncs->glGenBuffers(1, &ebo);
	m_glFuncs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	m_glFuncs->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

	m_glFuncs->glEnableVertexAttribArray(SHADER_POSITION_ANCHOR);
	m_glFuncs->glVertexAttribPointer(SHADER_POSITION_ANCHOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	m_glFuncs->glEnableVertexAttribArray(SHADER_NORMAL_ANCHOR);
	m_glFuncs->glVertexAttribPointer(SHADER_NORMAL_ANCHOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));

	m_glFuncs->glEnableVertexAttribArray(SHADER_TEXCOORD_ANCHOR);
	m_glFuncs->glVertexAttribPointer(SHADER_TEXCOORD_ANCHOR, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoord));


	//解绑VAO
	m_glFuncs->glBindVertexArray(0);

}
