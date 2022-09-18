#shader vertex

#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexUV;

out vec2 TexCoord;
out vec3 Normal;
uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(aPosition,1.0);
	Normal = aNormal;
	TexCoord = aTexUV;
}

#shader fragment

#version 460 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shiness;
};

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;

uniform bool uUseTextures;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

uniform Material uMaterial;

uniform vec3 uBaseColor;
uniform vec3 uLightColor;

void main()
{
	if(uUseTextures)
	{
		float ambientStrenght = 0.3;
		vec3 x = vec3(1.0,1.0,1.0);
		vec3 ambient = ambientStrenght * x;
		FragColor = vec4(texture(texture_diffuse1, TexCoord).xyz * ambient,1.0);
	}
	else
	{
		FragColor = vec4(uLightColor * uBaseColor,1.0);
	}
}