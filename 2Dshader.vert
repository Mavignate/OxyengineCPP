#version 330 core

in vec3 Position; 
in vec4 Color;
in vec2 Texture;

out vec4 color;
out vec2 TexCoord;
uniform mat4 transform;
uniform mat4 camera;
void main() 
{ 
	gl_Position = camera* transform * vec4( Position.x, Position.y, 0, 1 ); 
	color = Color;
	TexCoord = Texture;
}