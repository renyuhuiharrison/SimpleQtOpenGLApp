#version 430 core

layout(triangles) in;
layout(line_strip , max_vertices = 6) out;

in vec3 normal[];

uniform mat4 projMatrix;

void generateLine(int _index)
{
	float _mag = 0.05;
	gl_Position = projMatrix * gl_in[_index].gl_Position;
	EmitVertex();

	gl_Position = projMatrix * (gl_in[_index].gl_Position + vec4(_mag * normal[_index] , 1.0));
	EmitVertex();

	EndPrimitive();
}

void main()
{
	generateLine(0);
	generateLine(1);
	generateLine(2);
}