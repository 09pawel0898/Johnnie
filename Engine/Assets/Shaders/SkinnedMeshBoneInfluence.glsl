#shader vertex

#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in ivec4 aBoneIDs;
layout (location = 5) in vec4 aWeights;

out vec3 FragWorldPos;
flat out ivec4 BoneIDs;
out vec4 Weights;

uniform mat4 uProjMat;
uniform mat4 uViewMat;
uniform mat4 uModelMat;


const int MAX_BONES = 200;
const int MAX_BONES_INFLUENCED_VERTEX = 4;

uniform bool uIsSkinnedMesh;
uniform mat4 uBones[MAX_BONES];
uniform mat4 uFixedScaleMatrix;

void main()
{
	mat4 BoneTransform = mat4(1.0);
	
	for(int idx = 0 ; idx < MAX_BONES_INFLUENCED_VERTEX ; idx++)
	{
		BoneTransform += uBones[aBoneIDs[idx]] * aWeights[idx];
	}
		
	vec4 TransformedPosition = BoneTransform * vec4(aPosition,1.0);
	gl_Position = uProjMat * uViewMat * uModelMat * TransformedPosition;
	
	FragWorldPos = vec3(uModelMat * vec4(aPosition,1.0));
	BoneIDs = aBoneIDs;
	Weights = aWeights;
}

#shader fragment

#version 460 core

out vec4 FragColor;
in vec3 FragWorldPos;
flat in ivec4 BoneIDs;
in vec4 Weights;

void main()
{
	bool found = false;
	
	for(int i = 0; i<4 ;i++)
	{
		if(Weights[i] == -1 || Weights[i] == 0.f)
		{
			break;
		}
		else if(Weights[i] >= 0.7)
		{
			FragColor = vec4(vec3(1.0,0.0,0.0) * Weights[i],1.0);
		}
		else if(Weights[i] >= 0.4 && Weights[i] <0.7)
		{
			FragColor = vec4(vec3(0.0,1.0,0.0) * Weights[i],1.0);
		}
		else if(Weights[i] >= 0.1)
		{
			FragColor = vec4(vec3(1.0,1.0,0.0) * Weights[i],1.0);
		}
		else if(Weights[i] < 0.1)
		{
			FragColor = vec4(vec3(0.0,1.0,1.0) * Weights[i],1.0);
		}
		found = true;
		return;	
	}
	
	if(!found)
	{
		FragColor = vec4(0.0,0.0,1.0,1.0);
	}
}