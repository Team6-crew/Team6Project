#version 330 core
#define PI 3.14159265358979323846f
uniform sampler2D uDiffuseTex;
uniform float angle;
uniform vec4 player_colour;

uniform int num_players;
uniform int winning;
in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;



void main(void)
{
	vec4 color = vec4 (0.0f, 0.0f, 0.0f, 0.0f);
	int wr = -1;

		float in_circle = (IN.texCoord.x-0.5)*(IN.texCoord.x-0.5) + (IN.texCoord.y-0.5)*(IN.texCoord.y-0.5);
			if (in_circle<(0.5*0.5) ){
			
			vec2 test = vec2(IN.texCoord.x-0.5, IN.texCoord.y-0.5);

			float Angle = atan((IN.texCoord.y-0.5),(IN.texCoord.x-0.5))-PI/2;
			if (Angle<0) Angle+=2*PI;
			Angle = 2*PI - Angle;

			for (int i=0; i<num_players-1; i++){	
				if(wr==-1 && Angle <= players[i].angle){
					wr = i;
				}
			}
			if(wr==-1)
				wr = num_players-1;
		}
	color = players[wr].player_colour;
	if (winning == wr){
		color.x*= 0.7f;
		color.y*= 0.7f;
		color.z*= 0.7f;
	}
	OutFrag = color;
}