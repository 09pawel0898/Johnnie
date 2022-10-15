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
	bool UseSpecularMap;
	
	sampler2D TextureNormalMap;
	bool UseNormalMap;
	
	vec3 Specular;
	float Shiness;
};

struct PointLight
{
	vec3 Position;
	
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	
	float Constant;
    float Linear;
    float Quadratic;
};

#define MAX_POINT_LIGHTS 4

struct DirectionalLight
{
	vec3 Direction;
	
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

uniform PointLight uPointLights[MAX_POINT_LIGHTS];
uniform int uNumPointLights;

uniform DirectionalLight uDirectionalLight;

out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragWorldPos;

uniform Material uMaterial;
uniform vec3 uCameraPosition;

vec3 CalculateDirectionalLight(DirectionalLight Light, vec3 Normal, vec3 ViewDir)
{
	vec3 lightDir = normalize(-Light.Direction);
    
	float diff = max(dot(Normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + ViewDir);
	float spec = pow(max(dot(ViewDir, halfwayDir), 0.0), uMaterial.Shiness);
    
	vec3 diffuse,ambient,specular;
	
	if(uMaterial.UseDiffuseMap)
	{
		diffuse = Light.Diffuse * diff * vec3(texture(uMaterial.TextureDiffuse, TexCoord));
		ambient = Light.Ambient * vec3(texture(uMaterial.TextureDiffuse, TexCoord));
	}
	else
	{
		diffuse = Light.Diffuse * diff * uMaterial.BaseColor;
		ambient = Light.Ambient * uMaterial.BaseColor;
	}
	
	if(uMaterial.UseSpecularMap)
	{
		specular = spec * Light.Specular * vec3(texture(uMaterial.TextureSpecular,TexCoord));	
	}
	else
	{
		specular = uMaterial.Specular * spec * Light.Specular;
	}
	return (ambient + diffuse + specular);
}


vec3 CalculatePointLight(PointLight Light, vec3 Normal, vec3 FragPos, vec3 ViewDir)
{
	vec3 lightDir = normalize(Light.Position - FragPos);
    
	// diffuse // 
	float diff = max(dot(Normal, lightDir), 0.0);
    
	// specular // 
    vec3 halfwayDir = normalize(lightDir + ViewDir);
    float spec = pow(max(dot(ViewDir, halfwayDir), 0.0), uMaterial.Shiness);
    
	// attenuation // 
    float distance    = length(Light.Position - FragPos);
    float attenuation = 1.0 / (Light.Constant + Light.Linear * distance + 
						Light.Quadratic * (distance * distance));    
    
	vec3 ambient,diffuse,specular;
	
	if(uMaterial.UseDiffuseMap)
	{
		diffuse = Light.Diffuse  * diff * vec3(texture(uMaterial.TextureDiffuse, TexCoord)) * attenuation;
		ambient = Light.Ambient  * vec3(texture(uMaterial.TextureDiffuse, TexCoord)) * attenuation;
	}
	else
	{
		diffuse = Light.Diffuse * diff * uMaterial.BaseColor * attenuation;
		ambient = Light.Ambient * uMaterial.BaseColor * attenuation;
	}
	
	if(uMaterial.UseSpecularMap)
	{
		specular = Light.Specular * spec * vec3(texture(uMaterial.TextureSpecular, TexCoord)) * attenuation;	
	}
	else
	{
		specular = uMaterial.Specular * spec * Light.Specular * attenuation;
	}
	
    return (ambient + diffuse + specular);
}

void main()
{
	vec3 norm;

	if(uMaterial.UseNormalMap)
	{
		norm = texture(uMaterial.TextureNormalMap, TexCoord).rgb;
		norm = normalize(norm * 2.0 - 1.0);
	}
	else
	{
		norm = normalize(Normal);
	}
	vec3 viewDir = normalize(uCameraPosition - FragWorldPos);
		
	vec3 result = CalculateDirectionalLight(uDirectionalLight,norm,viewDir);
	
	for(int i=0; i < uNumPointLights; i++)
	{
		result += CalculatePointLight(uPointLights[i],norm,FragWorldPos,viewDir);
	}
	FragColor = vec4(result,1.0);
}