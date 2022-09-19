#shader vertex

#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexUV;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragWorldPos;

uniform mat4 uProjMat;
uniform mat4 uViewMat;
uniform mat4 uModelMat;
uniform mat3 uNormalMat;


void main()
{
	gl_Position = uProjMat * uViewMat * uModelMat * vec4(aPosition,1.0);
	Normal = uNormalMat * aNormal;
	TexCoord = aTexUV;
	FragWorldPos = vec3(uModelMat * vec4(aPosition,1.0));
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
in vec3 FragWorldPos;

uniform bool uUseTextures;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

uniform Material uMaterial;

uniform vec3 uLightPosition;
uniform vec3 uBaseColor;
uniform vec3 uLightColor;

uniform vec3 uCameraPosition;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(uLightPosition - FragWorldPos);
	
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = diff * uLightColor;
	
	float ambientStrength = 0.35;
	vec3 ambient = ambientStrength * uLightColor;
	
	float specularStrength = 0.6;
	vec3 viewDir = normalize(uCameraPosition - FragWorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir),0.0),32);
	vec3 specular = specularStrength * spec * uLightColor;
	
	if(uUseTextures)
	{
		
		FragColor = vec4(texture(texture_diffuse1, TexCoord).xyz * (ambient + diffuse + specular),1.0);
	}
	else
	{
		FragColor = vec4(uBaseColor * (ambient + diffuse + specular),1.0);
	}
}