#shader vertex

#version 460 core
layout (location = 0) in vec3 aPosition;

uniform mat4 uProjMat;
uniform mat4 uViewMat;
uniform mat4 uModelMat;

void main()
{
	gl_Position = uProjMat * uViewMat * uModelMat * vec4(aPosition,1.0);
}

#shader fragment

#version 460 core

out vec4 FragColor;

void main()
{

	FragColor = vec4(1.0,1.0,1.0,1.0);
}