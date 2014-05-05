#version 120
float m = 0.15;
float refrac = 200;

varying vec4 vPos;
varying vec3 vNrm;

void main( void )
{
  vec3 La = gl_LightSource[ 0 ].ambient.xyz;
  vec3 Ld = gl_LightSource[ 0 ].diffuse.xyz;
  vec3 Ls = gl_LightSource[ 0 ].specular.xyz;
  vec3 Lp = gl_LightSource[ 0 ].position.xyz;

  vec3 Ke = gl_FrontMaterial.emission.xyz;
  vec3 Ka = gl_FrontMaterial.ambient.xyz;
  vec3 Kd = gl_FrontMaterial.diffuse.xyz;
  vec3 Ks = gl_FrontMaterial.specular.xyz;
  float shine = gl_FrontMaterial.shininess;

  vec3 view    = -normalize( vPos.xyz );
  vec3 nrm     =  normalize( vNrm );
  vec3 light   =  normalize( Lp - vPos.xyz );
  vec3 halfway =  normalize( light + view );


  vec3 emissive = Ke;

  vec3 ambient = Ka * La;

  float diffuseLight = max( dot ( light, nrm ), 0.0 );
  vec3  diffuse = Kd * Ld * diffuseLight;


  vec3 specular = vec3( 0.0 );
  if ( diffuseLight > 0.0 )
    {
      float NH = dot( nrm , halfway );
      float VH = dot( view, halfway );
      float NV = dot( nrm , view );
      float NL = dot( nrm , light );

      float alpha = acos( NH );

      float D = ( 1.0 / ( 4 * m * m * NH * NH * NH * NH ) ) * exp( ( NH * NH - 1 ) / ( m * m * NH * NH ) );

      float G = min( 1, min( ( 2 * NH * NV ) / VH, ( 2 * NH * NL ) / VH ) );


      float c = VH;
      float g = sqrt( refrac * refrac + c * c -1 );
      float F = ((g-c)*(g-c)/((g+c)*(g+c)))*(1+(c*(g+c)-1)*(c*(g+c)-1)/((c*(g-c)-1)*(c*(g-c)-1)));
      

      float specularLight = D * G * F / NV;
      specular = Ks * Ls * specularLight;
    }

  gl_FragColor.xyz = emissive + ambient + diffuse + specular;
  gl_FragColor.w = 1.0;
}
