 #shader vertex

#version 460 core

layout(location = 0) in vec3 aPos;

layout (location = 4) in ivec4 aBoneIDs;
layout (location = 5) in vec4 aWeights;

uniform mat4 uDepthMVP;

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
		
		vec4 TransformedPosition = BoneTransform * vec4(aPos,1.0);
		gl_Position = uDepthMVP * TransformedPosition;
	}
	else
	{
		gl_Position =  uDepthMVP * vec4(aPos,1);
	}
}

#shader fragment

#version 460 core

void main()
{
}