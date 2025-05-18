#version 330 core

in vec3 ourColor;
in vec2 ourUV;

uniform sampler2D sampler;

out vec4 FragColor;

void main()
{
   FragColor = texture(sampler, ourUV);
}