#shader vertex

#version 460 core
layout (location = 0) in vec3 aPosition;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(aPosition,1.0);
}

#shader fragment

#version 460 core


out vec4 FragColor;
uniform vec3 uLightColor;

void main()
{
	vec3 a = vec3(1.0,1.0,1.0);
	FragColor = vec4(uLightColor,1.0);
}