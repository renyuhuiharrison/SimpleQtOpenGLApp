#pragma once


#include"glm/glm.hpp"

struct Vertex
{
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_textureCoord;

	Vertex()
	{
		m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_normal = glm::vec3(0.0f, 0.0f, 0.0f);
		m_textureCoord = glm::vec2(0.0f, 0.0f);
	}

	~Vertex()
	{

	}
};
