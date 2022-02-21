#version 430 core

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPosition;
in vec3 varyingHalfVector;
in vec2 texCoord;

in vec4 Ambient;
in vec4 Diffuse;
in vec4 Specular;

out vec4 fragColor;

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
uniform vec3 viewPosition;
uniform sampler2D  texture2D;
uniform bool bTexture2D;

void main()
{
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(viewPosition - varyingVertPosition);
	vec3 H = normalize(varyingHalfVector);

	//计算光照向量基于N的反射向量
	vec3 R = normalize(reflect(-L, N));

	//计算光照与平面法向量的角度
	float cosTheta = dot(L, N);

	//计算视觉向量与反射光向量的角度
	float cosPhi = dot(H, N);

	//计算衰减
    float dis = length(light.position - varyingVertPosition);
    float attenuation = 1.0f / (light.kc + light.kl * dis + light.kq * dis* dis);

	//按像素计算ADS分量，并合并以构建输出颜色
	vec3 ambient = ((globalAmbient * Ambient) + (light.ambient * Ambient)).xyz;
	vec3 diffuse = (light.diffuse.xyz * Diffuse.xyz) * max(cosTheta, 0.0);
	vec3 specular = light.specular.xyz * Specular.xyz * pow(max(cosPhi, 0.0), 4);

	vec3 result = (ambient + diffuse + specular) * attenuation;

	if (bTexture2D) {
		fragColor = texture(texture2D, texCoord) * vec4(result, 1.0);
	}else {
		fragColor = vec4(result, 1.0);
	}
}