#pragma once

#include "Base.h"

class Image
{
public:
	Image(int _width = 0, int _height = 0, int _picType = 0, PixelData* _data = nullptr);
	~Image();

	int			getWidth()const { return m_width; }
	int			getHeight()const { return m_height; }
	int			getPicType()const { return m_picType; }
	PixelData*	getData()const { return m_data; }

	PixelData getColor(int x, int y)const
	{
		if (x < 0 || x > m_width - 1 || y <0 || y > m_height - 1)
		{
			return PixelData(0, 0, 0, 0);
		}
		return m_data[y * m_width + x];
	}


public:
	static Image* readFromFile(const char* _fileName);

private:
	int	m_width;
	int	m_height;
	int	m_picType;

	PixelData* m_data;
};

