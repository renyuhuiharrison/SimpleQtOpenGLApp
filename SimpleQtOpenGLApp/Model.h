#pragma once

#include <QVector>
#include <QString>

#include <glm/common.hpp>

#include <assimp/material.h>

class Mesh;
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
struct Texture;
class QOpenGLFunctions_4_3_Core;
class Shader;

class Model
{
public:
	enum MODEL_MOVE : int
	{
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_FRONT,
		MOVE_BACK,
		MOVE_UP,
		MOVE_DOWN
	};

public:
	Model(QOpenGLFunctions_4_3_Core* glFuncs, QString fileName);
	~Model();

	void draw(Shader* shader);
	
	bool isLoadSuccess()
	{
		return m_bLoadSuccess;
	}

	void move(MODEL_MOVE mode);


	glm::mat4 getMatrix()const
	{
		return m_matrix;
	}


private:
	void load(QString fileName);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	QVector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, QString typeName);

private:
	QOpenGLFunctions_4_3_Core* m_glFuncs;
	QVector<Mesh*> m_meshes;
	QString m_fileName;
	bool  m_bLoadSuccess;

	glm::mat4 m_matrix;
	glm::vec3 m_translate;
	float		m_speed;
};

