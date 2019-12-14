#version 430 core

in vec2 vUV;
in float colorChange;

uniform sampler2D Texture;
out vec4 fColor;

void main()
{ 
	
	fColor = texture2D(Texture,vUV);
	if(colorChange==0)
	fColor = fColor-vec4(0.5,0.5,0.5,0);
	else
	fColor = fColor-vec4(colorChange,colorChange,colorChange,0);
	
}
