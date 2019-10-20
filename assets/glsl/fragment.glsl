#version 150 core
uniform sampler2D tex;

in vec3 vertexColor;
in vec2 vertexTexcoord;

out vec4 outColor;

void main()
{
	outColor = vec4(vertexColor, 1.0);
	// outColor = texture(tex, vertexTexcoord);
}
