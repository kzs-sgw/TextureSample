#version 440

// simple.vert

layout ( location = 0 ) in vec2 vPosition;
layout ( location = 1 ) in vec3 vNormal;
layout ( location = 2 ) in vec3 vColor;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;
uniform mat3 NormalMatrix;

out vec3 Color;

void main( void )
{
  gl_Position = ModelViewProjectionMatrix * vec4( vPosition, 0.0, 1.0 );
  Color = vColor;
}
