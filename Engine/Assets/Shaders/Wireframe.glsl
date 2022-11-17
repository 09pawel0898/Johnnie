#shader vertex

#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 4) in ivec4 aBoneIDs;
layout (location = 5) in vec4 aWeights;

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
	if(uIsSkinnedMesh)
	{
		mat4 BoneTransform = mat4(1.0);
	
		for(int idx = 0 ; idx < MAX_BONES_INFLUENCED_VERTEX ; idx++)
		{
			BoneTransform += uBones[aBoneIDs[idx]] * aWeights[idx];
		}
		
		vec4 TransformedPosition = BoneTransform * vec4(aPosition,1.0);
		gl_Position = uProjMat * uViewMat * uModelMat * TransformedPosition;
	}
	else
	{
		gl_Position = uProjMat * uViewMat * uModelMat * vec4(aPosition,1.0);
	}
}

#shader fragment

#version 460 core

out vec4 FragColor;

void main()
{

	FragColor = vec4(1.0,1.0,1.0,1.0);
}