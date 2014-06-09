 #version 440

// texture_sample.frag
in vec3 originalPos;
in vec3 originalNorm;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

// 元の値
in vec3 v10; //vu U軸ベクトルに対応したベクトル(正規化していない)
in vec3 v20; //vv V軸ベクトルに対応したベクトル(正規化していない)
in vec3 v0c;

// 座標変換後
in vec3 v10x; //vu U軸ベクトルに対応したベクトル(正規化していない)
in vec3 v20x; //vv V軸ベクトルに対応したベクトル(正規化していない)
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


void Refract( vec3 I, vec3 N, float eta, out vec3 result )
{
  float k = 1.0 - eta*eta*( 1.0-dot(N,I) * dot(N,I) );
  if ( k < 0.0 ) // 全反射
    result = reflect(I,N);
  else
    result = eta * I - ( eta * dot(N,I) + sqrt(k) ) * N;
}


void IsCrossing( vec3 v/*LineDir*/, vec3 x/*a point on the line*/,
				vec3 c/*center of sphere*/, float r/*radius of sphere*/, out float _judge )
{
  _judge = pow( dot(v,x-c), 2.0) - dot(v,v) * ( dot(x-c,x-c) - r*r );
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
    vec3 refrac1 = refract( refrac0, normalize(centerSphere - secondCrossPoint), nRatio );
    //Refract( refrac0, normalize(centerSphere - secondCrossPoint), nRatio, refrac1 );

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
  vec3 v30 = v3 - v0c;

  float adbc = dot(v10,v10)*dot(v20,v20) - dot(v10,v20)*dot(v20,v10);
  alteredTexCoord.x = ( dot(v20,v20)*dot(v30,v10)-dot(v20,v10)*dot(v30,v20) ) / adbc;
  alteredTexCoord.y = ( dot(v10,v10)*dot(v30,v20)-dot(v10,v20)*dot(v30,v10) ) / adbc;  
  
}

// 複数の場合
void calcTexCoordForPupil_plural( vec3 pos, vec3 norm, float radius, out vec2 alteredTexCoord )
{
  vec3 incident = normalize( pos - camera );
  
  // オリジナルを使うときの処理
  // ------最初の球を見つける---------------------------------------------------------------
  vec3 vU = normalize( v10 );
  vec3 vV = normalize( v20 ); 
  vec2 cef; // 係数
  float offset = 2*radius; // 球同士の距離
  float pOffset = 0.01*radius; // 平面のオフセット
  float pDist = 0.0; // 仮想平面とポリゴンの距離
  vec3 posOnPlane; // 仮想平面と視線の交わる点
  vec3 originUV = pos - v10*TexCoord.x - v20*TexCoord.y; // UV座標の原点に対応する空間3次元座標
  vec3 nearestSphere;

  
  for( pDist = pOffset; pDist <= radius; pDist += pOffset )
    {
      /// Normalの処理が必要な可能性あり
      calcLine_Plane_CrossPoint( norm, pos-normalize(norm)*pDist, pos, incident, posOnPlane );
      vec3 pos4Calc = posOnPlane + normalize(norm)*pDist; // ポリゴン上に戻す
      LeastSquaresMethod( pos4Calc - originUV, v10, v20, cef.x, cef.y ); // pos4CalcのUV値を再計算
      float a, b;
      if ( cef.x >= 0.0 ) a = ( floor(cef.x) + 0.5 );
      else                a = ( ceil (cef.x) - 0.5 );
      if ( cef.y >= 0.0 ) b = ( floor(cef.y) + 0.5 );
      else                b = ( ceil (cef.y) - 0.5 );

      nearestSphere = originUV + v10 * a + v20 * b - normalize(norm)*radius;
      
      float judgeC;
      IsCrossing( incident, camera, nearestSphere, radius, judgeC );
      if ( judgeC >= 0.0 ) break;

      cef = vec2(a,b);

    } 
  // -----------------------------------------------------------------------------------------
  // ------屈折処理---------------------------------------------------------------------------
  
  vec3 centerSphere = nearestSphere;

  // 球と視線ベクトルの交差判定（負なら交わらない）
  float judge;
  IsCrossing( incident, camera, centerSphere, radius, judge );

  vec3 finalCrossPoint; // 仮想平面との最終交点
  
  if ( judge >= 0.0 )
  {
    vec3 firstCrossPoint;
    calcLine_Sphere_CrossPointMinus(incident, camera, centerSphere, radius, firstCrossPoint);

    vec3 refrac0 = refract( incident, normalize(firstCrossPoint - centerSphere) , 1.0/nRatio );
    vec3 secondCrossPoint;
    calcLine_Sphere_CrossPointPlus (refrac0, firstCrossPoint, centerSphere, radius, secondCrossPoint);

    // 相対屈折率<1.0となる場合について全反射を考慮?
    vec3 refrac1 = refract( refrac0, normalize(centerSphere - secondCrossPoint), nRatio );

    //----------------------------
    float judge2;
    //IsCrossing( refrac1, secondCrossPoint, 
    
    //----------------------------

    calcLine_Plane_CrossPoint( norm, pos - 2*radius*normalize(norm), secondCrossPoint, refrac1, finalCrossPoint );
  }
  else
  {
    // 視線と仮想平面の交点を直接求める。
    calcLine_Plane_CrossPoint( norm, pos - 2*radius*normalize(norm), pos, incident, finalCrossPoint );
  }
  // -----------------------------------------------------------------------------------------
  
  // x,y,z→u,v
  //vec3 v3 = pos;
  vec3 v3 = finalCrossPoint + 2*radius * normalize(norm);
  //vec3( MVI * vec4( finalCrossPoint + 2*radius * normalize(norm), 1.0 ) );
  vec3 v30 = v3 - v0c; // 計算上v30(v3-v0)の値を入れる。

  float adbc = dot(v10,v10)*dot(v20,v20) - dot(v10,v20)*dot(v20,v10);
  alteredTexCoord.x = ( dot(v20,v20)*dot(v30,v10)-dot(v20,v10)*dot(v30,v20) ) / adbc;
  alteredTexCoord.y = ( dot(v10,v10)*dot(v30,v20)-dot(v10,v20)*dot(v30,v10) ) / adbc;  
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
  calcTexCoordForPupil_plural( originalPos, originalNorm, 2.0, altTexCoord ); // radiusの値に気をつける
  //calcTexCoordForPupil2( Position, Normal, 5.0, altTexCoord );
  vec4 texColor = texture( Tex1, altTexCoord );
  
  FragColor = 0.7*texColor;
  //FragColor = vec4(altTexCoord.xy/4.0,0.0,1.0);
  //FragColor = (vec4( ambAndDiff, 1.0 ) * texColor) + vec4(spec, 1.0);
}
  
 
 
