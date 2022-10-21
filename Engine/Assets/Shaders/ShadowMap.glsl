#shader vertex

#version 460 core

layout(location = 0) in vec3 aPos;

uniform mat4 uDepthMVP;

void main()
{
	gl_Position =  uDepthMVP * vec4(aPos,1);
}

#shader fragment

#version 460 core

layout(location = 0) out float fragDepth;

void main()
{
    fragDepth = gl_FragCoord.z;
}