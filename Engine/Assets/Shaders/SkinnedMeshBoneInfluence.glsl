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

void main()
{
	gl_Position = uProjMat * uViewMat * uModelMat * vec4(aPosition,1.0);

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
		//if(BoneIDs[i] == 15)
		//{
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
		//}
		
	}
	
	if(!found)
	{
		FragColor = vec4(0.0,0.0,1.0,1.0);
	}
}