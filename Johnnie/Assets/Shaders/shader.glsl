#shader vertex

#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(aPos,1.0);
	vertColor = aColor;
	TexCoord = aTexCoord;
}

#shader fragment

#version 460 core
out vec4 FragColor;

in vec3 vertColor;
in vec2 TexCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	FragColor = texture(tex1, TexCoord);
}