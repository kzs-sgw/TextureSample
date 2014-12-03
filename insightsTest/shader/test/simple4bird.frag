#version 440

// simple4bird.frag

in vec3 Normal;
in vec2 TexCoord;

layout ( location = 0 ) out vec4 FragColor;

void main( void )
{
  FragColor = vec4( 0.0, 0.0, 1.0, 1.0 );
}
