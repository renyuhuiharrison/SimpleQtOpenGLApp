#pragma once
#include <QMap>
#include <QString>

#include "Base.h"

class QOpenGLFunctions_4_3_Core;

class TextureManager
{
public:
	void SINGLE_OVER() {}
	uint createTexture(QString _path, QOpenGLFunctions_4_3_Core* _funs);
	uint createTexture(QString _path, QString _dir, QOpenGLFunctions_4_3_Core* _funs);
private:
	SINGLE_INSTANCE(TextureManager)
	TextureManager() {}

	QMap<QString, uint>	m_texMap;
};

