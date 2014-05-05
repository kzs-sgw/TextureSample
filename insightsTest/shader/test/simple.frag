#version 440

// simple.frag

in vec3 Color;

layout ( location = 0 ) out vec4 FragColor;

void main( void )
{
  FragColor = vec4( Color, 1.0 );
}
