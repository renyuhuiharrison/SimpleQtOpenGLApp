#include <algorithm>


#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"


#include "Image.h"


Image::Image(int _width, int _height, int _picType, PixelData* _data)
{
	m_width = _width;
	m_height = _height;
	m_picType = _picType;

	int _sumSize = m_width * m_height;
	if (_data && _sumSize)
	{
		m_data = new PixelData[_sumSize];
		memcpy(m_data, _data, sizeof(PixelData) * _sumSize);
	}
	else
	{
		m_data = nullptr;
	}
}

Image::~Image()
{
	if (m_data)
	{
		delete[]m_data;
	}
}

Image* Image::readFromFile(const char* _fileName)
{
	int			_picType = 0;
	int			_width = 0;
	int			_height = 0;

	//stbimage读入的图片是反过来的
	stbi_set_flip_vertically_on_load(true);

	unsigned char* bits = stbi_load(_fileName, &_width, &_height, &_picType, STBI_rgb_alpha);
	Image* _image = new Image(_width, _height, _picType, (PixelData*)bits);

	stbi_image_free(bits);

	return _image;
}