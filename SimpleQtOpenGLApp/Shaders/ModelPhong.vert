#version 430 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 textureCoord;

out vec3 varyingNormal; //顶点法向量
out vec3 varyingLightDir; //指向光源的向量
out vec3 varyingVertPosition; //顶点位置
out vec3 varyingHalfVector; //光线向量和视觉向量的角平分线向量，用于Blinn-Phong着色方式
out vec2 texCoord;
out vec4 Ambient;
out vec4 Diffuse;
out vec4 Specular;

struct PositionalLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;

	float kc;
	float kl;
	float kq;
};

// struct Material
// {
// 	vec4 ambient;
// 	vec4 diffuse;
// 	vec4 specular;
// 	float shininess;
// };


uniform vec4 globalAmbient;
uniform PositionalLight light;
// uniform Material material;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 normalMatrix;

uniform Mat{
	vec4 aAmbient;
	vec4 aDiffuse;
	vec4 aSpecular;
};

void main()
{
	//将顶点坐标转换到视图空间
	varyingVertPosition = (viewMatrix * modelMatrix * vec4(vertexPosition, 1.0)).xyz;

	//将法向量转换为到视图空间
	varyingNormal = (normalMatrix * vec4(vertexNormal, 1.0)).xyz;

	//计算光照向量（从顶点到光源）
	varyingLightDir = light.position - varyingVertPosition;
	
	varyingHalfVector = (varyingLightDir + (- varyingVertPosition)).xyz;

	texCoord = textureCoord;

	Ambient = aAmbient;
	Diffuse = aDiffuse;
	Specular = aSpecular;

	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
};