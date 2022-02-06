#version 330 core

in vec3 VertexPosition;
in vec3 VertexColor;

out vec3 Color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
	Color = VertexColor;
	gl_Position = projMatrix * viewMatrix  * modelMatrix * vec4(VertexPosition, 1.0);
};