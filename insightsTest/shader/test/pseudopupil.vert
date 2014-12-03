#version 440

// pseudopupil.vert

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec3 vNormal;
layout ( location = 2 ) in vec2 vTexCoord;

layout ( location = 3 ) in vec3 vc;
layout ( location = 4 ) in vec3 vu;
layout ( location = 5 ) in vec3 vv;

out vec3 originalPos;
out vec3 originalNorm; 

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

out vec3 v10;
out vec3 v20;
out vec3 v0c;


uniform cameraMat
{
  mat4 ModelViewMatrix;
  mat4 ModelViewProjectionMatrix;
  mat3 NormalMatrix; // DO NOT USE THIS
  //mat4 ProjectionMatrix;
};

uniform vec3 camera;


void main()
{
  originalPos  = vPosition;
  originalNorm = vNormal;
  
  TexCoord = vTexCoord;
  Normal   = normalize( mat3(ModelViewMatrix) * vNormal );
  Position = vec3( ModelViewMatrix * vec4( vPosition, 1.0 ) );
  
  
  v0c = vc;
  v10 = vu;
  v20 = vv;

  gl_Position = ModelViewProjectionMatrix * vec4( vPosition, 1.0 );
}
