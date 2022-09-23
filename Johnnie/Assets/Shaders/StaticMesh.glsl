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
	vec3 BaseColor;
	
	vec3 Ambient;
	
	vec3 Diffuse;
	bool UseDiffuseMap;
	
	vec3 Specular;
	float Shiness;
};

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragWorldPos;

uniform sampler2D uTextureDiffuse;
uniform sampler2D uTextureSpecular;

uniform Material uMaterial;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;

uniform vec3 uCameraPosition;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(uLightPosition - FragWorldPos);
	
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = (diff * uMaterial.Diffuse) * uLightColor;
	
	//float ambientStrength = 0.35;
	vec3 ambient = uMaterial.Ambient * uLightColor;
	
	//float specularStrength = 0.6;
	vec3 viewDir = normalize(uCameraPosition - FragWorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir),0.0),uMaterial.Shiness);
	vec3 specular = uMaterial.Specular * spec * uLightColor;
	
	if(uMaterial.UseDiffuseMap)
	{
		
		FragColor = vec4(texture(uTextureDiffuse, TexCoord).xyz * (ambient + diffuse + specular),1.0);
	}
	else
	{
		FragColor = vec4(uMaterial.BaseColor * (ambient + diffuse + specular),1.0);
	}
}