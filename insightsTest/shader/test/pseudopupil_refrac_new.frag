 #version 440

// pseudopupil.frag
in vec3 originalPos;
in vec3 originalNorm;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

// 元の値
in vec3 v10; //vu U軸ベクトルに対応したベクトル(正規化していない)
in vec3 v20; //vv V軸ベクトルに対応したベクトル(正規化していない)
in vec3 v0c;

uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform sampler2D Tex2;
uniform sampler2D Tex3;

uniform vec3 camera;

uniform cameraMat
{
  mat4 ModelViewMatrix;
  mat4 ModelViewProjectionMatrix;
  mat3 NormalMatrix;
  //mat4 ProjectionMatrix;
};


uniform bool isManThere;
//uniform vec3 angluar;
uniform float radius;
uniform int time;
uniform float value_test;


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


void calcLine_Sphere_CrossPointFar( vec3 v/*LineDir*/, vec3 x/*a point on the line*/,
				vec3 c/*center of sphere*/, float r/*radius of sphere*/, out vec3 cPoint )
{
  float D = pow( dot(v,x-c), 2.0) - dot(v,v) * ( dot(x-c,x-c) - r*r );
  float t = ( -dot(v,x-c) + sqrt(D) ) / dot(v,v);

  cPoint = x + t*v;
}


void calcLine_Sphere_CrossPointNear( vec3 v/*LineDir*/, vec3 x/*a point on the line*/,
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



// 複数の場合
void calcTexCoordForPupil_plural( vec3 pos, vec3 norm, float radius, out vec2 alteredTexCoord, out vec3 normal4phong )
{
  vec3 incident = normalize( pos - camera );
  
  // オリジナルを使うときの処理
  // ------最初の球を見つける---------------------------------------------------------------
  vec3 vU = normalize( v10 );
  vec3 vV = normalize( v20 ); 
  vec2 cef; // 係数
  float offset     = 2*radius; // 球同士の距離
  float pOffset    = 0.01*radius; // 平面のオフセット
  float pDist      = 0.0; // 仮想平面とポリゴンの距離
  vec3 posOnPlane; // 仮想平面と視線の交わる点
  vec3 originUV    = pos - v10*TexCoord.x - v20*TexCoord.y; // UV座標の原点に対応する空間3次元座標
  vec3 nearestSphere;
  bool isFirstSphereCrossing = false;

  for( pDist = 0; pDist <= radius; pDist += pOffset )
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
      if ( judgeC >= 0.0 )
	{
	  isFirstSphereCrossing = true;
	  cef = vec2(a,b);
	  break;
	}
    } 
  // -----------------------------------------------------------------------------------------
  // ------屈折処理---------------------------------------------------------------------------
  vec3 finalCrossPoint; // テクスチャ平面との最終交点
  
  if ( !isFirstSphereCrossing )
  {
    normal4phong = normalize( Normal ); // phongシェーディング用の法線を出力
    
    // 視線と仮想平面の交点を直接求める。
    calcLine_Plane_CrossPoint( norm, pos - 2*radius*normalize(norm), pos, incident, finalCrossPoint );
  }
  else
  {
    vec3 centerSphere = nearestSphere;
    vec3 nearCrossPoint;
    calcLine_Sphere_CrossPointNear(incident, camera, centerSphere, radius, nearCrossPoint);

    // phongシェーディング用の法線を出力
    normal4phong = normalize( mat3(ModelViewMatrix) * normalize(nearCrossPoint - centerSphere ) );

    vec3 refracOut;
    vec3 farCrossPoint;
    
    vec3 refracIncident = incident;
    vec3 refracIn = refract( refracIncident, normalize(nearCrossPoint - centerSphere) , 1.0/nRatio );
    calcLine_Sphere_CrossPointFar (refracIn, nearCrossPoint, centerSphere, radius, farCrossPoint);

    int fstep = 0;
    int fstep_limit = 50;
    while( fstep < fstep_limit )
      {
	vec3 r = normalize( refracIn );
	vec3 n = normalize( norm );
	vec3 dirVec = normalize( r - n*dot(r,n) ); /* 方向ベクトルの平面への投影 */	
	
	// 相対屈折率<1.0となる場合について全反射を考慮? ****追記：フレネル反射を利用？****
	/* 途中で更新されなければ、最後までこのrefracOutの値が使われる */
	refracOut = refract( refracIn, normalize(centerSphere - farCrossPoint), nRatio );
	//---------------------------------------------
	if ( abs(dot(dirVec,v10)) > abs(dot(dirVec,v20)) ) /*area 1 or 3*/
	  { 
	    float p_n = dot(dirVec,v10) / abs( dot(dirVec,v10) ); /* 符号 (1 or -1) */

	    //--------------------------------------------------------------------------------
	    /*1*/
	    cef = cef + vec2(p_n, -1.0f);
	    vec3 nextSphere = originUV + v10*cef.x + v20*cef.y - normalize(norm)*radius;
	    /**************** 内側 ****************/ 
	    if( length( nextSphere - farCrossPoint ) <= radius ) 
	      {
		// update farCross Point 
		calcLine_Sphere_CrossPointFar(refracIn, nearCrossPoint, nextSphere, radius, farCrossPoint);
		// update cetnerSphere 
		centerSphere = nextSphere;
		
		fstep++;
		continue;
	      } /* 内側なら次のfstepへ */
	    /**************************************/ 

	    float next_crossing;
	    /**************** 外側 ****************/ 
	    IsCrossing(refracOut, farCrossPoint, nextSphere, radius, next_crossing );
	    if ( next_crossing >= 0.0 )
	      {
		// 新たなnearCrossPointを計算
		calcLine_Sphere_CrossPointNear(refracOut, farCrossPoint, nextSphere, radius, nearCrossPoint);
		centerSphere = nextSphere;
		refracIncident = refracOut;
		refracIn = refract( refracIncident, normalize(nearCrossPoint - centerSphere) , 1.0/nRatio );
		calcLine_Sphere_CrossPointFar(refracIn, nearCrossPoint, centerSphere, radius, farCrossPoint);
		fstep++;
		continue;
	      } /* 交差しているなら次のfstepへ */
	    /**************************************/

	    /*2*/
	    //////////交差していないならcefを更新して次の球へ/////////
	    cef = cef + vec2( 0.0, 1.0f );
	    nextSphere = originUV + v10*cef.x + v20*cef.y - normalize(norm)*radius;
	    /**************** 内側 ****************/ 
	    if( length( nextSphere - farCrossPoint ) <= radius ) 
	      {
		// update farCross Point 
		calcLine_Sphere_CrossPointFar(refracIn, nearCrossPoint, nextSphere, radius, farCrossPoint);
		// update cetnerSphere 
		centerSphere = nextSphere;
		
		fstep++;
		continue;
	      } /* 内側なら次のfstepへ */
	    /**************************************/ 

	    /**************** 外側 ****************/ 
	    IsCrossing(refracOut, farCrossPoint, nextSphere, radius, next_crossing );
	    if ( next_crossing >= 0.0 )
	      {
		// 新たなnearCrossPointを計算
		calcLine_Sphere_CrossPointNear(refracOut, farCrossPoint, nextSphere, radius, nearCrossPoint);
		centerSphere = nextSphere;
		refracIncident = refracOut;
		refracIn = refract( refracIncident, normalize(nearCrossPoint - centerSphere) , 1.0/nRatio );
		calcLine_Sphere_CrossPointFar(refracIn, nearCrossPoint, centerSphere, radius, farCrossPoint);
		fstep++;
		continue;
	      } /* 交差しているなら次のfstepへ */
	    /**************************************/


	    /*3*/
	    //////////交差していないならcefを更新して最後の球へ/////////
	    cef = cef + vec2(0.0f,1.0f);
	    nextSphere = originUV + v10*cef.x + v20*cef.y - normalize(norm)*radius;
	    /**************** 内側 ****************/ 
	    if( length( nextSphere - farCrossPoint ) <= radius ) 
	      {
		// update farCross Point 
		calcLine_Sphere_CrossPointFar(refracIn, nearCrossPoint, nextSphere, radius, farCrossPoint);
		// update cetnerSphere 
		centerSphere = nextSphere;
		
		fstep++;
		continue;
	      } /* 内側なら次のfstepへ */
	    /**************************************/ 

	    /**************** 外側 ****************/ 
	    IsCrossing(refracOut, farCrossPoint, nextSphere, radius, next_crossing );
	    if ( next_crossing >= 0.0 )
	      {
		// 新たなnearCrossPointを計算
		calcLine_Sphere_CrossPointNear(refracOut, farCrossPoint, nextSphere, radius, nearCrossPoint);
		centerSphere = nextSphere;
		refracIncident = refracOut;
		refracIn = refract( refracIncident, normalize(nearCrossPoint - centerSphere) , 1.0/nRatio );
		calcLine_Sphere_CrossPointFar(refracIn, nearCrossPoint, centerSphere, radius, farCrossPoint);
		fstep++;
		continue;
	      } /* 交差しているなら次のfstepへ */
	    /**************************************/

	    /* すべての球（三連星）と交差しなければ隣接球プロセスを終了する*/
	    fstep = fstep_limit;
	    //-----------area 1 or 3-------------------------------------------------------------
	  }
	else /* area 2 or 4 */
	  {
	    float p_n = dot(dirVec,v20) / abs( dot(dirVec,v20) ); /* 符号 (1 or -1) */

	    //--------------------------------------------------------------------------------
	    /*1*/
	    cef = cef + vec2(-1.0, p_n);
	    vec3 nextSphere = originUV + v10*cef.x + v20*cef.y - normalize(norm)*radius;
	    /**************** 内側 ****************/ 
	    if( length( nextSphere - farCrossPoint ) <= radius ) 
	      {
		// update farCross Point 
		calcLine_Sphere_CrossPointFar(refracIn, nearCrossPoint, nextSphere, radius, farCrossPoint);
		// update cetnerSphere 
		centerSphere = nextSphere;
		
		fstep++;
		continue;
	      } /* 内側なら次のfstepへ */
	    /**************************************/ 

	    float next_crossing;
	    /**************** 外側 ****************/ 
	    IsCrossing(refracOut, farCrossPoint, nextSphere, radius, next_crossing );
	    if ( next_crossing >= 0.0 )
	      {
		// 新たなnearCrossPointを計算
		calcLine_Sphere_CrossPointNear(refracOut, farCrossPoint, nextSphere, radius, nearCrossPoint);
		centerSphere = nextSphere;
		refracIncident = refracOut;
		refracIn = refract( refracIncident, normalize(nearCrossPoint - centerSphere) , 1.0/nRatio );
		calcLine_Sphere_CrossPointFar(refracIn, nearCrossPoint, centerSphere, radius, farCrossPoint);
		fstep++;
		continue;
	      } /* 交差しているなら次のfstepへ */
	    /**************************************/
	    

	    /*2*/
	    //////////交差していないならcefを更新して次の球へ/////////
	    cef = cef + vec2(1.0,0.0);
	    nextSphere = originUV + v10*cef.x + v20*cef.y - normalize(norm)*radius;
	    /**************** 内側 ****************/ 
	    if( length( nextSphere - farCrossPoint ) <= radius ) 
	      {
		// update farCross Point 
		calcLine_Sphere_CrossPointFar(refracIn, nearCrossPoint, nextSphere, radius, farCrossPoint);
		// update cetnerSphere 
		centerSphere = nextSphere;
		
		fstep++;
		continue;
	      } /* 内側なら次のfstepへ */
	    /**************************************/ 

	    /**************** 外側 ****************/ 
	    IsCrossing(refracOut, farCrossPoint, nextSphere, radius, next_crossing );
	    if ( next_crossing >= 0.0 )
	      {
		// 新たなnearCrossPointを計算
		calcLine_Sphere_CrossPointNear(refracOut, farCrossPoint, nextSphere, radius, nearCrossPoint);
		centerSphere = nextSphere;
		refracIncident = refracOut;
		refracIn = refract( refracIncident, normalize(nearCrossPoint - centerSphere) , 1.0/nRatio );
		calcLine_Sphere_CrossPointFar(refracIn, nearCrossPoint, centerSphere, radius, farCrossPoint);
		fstep++;
		continue;
	      } /* 交差しているなら次のfstepへ */
	    /**************************************/


	    /*3*/
	    //////////交差していないならcefを更新して最後の球へ/////////
	    cef = cef + vec2(1.0,0.0);
	    nextSphere = originUV + v10*cef.x + v20*cef.y - normalize(norm)*radius;
	    /**************** 内側 ****************/ 
	    if( length( nextSphere - farCrossPoint ) <= radius ) 
	      {
		// update farCross Point 
		calcLine_Sphere_CrossPointFar(refracIn, nearCrossPoint, nextSphere, radius, farCrossPoint);
		// update cetnerSphere 
		centerSphere = nextSphere;
		
		fstep++;
		continue;
	      } /* 内側なら次のfstepへ */
	    /**************************************/ 

	    /**************** 外側 ****************/ 
	    IsCrossing(refracOut, farCrossPoint, nextSphere, radius, next_crossing );
	    if ( next_crossing >= 0.0 )
	      {
		// 新たなnearCrossPointを計算
		calcLine_Sphere_CrossPointNear(refracOut, farCrossPoint, nextSphere, radius, nearCrossPoint);
		centerSphere = nextSphere;
		refracIncident = refracOut;
		refracIn = refract( refracIncident, normalize(nearCrossPoint - centerSphere) , 1.0/nRatio );
		calcLine_Sphere_CrossPointFar(refracIn, nearCrossPoint, centerSphere, radius, farCrossPoint);
		fstep++;
		continue;
	      } /* 交差しているなら次のfstepへ */
	    /**************************************/

	    /* すべての球（三連星）と交差しなければ隣接球プロセスを終了する*/
	    fstep = fstep_limit;
	    //-----------area 2 or 4-------------------------------------------------------------
	  }// end of else{}
		
	
	//---------------------------------------------
	
      }// whileの終わり*/
    //refracOut = refract( refracIn, normalize(centerSphere - farCrossPoint), nRatio );
    calcLine_Plane_CrossPoint( norm, pos - 2*radius*normalize(norm), farCrossPoint, refracOut, finalCrossPoint );
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
  vec3 diffuse =  sDotN * vec3(Light.Intensity * Material.Kd);
  spec = vec3(0.0);
  if( sDotN > 0.0 )
    spec = Light.Intensity * Material.Ks *
               pow( max( dot(r,v), 0.0 ), Material.Shininess );
  ambAndDiff = ambient + diffuse;
}

void main()
{
  vec2 altTexCoord;
  vec3 norm4phong;
  calcTexCoordForPupil_plural( originalPos, originalNorm, radius, altTexCoord, norm4phong ); // radiusの値に気をつける

  /* vec4 texColor; */
  /* if(value_test <= 1f) texColor = texture( Tex0, altTexCoord )*( 1f - (value_test-floor(value_test)) ) */
  /* 			 +texture( Tex1, altTexCoord )*(value_test-floor(value_test)); */
  /* if(1f< value_test && value_test <= 2f) texColor = texture( Tex1, altTexCoord )*( 1f - (value_test-floor(value_test)) ) */
  /* 			+texture( Tex2, altTexCoord )*(value_test-floor(value_test)); */
  /* if(2f< value_test && value_test <= 3f) texColor = texture( Tex2, altTexCoord )*( 1f - (value_test-floor(value_test)) )  */
  /* 			+texture( Tex3, altTexCoord )*(value_test-floor(value_test)); */
  /* if(value_test > 3f) texColor = texture( Tex3, altTexCoord ); */

  vec4 texColor = texture( Tex0, altTexCoord );
    
  vec3 ambAndDiff, spec;
  

  /* norm4phongを使用する場合、フレネル反射を考慮しなければうまい絵が得られないのでは？*/
  phongModel( Position, Normal, ambAndDiff, spec );

  
  FragColor = vec4(ambAndDiff,1.0)*texColor + vec4(spec, 1.0);
  FragColor = texColor;
}
  
 
 
