#version 330 core

uniform sampler2D DiffuseTex;
uniform vec4 playerColor;
uniform float perc;

in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;
void main(void)
{
    float y = perc;
	vec4 color  = texture(DiffuseTex, IN.texCoord);
	if((1-IN.texCoord.y) >= perc && color.a > 0.0f){
		color = playerColor;
	}
	if(color.a >0.0f){
		color.a = 0.8f;
	}
	OutFrag = color;
}