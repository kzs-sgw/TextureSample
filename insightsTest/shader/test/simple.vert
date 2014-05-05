#version 440

// simple.vert

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec3 vColor;

uniform cameraMat
{
  mat4 ModelViewMatrix;
  mat4 ModelViewProjectionMatrix;
  mat3 NormalMatrix;
};

out vec3 Color;

void main( void )
{
  gl_Position = ModelViewProjectionMatrix * vec4( vPosition, 1.0 );
  Color = vColor;
}
