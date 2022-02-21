#pragma once

#include <QString>
#include "Base.h"

#define TEXTURE_DIFFUSE_STR "texture_diffuse"
#define TEXTURE_SPECULAR_STR "texture_specular"

struct Texture
{
	uint		id;
	QString		type;
	QString		path;
};