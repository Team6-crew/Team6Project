#version 330 core

uniform sampler2D DiffuseTex;

in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;
void main(void)
{

	vec4 color  = texture(DiffuseTex, IN.texCoord);
	OutFrag = color;
}