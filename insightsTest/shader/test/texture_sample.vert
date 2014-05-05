#version 440

// texture_sample.vert

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec3 vNormal;
layout ( location = 2 ) in vec2 vTexCoord;

layout ( location = 3 ) in vec3 v0;
layout ( location = 4 ) in vec3 v1;
layout ( location = 5 ) in vec3 v2;

out vec3 originalPos;
out vec3 originalNorm;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;


out vec3 v10;
out vec3 v20;
out vec3 v0c;

out vec3 v10x;
out vec3 v20x;
out vec3 v0cx;

uniform cameraMat
{
  mat4 ModelViewMatrix;
  mat4 ModelViewProjectionMatrix;
  mat3 NormalMatrix;
  //mat4 ProjectionMatrix;
};

uniform vec3 camera;

//uniform mat4 MVI; //ModelViewInverseMatrix


void main()
{
  originalPos  = vPosition;
  originalNorm = vNormal;
  
  TexCoord = vTexCoord;
  Normal   = normalize( NormalMatrix * vNormal );
  Position = vec3( ModelViewMatrix * vec4( vPosition, 1.0 ) );
  
  
  v0c = v0;
  v10 = v1;
  v20 = v2;

  v0cx = vec3(ModelViewMatrix *vec4(v0,1.0));
  v10x = NormalMatrix * v1;
  v20x = NormalMatrix * v2;

  gl_Position = ModelViewProjectionMatrix * vec4( vPosition, 1.0 );
}
