#shader vertex

#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in ivec4 aBoneIDs;
layout (location = 5) in vec4 aWeights;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragWorldPos;
out mat3 TBN;
out vec4 ShadowCoord;

uniform mat4 uProjMat;
uniform mat4 uViewMat;
uniform mat4 uModelMat;
uniform mat3 uNormalMat;
uniform mat4 uDepthBiasMVP;

// Skeleton Data //

const int MAX_BONES = 200;
const int MAX_BONES_INFLUENCED_VERTEX = 4;

uniform bool uIsSkinnedMesh;
uniform mat4 uBones[MAX_BONES];
uniform mat4 uFixedScaleMatrix;

void main()
{
	if(uIsSkinnedMesh)
	{
		mat4 BoneTransform = mat4(1.0);
	
		for(int idx = 0 ; idx < MAX_BONES_INFLUENCED_VERTEX ; idx++)
		{
			BoneTransform += uBones[aBoneIDs[idx]] * aWeights[idx];
		}
		
		BoneTransform = uFixedScaleMatrix * BoneTransform;
		
		vec4 TransformedPosition = BoneTransform * vec4(aPosition,1.0);
		gl_Position = uProjMat * uViewMat * uModelMat * TransformedPosition;
	}
	else
	{
		gl_Position = uProjMat * uViewMat * uModelMat * vec4(aPosition,1.0);
	}
	
	vec3 T = normalize(vec3(uModelMat * vec4(aTangent,   0.0)));
	vec3 N = normalize(vec3(uModelMat * vec4(aNormal,    0.0)));
	T = normalize(T - dot(T,N) * N);
	
	vec3 B = cross(N,T);
	
	TBN = transpose(mat3(T, B, N));
	
	Normal = uNormalMat * aNormal;
	TexCoord = aTexUV;
	FragWorldPos = vec3(uModelMat * vec4(aPosition,1.0));
	ShadowCoord = uDepthBiasMVP * vec4(aPosition,1.0);
	
	// Fog Calculation
	//vec4 positionRelativeToCam = mat4(1.0) * (uModelMat * vec4(aPosition,1.0));
	
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
in mat3 TBN; 
in vec4 ShadowCoord;

//uniform vec3 uFogColor;


uniform Material uMaterial;
uniform vec3 uCameraPosition;
uniform sampler2D uTextureShadowMap;

vec3 CalculateDirectionalLight(DirectionalLight Light, vec3 Normal, vec3 ViewDir, float Shadow)
{
	vec3 lightDir;

	if(uMaterial.UseNormalMap)
	{
		lightDir = TBN * normalize(-Light.Direction);
	}
	else
	{
		lightDir = normalize(-Light.Direction);
	}

	float diff = max(dot(Normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + ViewDir);
	float spec = pow(max(dot(Normal, halfwayDir), 0.0), uMaterial.Shiness);
    
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
	return (ambient + (1.0-Shadow) * (diffuse + specular));
}


vec3 CalculatePointLight(PointLight Light, vec3 Normal, vec3 FragPos, vec3 ViewDir, float Shadow)
{
	vec3 lightDir;
	
	if(uMaterial.UseNormalMap)
	{
		lightDir = TBN * normalize(Light.Position - FragPos);
	}
	else
	{
		lightDir = normalize(Light.Position - FragPos);
	}
	
	
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
	
    return (ambient + (1.0-Shadow)*(diffuse + specular));
}

float ShadowCalculation(vec4 FragPosLightSpace, vec3 Normal)
{
	if(FragPosLightSpace.z > 1.0)
	{	
		return 0.0;
	}	
	
	float temp = clamp(dot(Normal,uDirectionalLight.Direction),0.0,1.0);
	
	float bias = 0.005 * tan(acos(temp));
	bias = clamp(bias,0.0,0.01);
	
	float currentDepth = FragPosLightSpace.z;
	
	//float closestDepth = texture(uTextureShadowMap, FragPosLightSpace.xy).x; 
    
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	
	vec2 poissonDisk[8] = vec2[]
	(
		vec2( -0.94201624, -0.39906216 ),
		vec2( 0.94558609, -0.76890725 ),
		vec2( -0.094184101, -0.92938870 ),
		vec2( 0.34495938, 0.29387760 ),
		vec2( 0.2, 0.3 ),
		vec2( 0.6, 0.65 ),
		vec2( -0.4, 0.85 ),
		vec2( -0.8, 0.33 )
	);
	
	float shadow = 0.0;
	
	for (int i=0;i<8;i++)
	{
		float closestDepth = texture(uTextureShadowMap, FragPosLightSpace.xy + poissonDisk[i]/700.0).x; 
		if(currentDepth - bias > closestDepth)
		{
			shadow += 0.125;
		}
	}
    return shadow;
}

const float fogDensity = 0.091;
const float fogGradient = 10.0;

vec3 mixFixed(vec3 v1, vec3 v2,float a)
{
    vec3 result;
    result.x = v1.x * v1.x * (1 - a) + v2.x * v2.x * a;
    result.y = v1.y * v1.y  * (1 - a) + v2.y * v2.y * a;
    result.z = v1.z * v1.z  * (1 - a) + v2.z * v2.z * a;

    result.x = sqrt(result.x);
    result.y = sqrt(result.y);
    result.z = sqrt(result.z);
   
    return result;
}

void main()
{
	vec3 norm;
	vec3 viewDir;

	
	if(uMaterial.UseNormalMap)
	{
		norm = texture(uMaterial.TextureNormalMap, TexCoord).rgb;
		norm = normalize(norm * 2.0 - 1.0);
		viewDir = TBN * normalize(uCameraPosition - FragWorldPos);
	}
	else
	{
		norm = normalize(Normal);
		viewDir = normalize(uCameraPosition - FragWorldPos);	
	}
	
	float shadow = ShadowCalculation(ShadowCoord, norm);  
	
	vec3 result = CalculateDirectionalLight(uDirectionalLight,norm,viewDir,shadow);
	
	//float visibility = 1.0;
	//float temp = clamp(dot(norm,uDirectionalLight.Direction),0.0,1.0);
	//
	//float bias = 0.005 * tan(acos(temp));
	//bias = clamp(bias,0.0,0.01);
	//
	//vec2 poissonDisk[4] = vec2[](
	//	vec2( -0.94201624, -0.39906216 ),
	//	vec2( 0.94558609, -0.76890725 ),
	//	vec2( -0.094184101, -0.92938870 ),
	//	vec2( 0.34495938, 0.29387760 )
	//);
	//
	//for (int i=0;i<4;i++)
	//{
	//	
	//	if ( texture( uTextureShadowMap, ShadowCoord.xy + poissonDisk[i]/700.0 ).x  <  ShadowCoord.z-bias )
	//	{
	//		visibility-=0.2;
	//	}
	//}
	//result *= visibility;
	
	for(int i=0; i < uNumPointLights; i++)
	{
		result += CalculatePointLight(uPointLights[i],norm,FragWorldPos,viewDir,shadow);
	}
	
	vec3 uFogColor = vec3(0.101, 0.105, 0.109);
	//vec3 uFogColor = vec3(1.0, 1.0, 1.0);
	// apply fog effect

	float distanceFromCamera = length(FragWorldPos.xyz);
	float FogVisibility = exp(-pow((distanceFromCamera * fogDensity),fogGradient));
	FogVisibility = clamp(FogVisibility,0.0,1.0);
	result = mixFixed(uFogColor,result,FogVisibility);
	
	FragColor = vec4(result,1.0);
}