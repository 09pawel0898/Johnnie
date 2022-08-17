#shader vertex

#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertColor;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertColor = aColor;
}

#shader fragment

#version 330 core
out vec4 FragColor;
in vec3 vertColor;

void main()
{
	FragColor = vec4(vertColor, 1.0f);
}