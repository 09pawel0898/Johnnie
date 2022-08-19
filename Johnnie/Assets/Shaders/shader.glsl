#shader vertex

#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertColor = aColor;
	TexCoord = aTexCoord;
}

#shader fragment

#version 330 core
out vec4 FragColor;

in vec3 vertColor;
in vec2 TexCoord;

uniform sampler2D tex;

void main()
{
	FragColor = texture(tex,TexCoord);
}