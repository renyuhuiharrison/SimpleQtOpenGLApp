#pragma once

#include <QVector3D>

class Vertex
{
public:
	Vertex();
	Vertex(const QVector3D& position);
	Vertex(const QVector3D& position, const QVector3D& color);

	inline const QVector3D& getPosition()
	{
		return m_position;
	}

	inline const QVector3D& getColor()
	{
		return m_color;
	}

	static inline int getPositionOffset()
	{
		return offsetof(Vertex, m_position);
	}

	static inline int getColorOffset()
	{
		return offsetof(Vertex, m_color);
	}

	static inline int getStride()
	{
		return sizeof(Vertex);
	}
private:
	QVector3D m_position;
	QVector3D m_color;

};

