#version 400

uniform int id;

out vec4 FragColor;

void main()
{	
	FragColor = vec4(id / 255.0, id / 255.0, id / 255.0, 0.0);
}