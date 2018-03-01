#version 330 core

uniform float radius_perc;
uniform vec4 playerColor;
in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;

vec3 drip[11];

void main(void)
{
	float seed = (playerColor.x+playerColor.y+playerColor.z)/100.0f; //0.0169, 0.023, 0.021 , 0.012
	
	drip [0] = vec3 (0.03f, 0.03f, seed);
	drip [1] = vec3 (0.07f, 0.04f, 0.03f-seed);
	drip [2] = vec3 (0.14f, 0.05f, 0.00023f/seed);
	drip [3] = vec3 (0.16f, 0.01f, 0.025f-seed);
	drip [4] = vec3 (0.21f, 0.04f, seed - 0.005f);
	drip [5] = vec3 (0.26f, 0.03f, 0.00031f/seed);
	drip [6] = vec3 (0.30f, 0.02f, seed + 0.008f);
	drip [7] = vec3 (0.34f, 0.03f, seed * 1.3f);
	drip [8] = vec3 (0.39f, 0.02f, 0.038f - seed);
	drip [9] = vec3 (0.42f, 0.03f, 0.00027f/seed);
	drip [10] = vec3 (0.46f, 0.04f, 0.035f);
	
	vec4 color = vec4 (0.0f, 0.0f, 0.0f, 0.0f);
	
	
	if (IN.texCoord.x>0.5 && IN.texCoord.y<0.5 ){
		color = playerColor;
	}
	else if(IN.texCoord.x<0.5 && IN.texCoord.y>0.5 && radius_perc >98.0f){
		color = playerColor;
	}
	else if(IN.texCoord.x<0.5 && IN.texCoord.y<0.5){
		for (int i=0; i<11; i++){
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
	if(color.a>0.0f){
		color.a = 0.95f;
	}
	OutFrag = color;
}