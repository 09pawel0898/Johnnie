#shader vertex

#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexUV;

out vec2 TexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(aPosition,1.0);
	TexCoord = aTexUV;
}

#shader fragment

#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform bool uUseTextures;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main()
{
	if(uUseTextures)
	{
		FragColor = texture(texture_diffuse1, TexCoord);
	}
	else
	{
		FragColor = vec4(1.0,1.0,1.0,1.0);
	}
}