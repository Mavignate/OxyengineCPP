#version 330 core

out vec4 LFragment;

in vec4 color;
in vec2 TexCoord;
uniform sampler2D ourTexture;
 void main()
{
       LFragment = texture(ourTexture, TexCoord);
}