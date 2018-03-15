#version 330 core
uniform int num_objects;

uniform struct Player {
	 float pos_x;
	 float pos_z;
	 vec2 halfdims;
	 vec4 objColor;
} objects[50];

in Vertex	{
	vec2 texCoord;
} IN;
out vec4 OutFrag;
void main(void)
{
	vec4 color = vec4 (0.0f, 0.0f, 0.0f, 0.0f);
	for (int i=0; i<num_objects; i++){
		if (abs(IN.texCoord.x-objects[i].pos_x)<objects[i].halfdims.x && abs(IN.texCoord.y-objects[i].pos_z)<objects[i].halfdims.y){
			color = objects[i].objColor;
		}

	}
	OutFrag = color;
}