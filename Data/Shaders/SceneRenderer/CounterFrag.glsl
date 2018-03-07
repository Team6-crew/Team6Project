#version 330 core

uniform sampler2D DiffuseTex;
uniform float seconds;
in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;
void main(void)
{
	float tempSec = seconds*3.0f;
	vec2 myCoords = (IN.texCoord)*tempSec + vec2(0.5f, 0.5f)*(1-tempSec);
	vec4 color  = texture(DiffuseTex, myCoords);
	OutFrag = color;
}