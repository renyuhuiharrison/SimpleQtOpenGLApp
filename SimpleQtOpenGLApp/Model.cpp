#include "stdafx.h"


//Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//glm
#include <glm/ext/matrix_transform.hpp>

//local
#include "Vertex.h"
#include "Model.h"
#include "Mesh.h"

Model::Model(QOpenGLFunctions_4_3_Core* glFuncs, QString fileName):
	m_glFuncs(glFuncs)
{
	m_bLoadSuccess = false;
	
	m_matrix = glm::mat4(1.0f);
	m_speed = 0.05f;

	load(fileName);
}

Model::~Model()
{
	QVector<Mesh*>().swap(m_meshes);
}

void Model::draw()
{
	for (int i=0; i<m_meshes.size(); i++)
	{
		m_meshes[i]->draw();
	}
}

void Model::move(MODEL_MOVE mode)
{
	switch (mode)
	{
	case Model::MOVE_LEFT:
		m_translate = -glm::vec3(1.0f, 0.0f, 0.0f) * m_speed;
		break;
	case Model::MOVE_RIGHT:
		m_translate = glm::vec3(1.0f, 0.0f, 0.0f) * m_speed;
		break;
	case Model::MOVE_FRONT:
		m_translate = -glm::vec3(0.0f, 0.0f, 1.0f) * m_speed;
		break;
	case Model::MOVE_BACK:
		m_translate = glm::vec3(0.0f, 0.0f, 1.0f) * m_speed;
		break;
	case Model::MOVE_UP:
		m_translate = -glm::vec3(0.0f, 1.0f, 0.0f) * m_speed;
		break;
	case Model::MOVE_DOWN:
		m_translate = glm::vec3(0.0f, 1.0f, 0.0f) * m_speed;
		break;
	default:
		break;
	}

	m_matrix = glm::translate(m_matrix, m_translate);
}


void Model::load(QString fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName.toStdString(), 
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		qDebug() << "Failed to read model";
		return;
	}

	m_fileName = fileName;

	processNode(scene->mRootNode, scene);

	m_bLoadSuccess = true;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (int i=0; i<node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}

	for (int i=0; i<node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	QVector<Vertex> vertices;
	QVector<GLuint> indices;

	Vertex vertex;
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		//λ������
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;

		vertex.m_position = position;

		//����
		if (mesh->HasNormals())
		{
			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;

			vertex.m_normal = normal;
		}

		//��������
		if (mesh->HasTextureCoords(0))
		{
			if (mesh->mTextureCoords[0])
			{
				glm::vec3 texCoord;
				texCoord.x = mesh->mTextureCoords[0][i].x;
				texCoord.y = mesh->mTextureCoords[0][i].y;

				vertex.m_textureCoord = texCoord;
			}
		}


		vertices.push_back(vertex);
	}

	//����index
	for (int i=0; i<mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return new Mesh(m_glFuncs, vertices, indices);
}
