#version 400

layout(location = 0) in vec3 VertexLocation;

uniform mat4 model_transform;
uniform mat4 view_transform;
uniform mat4 projection_transform;

void main()
{			
	gl_Position = projection_transform * view_transform * model_transform * vec4(VertexLocation, 1.0);
}