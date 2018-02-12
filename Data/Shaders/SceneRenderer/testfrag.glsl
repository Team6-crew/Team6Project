#version 330 core

uniform uint uPickerIdx;
uniform float perc_x;
uniform float perc_z;
uniform sampler2D DiffuseTex;
uniform vec3 trailColor;
in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;
void main(void)
{
	
	vec4 color = vec4 (1.0f, 1.0f, 1.0f, 0.0f);
	
	if ((IN.texCoord.x<perc_x+0.01 && IN.texCoord.x>perc_x-0.01) && (IN.texCoord.y<perc_z+0.01 && IN.texCoord.y>perc_z-0.01) ){
		color = vec4 (trailColor, 1.0f);
	}
	


	OutFrag = color;
}