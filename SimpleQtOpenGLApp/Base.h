#pragma once

typedef unsigned int	uint;
typedef unsigned char	byte;

struct PixelData
{
	byte r;
	byte g;
	byte b;
	byte a;

	PixelData(byte _r = 255,
		byte _g = 255,
		byte _b = 255,
		byte _a = 255)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
};


#define SINGLE_INSTANCE(className)	private:\
										static className* m_Instance;\
										className(const className& gw) = delete; \
										className& operator=(const className& ins) = delete;\
									public:\
										~className()\
										{\
											this->SINGLE_OVER();\
											delete m_Instance;\
										}\
										static className* getInstance()\
										{\
											if (m_Instance == nullptr)\
											{\
												m_Instance = new className(); \
											}\
											return m_Instance;\
										}\

#define SINGLE_INSTANCE_SET(className)  className*  className::m_Instance = nullptr;
