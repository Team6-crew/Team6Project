#version 330 core
#define PI 3.14159265358979323846f
uniform uint uPickerIdx;
uniform struct Player {
	 float angle;
} players[4];
uniform int num_players;
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

			float Angle = atan((IN.texCoord.y-0.5),(IN.texCoord.x-0.5))+PI;
			for (int i=0; i<num_players-1; i++){	
				if(wr==-1 && Angle <= players[i].angle){
					wr = i;
				}
			}
			if(wr==-1)
				wr = num_players-1;
		}
	if (wr ==0) color = vec4(1.0f, 0.0f, 0.69f, 1.0f);
	else if(wr == 1)color = vec4(0.3f, 1.0f, 1.0f, 1.0f);
	else if(wr == 2)color = vec4(1.0f, 0.68f, 0.33f, 1.0f);
	else if(wr == 3)color = vec4(0.0f, 1.0f, 0.02f, 1.0f);
	OutFrag = color;
}