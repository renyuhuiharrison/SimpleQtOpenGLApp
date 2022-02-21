//Qt
#include <QOpenGLFunctions_4_3_Core>

#include "TextureManager.h"
#include "Image.h"

SINGLE_INSTANCE_SET(TextureManager)


uint TextureManager::createTexture(QString _path, QOpenGLFunctions_4_3_Core* _funs)
{
	QMap<QString, uint>::iterator _it = m_texMap.find(_path);
	if (_it != m_texMap.end())
	{
		return _it.value();
	}

	Image* _image = Image::readFromFile(_path.toStdString().c_str());

	uint _texID = 0;
	_funs->glGenTextures(1, &_texID);
	_funs->glBindTexture(GL_TEXTURE_2D, _texID);

	_funs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _image->getWidth(), _image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _image->getData());
	_funs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	_funs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	_funs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	_funs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete _image;
	m_texMap[_path] = _texID;
	return _texID;
}

uint TextureManager::createTexture(QString _path, QString _dir, QOpenGLFunctions_4_3_Core* _funs)
{
	return createTexture(_dir + '/' + _path, _funs);
}
