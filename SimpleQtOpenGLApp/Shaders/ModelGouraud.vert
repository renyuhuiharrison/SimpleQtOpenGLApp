#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

struct PositionalLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

out vec4 varyingColor;

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 normalMatrix;

void main()
{
	//将顶点坐标转换到视图空间
	vec4 P = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

	//将法向量转换为到视图空间
	vec3 N = normalize((normalMatrix * vec4(vertexNormal, 1.0)).xyz);

	//计算光照向量（从顶点到光源）
	vec3 L = normalize(light.position - P.xyz);

	//观察向量
	vec3 V = normalize(-P.xyz);

	//R是-L相对于表面向量N的镜像
	vec3 R = reflect(-L, N);

	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
	vec3 diffuse = (light.diffuse.xyz * material.diffuse.xyz) * max(dot(N, L), 0.0);
	vec3 specular = material.specular.xyz * light.specular.xyz * pow(max(dot(R, V), 0.0), material.shininess);

	varyingColor = vec4((ambient + diffuse + specular), 1.0);
	
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

};