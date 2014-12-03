#version 440

// simple4bird.vert

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec3 vNormal;
layout ( location = 2 ) in vec2 vTexCoord;


uniform cameraMat
{
  mat4 ModelViewMatrix;
  mat4 ModelViewProjectionMatrix;
  mat3 NormalMatrix;
};

out vec3 Normal;
out vec2 TexCoord;

void main( void )
{
  gl_Position = ModelViewProjectionMatrix * vec4( vPosition, 1.0 );

  Normal   = normalize( NormalMatrix * vNormal );
  TexCoord = vTexCoord;
}
