#version 120

// cooktorrance.vert

varying vec4 vPos;
varying vec3 vNrm;



void main( void )
{
  vPos = gl_ModelViewMatrix * gl_Vertex;
  vNrm = normalize( gl_NormalMatrix * gl_Normal );
 
  
  gl_Position = ftransform();
}
