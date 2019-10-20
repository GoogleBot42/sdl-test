#version 150 core

uniform mat4 mvp;

in vec3 position;
in vec3 color;
in vec2 texcoord;

out vec2 vertexTexcoord;
out vec3 vertexColor;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);
	vertexColor = color;
	vertexTexcoord = texcoord;
}
