#version 330 core

uniform struct Player {
	 float pos_x;
	 float pos_z;
	 float rad;
	 vec3 trailColor;
} players[4];
uniform int num_players;
in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;
void main(void)
{
	vec4 color = vec4 (0.0f, 0.0f, 0.0f, 0.0f);
	for (int i=0; i<num_players; i++){
		float in_circle = (IN.texCoord.x-players[i].pos_x)*(IN.texCoord.x-players[i].pos_x) + (IN.texCoord.y-players[i].pos_z)*(IN.texCoord.y-players[i].pos_z);
		if (in_circle<(players[i].rad*players[i].rad) ){
			color = vec4 (players[i].trailColor, 1.0f);
		}
	}
	OutFrag = color;
}