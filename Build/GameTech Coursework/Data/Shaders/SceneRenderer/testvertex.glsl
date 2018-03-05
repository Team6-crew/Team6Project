#version 330 core

in vec3 position;
in  vec2 texCoord;

out Vertex	{
	vec2 texCoord;
} OUT;

void main(void)	{
	OUT.texCoord = texCoord;
	gl_Position = vec4(position.x,-position.y, position.z, 1.0); //Fix nclgl quad being the wrong way around!
}