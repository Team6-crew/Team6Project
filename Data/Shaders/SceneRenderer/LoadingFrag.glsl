#version 330 core
uniform sampler2D uDiffuseTex;
uniform float radius_perc;
in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;

vec3 drip[11];

void main(void)
{
	float seed = (1.69f)/100.0f; //0.0169, 0.023, 0.021 , 0.012
	
	drip [0] = vec3 (0.06f, 0.014f, seed);
	drip [1] = vec3 (0.14f, 0.01f, 0.03f-seed);
	drip [2] = vec3 (0.28f, 0.012f, 0.00023f/seed);
	drip [3] = vec3 (0.32f, 0.013f, 0.025f-seed);
	drip [4] = vec3 (0.42f, 0.013f, seed - 0.005f);
	drip [5] = vec3 (0.52f, 0.014f, 0.00031f/seed);
	drip [6] = vec3 (0.60f, 0.002f, seed + 0.008f);
	drip [7] = vec3 (0.68f, 0.017f, seed * 1.3f);
	drip [8] = vec3 (0.78f, 0.013f, 0.038f - seed);
	drip [9] = vec3 (0.84f, 0.016f, 0.00027f/seed);
	drip [10] = vec3 (0.92f, 0.015f, 0.035f);
	
	vec4 color = vec4 (0.0f, 0.0f, 0.0f, 0.0f);
	
	vec4 playerColor = color;
	
	if(IN.texCoord.x<0.25) playerColor = vec4(1.0f, 0.0f, 0.69f, 1.0f);
	else if(IN.texCoord.x<0.5)playerColor = vec4(0.3f, 1.0f, 1.0f, 1.0f);
	else if(IN.texCoord.x<0.75)playerColor = vec4(1.0f, 0.68f, 0.33f, 1.0f);
	else playerColor = vec4(0.0f, 1.0f, 0.02f, 1.0f);
	
	for (int i=0; i<11; i++){
		drip[i].z = drip[i].z*2;
		if (IN.texCoord.x>(drip[i].x-drip[i].y) && IN.texCoord.x<(drip[i].x+drip[i].y) && IN.texCoord.y > (1.0-drip[i].z*radius_perc)){
			color = playerColor;
		}
		else{
		float in_circle = (IN.texCoord.x-drip[i].x)*(IN.texCoord.x-drip[i].x) + (IN.texCoord.y-(1.0-drip[i].z*radius_perc))*(IN.texCoord.y-(1.0-drip[i].z*radius_perc));
			if (in_circle < drip[i].y*drip[i].y ){
				color = playerColor;
			}
		}
	}
	vec4 texColor 	= texture(uDiffuseTex, IN.texCoord);
	
	if(texColor == vec4 (1.0f, 1.0f ,1.0f,1.0f)){
		color = texColor;
	}
	OutFrag = color;
}