#version 430 core

in vec2 vUV;

uniform sampler2D Texture;

out vec4 fColor;

void main()
{
	
	fColor = texture2D(Texture,vUV);
	fColor=fColor-vec4(0.5,0.5,0.5,0);
	
}
