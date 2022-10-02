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
	
	sampler2D TextureDiffuse;
	bool UseDiffuseMap;
	
	sampler2D TextureSpecular;
	
	vec3 Specular;
	float Shiness;
};

struct Light
{
	vec3 Position;
	
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

uniform Light uLight;

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragWorldPos;


uniform Material uMaterial;

uniform vec3 uCameraPosition;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(uLight.Position - FragWorldPos);
	
	float diff = max(dot(norm,lightDir),0.0);
	
	vec3 diffuse;
	vec3 ambient;
	
	if(uMaterial.UseDiffuseMap)
	{
		diffuse = uLight.Diffuse * diff * vec3(texture(uMaterial.TextureDiffuse, TexCoord));
	}	
	else
	{
		diffuse = uLight.Diffuse * diff * uMaterial.BaseColor;
	}
	
	if(uMaterial.UseDiffuseMap)
	{
		ambient = uLight.Ambient * vec3(texture(uMaterial.TextureDiffuse, TexCoord));
	}
	else
	{
		ambient = uLight.Ambient * uMaterial.BaseColor;
	}
	
	vec3 viewDir = normalize(uCameraPosition - FragWorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir),0.0),uMaterial.Shiness);
	vec3 specular = uMaterial.Specular * (spec * uLight.Specular);
	
	if(uMaterial.UseDiffuseMap)
	{
		
		FragColor = vec4(texture(uMaterial.TextureDiffuse, TexCoord).xyz * (ambient + diffuse + specular),1.0);
	}
	else
	{
		FragColor = vec4(uMaterial.BaseColor * (ambient + diffuse + specular),1.0);
	}
}