#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3		normal;
uniform mat4	modelMatrix;
uniform mat4	viewMatrix;

void main()
{
	gl_Position = viewMatrix * modelMatrix * vec4(aPos , 1.0);
	mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
	normal = normalMatrix * aNormal;
};