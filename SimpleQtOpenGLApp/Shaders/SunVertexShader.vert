#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 outColor;
out vec2 outTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;


void main()
{
   gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
};