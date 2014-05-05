 #version 440

// texture_sample.frag
in vec3 originalPos;
in vec3 originalNorm;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

// 元の値
in vec3 v10; //vu U軸単位ベクトルに対応したベクトル
in vec3 v20; //vv V軸単位ベクトルに対応したベクトル
in vec3 v0c;

// 座標変換後
in vec3 v10x; //vu U軸単位ベクトルに対応したベクトル
in vec3 v20x; //vv V軸単位ベクトルに対応したベクトル
in vec3 v0cx;


//in vec3 spherePos;

uniform sampler2D Tex1;
uniform vec3 camera;
uniform vec3 spherePos;

// for debug //--------------------------------------------------
uniform cameraMat
{
  mat4 ModelViewMatrix;
  mat4 ModelViewProjectionMatrix;
  mat3 NormalMatrix;
  //mat4 ProjectionMatrix;
};

uniform mat4 MVI;

float nRatio = 1.491;


   
//---------------------------------------------------------------

struct LightInfo
{
  vec4 Position;
  vec3 Intensity;
};
uniform LightInfo Light;

struct MaterialInfo
{
  vec3 Ka;
  vec3 Kd;
  vec3 Ks;
  float Shininess;
};
uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;


// 最小二乗法
void LeastSquaresMethod(vec3 _v30/*ベクトルの向きに注意*/, vec3 _v10, vec3 _v20, out float _a, out float _b)
{
  float adbc = dot(_v10,_v10)*dot(_v20,_v20) - dot(_v10,_v20)*dot(_v20,_v10);
  _a = ( dot(_v20,_v20)*dot(_v30,_v10)-dot(_v20,_v10)*dot(_v30,_v20) ) / adbc;
  _b = ( dot(_v10,_v10)*dot(_v30,_v20)-dot(_v10,_v20)*dot(_v30,_v10) ) / adbc;
}

void calcLine_Sphere_CrossPointPlus( vec3 v/*LineDir*/, vec3 x/*a point on the line*/,
				vec3 c/*center of sphere*/, float r/*radius of sphere*/, out vec3 cPoint )
{
  float D = pow( dot(v,x-c), 2.0) - dot(v,v) * ( dot(x-c,x-c) - r*r );
  float t = ( -dot(v,x-c) + sqrt(D) ) / dot(v,v);

  cPoint = x + t*v;
}


void calcLine_Sphere_CrossPointMinus( vec3 v/*LineDir*/, vec3 x/*a point on the line*/,
				vec3 c/*center of sphere*/, float r/*radius of sphere*/, out vec3 cPoint )
{
  float D = pow( dot(v,x-c), 2.0) - dot(v,v) * ( dot(x-c,x-c) - r*r );
  float t = ( -dot(v,x-c) - sqrt(D) ) / dot(v,v);

  cPoint = x + t*v;
}


void calcLine_Plane_CrossPoint( vec3 n/*planeNormal*/, vec3 x/*pointOnPlane*/,
				vec3 p/*pointOnline*/, vec3 v/*lineDir*/, out vec3 cPoint )
{
  n = normalize(n);
  v = normalize(v);
  cPoint = v * ( dot(n,x) - dot(n,p) ) / dot(n,v) + p;
}


void calcTexCoordForPupil( vec3 pos, vec3 norm, float radius, out vec2 alteredTexCoord )
{
  // オリジナルを使うときの処理
  
  vec3 centerSphere = vec3(0.0f,0.0f,-radius);
  vec3 incident = normalize( pos - camera );

  // 球と視線ベクトルの交差判定（負なら交わらない）
  float judge = pow( dot(incident, camera-centerSphere),2.0 )
    - dot(incident,incident)*( dot(camera-centerSphere,camera-centerSphere) - radius*radius );

  
  vec3 finalCrossPoint; // 仮想平面との最終交点
  
  if ( judge >= 0.0 )
  {
    vec3 firstCrossPoint;
    calcLine_Sphere_CrossPointMinus(incident, camera, centerSphere, radius, firstCrossPoint);

    vec3 refrac0 = refract( incident, normalize(firstCrossPoint - centerSphere) , 1.0/nRatio );
    vec3 secondCrossPoint;
    calcLine_Sphere_CrossPointPlus (refrac0, firstCrossPoint, centerSphere, radius, secondCrossPoint);

    // 相対屈折率<1.0となる場合について全反射を考慮?
    vec3 refrac1 = refract( refrac0, normalize(secondCrossPoint - centerSphere), nRatio );

    calcLine_Plane_CrossPoint( norm, pos - 2*radius*normalize(norm), secondCrossPoint, refrac1, finalCrossPoint );
  }
  else
  {
    // 視線と仮想平面の交点を直接求める。
    calcLine_Plane_CrossPoint( norm, pos - 2*radius*normalize(norm), pos, incident, finalCrossPoint );
  }
    
  // x,y,z→u,v
  //vec3 v3 = pos;
  vec3 v3 = finalCrossPoint + 2*radius * normalize(norm);
  //vec3( MVI * vec4( finalCrossPoint + 2*radius * normalize(norm), 1.0 ) );
  vec3 v30 = v3-v0c;

  float adbc = dot(v10,v10)*dot(v20,v20) - dot(v10,v20)*dot(v20,v10);
  alteredTexCoord.x = ( dot(v20,v20)*dot(v30,v10)-dot(v20,v10)*dot(v30,v20) ) / adbc;
  alteredTexCoord.y = ( dot(v10,v10)*dot(v30,v20)-dot(v10,v20)*dot(v30,v10) ) / adbc;  
  
}

// 複数の場合
void calcTexCoordForPupil_plural( vec3 pos, vec3 norm, float radius, out vec2 alteredTexCoord )
{
  // オリジナルを使うときの処理
  // ------ここからミスありそう---------------------------------------------------------------
  vec2 cef; // 係数
  float offset = 2*radius; // 球同士の距離
  LeastSquaresMethod( v0c-pos,v10*offset,v20*offset,cef.x,cef.y );
  float a = floor(cef.x);
  float b = floor(cef.y);
  vec3 nearestSphere = v0c + v10 *offset*( 0.5 + a ) + v20 *offset*( 0.5 + b ) - normalize(norm)*radius;
   
  // -----------------------------------------------------------------------------------------
  
  vec3 centerSphere = nearestSphere;
  vec3 incident = normalize( pos - camera );

  // 球と視線ベクトルの交差判定（負なら交わらない）
  float judge = pow( dot(incident, camera-centerSphere),2.0 )
    - dot(incident,incident)*( dot(camera-centerSphere,camera-centerSphere) - radius*radius );

  
  vec3 finalCrossPoint; // 仮想平面との最終交点
  
  if ( judge >= 0.0 )
  {
    vec3 firstCrossPoint;
    calcLine_Sphere_CrossPointMinus(incident, camera, centerSphere, radius, firstCrossPoint);

    vec3 refrac0 = refract( incident, normalize(firstCrossPoint - centerSphere) , 1.0/nRatio );
    vec3 secondCrossPoint;
    calcLine_Sphere_CrossPointPlus (refrac0, firstCrossPoint, centerSphere, radius, secondCrossPoint);

    // 相対屈折率<1.0となる場合について全反射を考慮?
    vec3 refrac1 = refract( refrac0, normalize(secondCrossPoint - centerSphere), nRatio );

    calcLine_Plane_CrossPoint( norm, pos - 2*radius*normalize(norm), secondCrossPoint, refrac1, finalCrossPoint );
  }
  else
  {
    // 視線と仮想平面の交点を直接求める。
    calcLine_Plane_CrossPoint( norm, pos - 2*radius*normalize(norm), pos, incident, finalCrossPoint );
  }
 
  // x,y,z→u,v
  //vec3 v3 = pos;
  vec3 v3 = finalCrossPoint + 2*radius * normalize(norm);
  //vec3( MVI * vec4( finalCrossPoint + 2*radius * normalize(norm), 1.0 ) );
  vec3 v30 = v3-v0c;

  float adbc = dot(v10,v10)*dot(v20,v20) - dot(v10,v20)*dot(v20,v10);
  alteredTexCoord.x = ( dot(v20,v20)*dot(v30,v10)-dot(v20,v10)*dot(v30,v20) ) / adbc;
  alteredTexCoord.y = ( dot(v10,v10)*dot(v30,v20)-dot(v10,v20)*dot(v30,v10) ) / adbc;

  //alteredTexCoord = nearestSphere.xy;
  
}



void phongModel( vec3 pos, vec3 norm, out vec3 ambAndDiff, out vec3 spec )
{
  vec3 s = normalize( vec3( Light.Position ) - pos );
  vec3 v = normalize( -pos.xyz );
  vec3 r = reflect( -s, norm );
  vec3 ambient = Light.Intensity * Material.Ka;
  float sDotN = max( dot(s,norm), 0.0 );
  vec3 diffuse = Light.Intensity * Material.Kd * sDotN;
  spec = vec3(0.0);
  if( sDotN > 0.0 )
    spec = Light.Intensity * Material.Ks *
               pow( max( dot(r,v), 0.0 ), Material.Shininess );
  //spec = Light.Intensity * Material.Ks * pow( max( dot(r,v), 0.0 ), Material.Shininess );

  ambAndDiff = ambient + diffuse;
}

void main()
{
  //vec3 ambAndDiff, spec;
  //phongModel( Position, Normal, ambAndDiff, spec );
  vec2 altTexCoord;
  //calcTexCoordForPupil( originalPos, originalNorm, 5.0, altTexCoord ); // radiusの値に気をつける
  calcTexCoordForPupil_plural( originalPos, originalNorm, 0.5, altTexCoord ); // radiusの値に気をつける
  //calcTexCoordForPupil2( Position, Normal, 5.0, altTexCoord );
  vec4 texColor = texture( Tex1, altTexCoord );
  
  FragColor = 0.7*texColor;
//FragColor = vec4(camera.xyz,1.0);
  //FragColor = (vec4( ambAndDiff, 1.0 ) * texColor) + vec4(spec, 1.0);
}
  
 
 
