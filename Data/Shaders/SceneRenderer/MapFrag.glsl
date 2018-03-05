#version 330 core

uniform sampler2D uColorTex;
uniform sampler2D uColorTex2;
in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;
void main(void)
{
	vec4 outColor = vec4 (0.0f, 0.0f, 0.0f, 0.0f);
	vec4 color1 = texture(uColorTex2, IN.texCoord);
	vec4 color0 = texture(uColorTex, IN.texCoord);
	if(color1.a != 0.0f){
		outColor = color1;
	}
	else{
		outColor = color0;
	}
	if (outColor == vec4(1.0f, 1.0f, 1.0f, 1.0f)){
		outColor = vec4 (0.3f, 0.3f, 0.3f, 1.0f);
	}
	OutFrag = outColor;
}