#version 330 core

uniform float radius_perc;
uniform vec4 playerColor;
in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;

vec3 drip[5];

void main(void)
{
	drip [0] = vec3 (0.07f, 0.07f, 0.01f);
	drip [1] = vec3 (0.17f, 0.04f, 0.02f);
	drip [2] = vec3 (0.25f, 0.05f, 0.025f);
	drip [3] = vec3 (0.34f, 0.06f, 0.015f);
	drip [4] = vec3 (0.46f, 0.08f, 0.03f);
	vec4 color = vec4 (0.0f, 0.0f, 0.0f, 0.0f);
	if (IN.texCoord.x>0.5 && IN.texCoord.y<0.5 ){
		color = playerColor;
	}
	else if(IN.texCoord.x<0.5 && IN.texCoord.y>0.5 && radius_perc >98.0f){
		color = playerColor;
	}
	else if(IN.texCoord.x<0.5 && IN.texCoord.y<0.5){
		for (int i=0; i<5; i++){
			if (IN.texCoord.x>(drip[i].x-drip[i].y) && IN.texCoord.x<(drip[i].x+drip[i].y) && IN.texCoord.y > (0.5-drip[i].z*radius_perc)){
				color = playerColor;
			}
			else{
				float in_circle = (IN.texCoord.x-drip[i].x)*(IN.texCoord.x-drip[i].x) + (IN.texCoord.y-(0.5-drip[i].z*radius_perc))*(IN.texCoord.y-(0.5-drip[i].z*radius_perc));
				if (in_circle < drip[i].y*drip[i].y ){
					color = playerColor;
				}
			}
		}
	}
	
	OutFrag = color;
}