#version 330 core

layout (location = 0) in vec3 VertexPosition;
in vec3 VertexColor;

out vec3 Color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
	Color = vec3(1.0f, 0.0f, 0.0f);//VertexColor;
	gl_Position = projMatrix * viewMatrix  * modelMatrix * vec4(VertexPosition, 1.0);
};