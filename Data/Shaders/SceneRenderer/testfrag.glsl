#version 330 core

uniform uint uPickerIdx;
uniform float perc_x;
uniform float perc_z;
uniform float rad;
uniform sampler2D DiffuseTex;
uniform vec3 trailColor;
in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;
void main(void)
{
	
	vec4 color = vec4 (1.0f, 1.0f, 1.0f, 0.0f);
	float in_circle = (IN.texCoord.x-perc_x)*(IN.texCoord.x-perc_x) + (IN.texCoord.y-perc_z)*(IN.texCoord.y-perc_z);
	if (in_circle<(rad*rad) ){
		color = vec4 (trailColor, 1.0f);
	}
	OutFrag = color;
}