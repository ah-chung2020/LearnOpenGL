#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

uniform mat4 transform;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 ourColor;
out vec2 ourUV;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * transform * vec4(aPos, 1.0);
	ourColor = aColor;
	ourUV = aUV;
}