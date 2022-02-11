#include "stdafx.h"


//Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//local
#include "Model.h"
#include "Mesh.h"

Model::Model(QOpenGLFunctions_3_3_Core* glFuncs, QString fileName):
	m_glFuncs(glFuncs)
{
	m_bLoadSuccess = false;

	load(fileName);
}

Model::~Model()
{

}

void Model::draw()
{
	for (int i=0; i<m_meshes.size(); i++)
	{
		m_meshes[i]->draw();
	}
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
		//位置坐标
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;

		vertex.m_position = position;

		//法线
		if (mesh->HasNormals())
		{
			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;

			vertex.m_normal = normal;
		}

		//纹理坐标
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

	//解析index
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
