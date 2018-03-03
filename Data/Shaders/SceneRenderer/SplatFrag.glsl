#version 330 core
#define PI 3.14159265358979323846f
uniform sampler2D uDiffuseTex;

uniform	float pos_x;
uniform	float pos_z;
uniform	float rad;
uniform	vec3 trailColor;


in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;
void main(void)
{
	vec4 color = vec4 (0.0f, 0.0f, 0.0f, 0.0f);
	if (abs(IN.texCoord.x - pos_x)<rad&&abs(IN.texCoord.y - pos_z)<rad){
		vec2 splatCoord = vec2 ((IN.texCoord.x - pos_x+rad)/(2.0f*rad), (IN.texCoord.y - pos_z+rad)/(2.0f*rad));
		color = texture(uDiffuseTex, splatCoord);
	}
	if (color != vec4 (0.0f, 0.0f, 0.0f, 0.0f)){
		color.xyz = trailColor.xyz;
		color.a = 1.0f;
	}
	
	OutFrag = color;
}