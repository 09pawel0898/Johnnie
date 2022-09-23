#shader vertex

#version 460 core
layout (location = 0) in vec3 aPosition;

uniform mat4 uViewMat;
uniform mat4 uProjMat;
uniform mat4 uModelMat;


void main()
{
	gl_Position = uProjMat * uViewMat * uModelMat * vec4(aPosition,1.0);
}

#shader fragment

#version 460 core

struct Material
{
	vec3 BaseColor;
};

out vec4 FragColor;
uniform Material uMaterial;

void main()
{
	FragColor = vec4(uMaterial.BaseColor,1.0);
}