#pragma once

#include <QVector>
#include <QString>


class Mesh;
struct aiNode;
struct aiScene;
struct aiMesh;
class QOpenGLFunctions_3_3_Core;

class Model
{
public:
	Model(QOpenGLFunctions_3_3_Core* glFuncs, QString fileName);
	~Model();

	void draw();
	
	bool isLoadSuccess()
	{
		return m_bLoadSuccess;
	}

private:
	void load(QString fileName);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);


private:
	QOpenGLFunctions_3_3_Core* m_glFuncs;
	QVector<Mesh*> m_meshes;
	QString m_fileName;
	bool  m_bLoadSuccess;
};

