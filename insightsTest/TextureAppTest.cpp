#include "TextureAppTest.h"

#include "TextureManager.h"
#pragma comment(lib, "FreeImage.lib")


TextureAppTest::TextureAppTest(void)
{ 
	t=0.0;
}


TextureAppTest::~TextureAppTest(void)
{
}


void TextureAppTest::initGL()
{

	//----------------------------------------------------------------	
	ActivateGLSL();

	kzsGLObject::initGL();
	cout << "GLSL version: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << endl;
	cout << "" << endl;

	cout << "Vendor :"<<glGetString(GL_VENDOR)<<'\n';
	cout << "GPU : "<<glGetString(GL_RENDERER) <<'\n';
	cout << "OpenGL ver. " <<glGetString(GL_VERSION)<<'\n';
	//cout << "【拡張機能一覧】"<<endl;
	//cout << glGetString(GL_EXTENSIONS) <<endl;
	cout << endl;
	//----------------------------------------------------------------

	// Axes Initialization
	initAxes();

	// 構想発表用-----------------------------------------------------
	vec3 points[6] =
	{
		vec3( -5, -5, 0 ), vec3(  5, -5, 0 ),
		vec3(  5,  5, 0 ), vec3(  5,  5, 0 ),
		vec3( -5,  5, 0 ), vec3( -5, -5, 0 ) 
	};

	vec3 normals[ 6 ] =
	{
		vec3( 0.0, 0.0, 1.0 ), vec3( 0.0, 0.0, 1.0 ),
		vec3( 0.0, 0.0, 1.0 ), vec3( 0.0, 0.0, 1.0 ),
		vec3( 0.0, 0.0, 1.0 ), vec3( 0.0, 0.0, 1.0 ),
	};

	vec2 texCoords[ 6 ] =
	{
		vec2( 0.0, 0.0 ), vec2( 1.0, 0.0 ),
		vec2( 1.0, 1.0 ), vec2( 1.0, 1.0 ),
		vec2( 0.0, 1.0 ), vec2( 0.0, 0.0 )
	};
	
	//----------------------------------------------------------------
	/*
	vec3 points[ 600 ];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			points[i*60+j*6]   = vec3(-5+j  , -5+i  , 0);
			points[i*60+j*6+1] = vec3(-5+j+1, -5+i  , 0);
			points[i*60+j*6+2] = vec3(-5+j+1, -5+i+1, 0);
			points[i*60+j*6+3] = vec3(-5+j+1, -5+i+1, 0);
			points[i*60+j*6+4] = vec3(-5+j  , -5+i+1, 0);
			points[i*60+j*6+5] = vec3(-5+j  , -5+i,   0);
		}
		
	}

	vec3 normals[ 600 ];
	for (int i = 0; i < 600; i++)
	{
		normals[i] = vec3( 0.0, 0.0, 1.0 );
	}

	vec2 texCoords[ 600 ];
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			texCoords[i*6+j] = originalTexCoords[j];
		}
	}*/

	// for calculation plane:(v0,vu,vv)
	vec3 planeVec[ 18 ];

	// make unit vector(uv <---> xyz)---------
	for (int i = 0; i < 2;/*polygons*/ i++)
	{
		vec2 u0  = texCoords[i*3];
		vec2 u10 = texCoords[i*3+1] - u0;
		vec2 u20 = texCoords[i*3+2] - u0;

		float p,q,r,s;
		float det_inv;
		det_inv = 1.0 / (u10.x * u20.y - u20.x * u10.y);
		p =  u20.y * det_inv;
		q = -u10.y * det_inv;
		r = -u20.x * det_inv;
		s =  u10.x * det_inv;
		/*cout<<"p:"<<p<<endl;
		cout<<"q:"<<q<<endl;
		cout<<"r:"<<r<<endl;
		cout<<"s:"<<s<<endl;*/

		//vec2 check = p*u10 + q*u20;
		//cout<<"check:"<<check.x<<" "<<check.y<<endl;


		vec3 v0  = points[i*3];
		vec3 v10 = points[i*3+1] - v0;
		vec3 v20 = points[i*3+2] - v0;

		vec3 vu = p*v10 + q*v20;
		vec3 vv = r*v10 + s*v20;
		
		//cout<<"vu: "<< vu.x <<" "<< vu.y <<" "<< vu.z <<endl;
		//cout<<"vv: "<< vv.x <<" "<< vv.y <<" "<< vv.z <<endl;


		for (int j = 0; j < 3; j++)
		{
			planeVec[i*9 + j*3]     = v0;
			planeVec[i*9 + j*3 + 1] = vu;
			planeVec[i*9 + j*3 + 2] = vv;
		}
	}

	//cout<<"planeVec"<<endl;
	for (int i = 0; i < 18; i++)
	{
		//cout<< planeVec[i].x <<" "<< planeVec[i].y <<" "<< planeVec[i].z <<endl;
	}
	cout<<"-----------------------------------"<<endl;

	/*
	vec3 spherePos[200];
	for (int i = 0; i < 100; i++)
	{
		spherePos[i*2] = ( points[i*6]+points[i*6+1]+points[i*6+3]+points[i*6+4] ) * 0.25f;
		spherePos[i*2+1] = spherePos[i*2];
		//cout<<spherePos[i*2].x<<", "<<spherePos[i*2].y<<", "<<spherePos[i*2].z<<endl;
	}*/

	//----------------------------------------


	GLuint buffer[ 5 ];
	
	glGenBuffers( 5, buffer );
	GLuint positionVBO = buffer[ 0 ];
	GLuint normalVBO   = buffer[ 1 ];
	GLuint texCoordVBO = buffer[ 2 ];
	GLuint planeVecVBO = buffer[ 3 ];
	//GLuint spherePosVBO = buffer[ 4 ];

	
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( points ), points, GL_STATIC_DRAW );
	
	glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( normals ), normals, GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, texCoordVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( texCoords ), texCoords, GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, planeVecVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( planeVec ), planeVec, GL_STATIC_DRAW );
		
	//glBindBuffer( GL_ARRAY_BUFFER, spherePosVBO );
	//glBufferData( GL_ARRAY_BUFFER, sizeof( spherePos ), spherePos, GL_STATIC_DRAW );

	//----------------------------------------------------------------
	
	// GLSL shader Initialization
	shader.initShaderProgram( "shader/test/texture_sample.vert", "shader/test/texture_sample.frag" );
	printf( "---main shader---\n" );
	shader.printActiveAttribs();
	shader.printActiveUniforms();
	shader.BeginShader();


	// VBO Settings
	glGenVertexArrays( 1, &vao[0] );
	glBindVertexArray( vao[0] );

	GLuint pos_loc = shader.getAttribLocation( "vPosition" );
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
	glEnableVertexAttribArray( pos_loc );
	glVertexAttribPointer( pos_loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	GLuint nrm_loc = shader.getAttribLocation( "vNormal" );
	glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
	glEnableVertexAttribArray( nrm_loc );
	glVertexAttribPointer( nrm_loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	
	GLuint tex_loc = shader.getAttribLocation( "vTexCoord" );
	glBindBuffer( GL_ARRAY_BUFFER, texCoordVBO );
	glEnableVertexAttribArray( tex_loc );
	glVertexAttribPointer( tex_loc, 2, GL_FLOAT, GL_FALSE, 0, 0 );


	//------------------------------------------------------madayokuwakarann
	GLuint v_loc = shader.getAttribLocation( "v0" );
	glBindBuffer( GL_ARRAY_BUFFER, planeVecVBO );
	glEnableVertexAttribArray( v_loc );
	glVertexAttribPointer( v_loc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3)*3, 0 );

	v_loc = shader.getAttribLocation( "v1" );
	glBindBuffer( GL_ARRAY_BUFFER, planeVecVBO );
	glEnableVertexAttribArray( v_loc );
	glVertexAttribPointer( v_loc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3)*3, (const void*) sizeof(vec3) );

	v_loc = shader.getAttribLocation( "v2" );
	glBindBuffer( GL_ARRAY_BUFFER, planeVecVBO );
	glEnableVertexAttribArray( v_loc );
	glVertexAttribPointer( v_loc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3)*3, (const void*) (sizeof(vec3)*2) );




	glBindVertexArray( vao[0] );


	// Camera Matrices Initialization
	camera.SetMatrices( shader );

	// Texture Settings
	TextureManager::Inst()->LoadTexture( "texture/a.jpg", 0 );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	// sampler uniform settings
	int loc = glGetUniformLocation( shader.getHandle(), "Tex1" );
	if ( loc >= 0 )	glUniform1i( loc, 0 );
	else fprintf( stderr, "Uniform Variable \"Tex1\" Not Found!\n" );


	//------------------------------------------------------------------------
	/*
	// sphere lens position
	float sphere[75];
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			sphere[ i*15 + j*3     ] = i*2 -4;
			sphere[ i*15 + j*3 + 1 ] = j*2 -4;
			sphere[ i*15 + j*3 + 2 ] = 0;
		}
	}

	for (int i = 0; i < 25; i++)
	{
		cout<< sphere[ i*3 ] << ", " << sphere[ i*3 +1 ] << ", " << sphere[ i*3 +2 ] <<endl; 
	}

	glUniform1fv( glGetUniformLocation( shader.getHandle(), "sphere" ), 75, sphere );
	//------------------------------------------------------------------------
	*/

	glClearColor( 0.0, 0.0, 0.0, 1.0 );



	
}


void TextureAppTest::display()
{
	shader.setUniform("Light.Position", vec4(0.0f,0.0f,5.0f,1.0f) );
	shader.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f) );
    shader.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
    shader.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
    shader.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    shader.setUniform("Material.Shininess", 100.0f);


	camera.SetCameraPosition( vec3( 20 * sin( 0.0 ), 20, 20 * cos( 0.0 ) ) );
	

	if ( gScene ) StepPhysX();
	glClear( GL_COLOR_BUFFER_BIT );//| GL_DEPTH_BUFFER_BIT );
	
	//-------------------------------------------------
	// add Object to draw
	shader.BeginShader();
	camera.updataMatrices();
	//camera.SetModelViewInv( shader, "MVI" );
	/*cout<< "cameraPos: "
		<< camera.getCameraPosition().x <<", "
		<< camera.getCameraPosition().y <<", "
		<< camera.getCameraPosition().z <<endl;*/
	shader.setUniform("camera", camera.getCameraPosition() );
	t+=0.03;

	///注意！！球の半径によって変化する値があります。
	shader.setUniform("spherePos", vec3(0*cos(t),0*sin(t),-5.0));

	TextureManager::Inst()->BindTexture(0);
	glBindVertexArray( vao[0] );
	glDrawArrays( GL_TRIANGLES, 0, 6 );

	// glUseProgramによって(kzsGLSL)simpleにバインドされます。
	drawAxes();

	//-------------------------------------------------
	
	//glUtil.RenderFramerate( WINDOW_WIDTH, WINDOW_HEIGHT );

	glutSwapBuffers();
}


//===============================================================
// for debug

void TextureAppTest::initAxes()
{

	simple.initShaderProgram( "shader/test/simple.vert", "shader/test/simple.frag" );
	printf("---Simple Shader---\n" );
	simple.printActiveAttribs();
	simple.printActiveUniforms();
	
	vec3 points[6]=
	{
		vec3(0,0,0), vec3(5,0,0), // x Axis
		vec3(0,0,0), vec3(0,5,0), // y Axis
		vec3(0,0,0), vec3(0,0,5), // z Axis
	};

	vec3 colors[6]=
	{
		vec3(1.0f,0.0f,0.0f), vec3(1.0f,0.0f,0.0f),
		vec3(0.0f,1.0f,0.0f), vec3(0.0f,1.0f,0.0f),
		vec3(0.0f,0.0f,1.0f), vec3(0.0f,0.0f,1.0f)
	};

	GLuint buffer[2];

	glGenBuffers(2, buffer);
	GLuint posVBO = buffer[0];
	GLuint colVBO = buffer[1];

	glBindBuffer( GL_ARRAY_BUFFER, posVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
	
	glBindBuffer( GL_ARRAY_BUFFER, colVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW );
	
	glGenVertexArrays( 1, &vao[1] );
	glBindVertexArray( vao[1] );

	GLuint pos_loc = simple.getAttribLocation( "vPosition" );
	glBindBuffer( GL_ARRAY_BUFFER, posVBO );
	glEnableVertexAttribArray( pos_loc );
	glVertexAttribPointer( pos_loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	GLuint col_loc = simple.getAttribLocation( "vColor" );
	glBindBuffer( GL_ARRAY_BUFFER, colVBO );
	glEnableVertexAttribArray( col_loc );
	glVertexAttribPointer( col_loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	glBindVertexArray( vao[1] );
	
	camera.SetMatrices( simple );
}


void TextureAppTest::drawAxes()
{
	simple.BeginShader();
	glBindVertexArray( vao[1] );
	glDrawArrays( GL_LINES, 0, 6 );
}


void TextureAppTest::testLinePlaneCrossPoint(vec3 n, vec3 p, vec3 x0, vec3 v )
{
	n = glm::normalize(n);
	v = glm::normalize(v);
	vec3 cPoint = v*(glm::dot(n,x0) - glm::dot(n,p))/glm::dot(n,v) + p;
	cout<< cPoint.x <<", "<< cPoint.y <<", "<<cPoint.z <<endl;
}


void TextureAppTest::testTexCoordCalc()
{
	vec3 points[ 6 ] =
	{
		vec3( -5, -5, 0 ), vec3(  5, -5, 0 ),
		vec3(  5,  5, 0 ), vec3(  5,  5, 0 ),
		vec3( -5,  5, 0 ), vec3( -5, -5, 0 ) 
	};

	vec3 normals[ 6 ] =
	{
		vec3( 0.0, 0.0, 1.0 ), vec3( 0.0, 0.0, 1.0 ),
		vec3( 0.0, 0.0, 1.0 ), vec3( 0.0, 0.0, 1.0 ),
		vec3( 0.0, 0.0, 1.0 ), vec3( 0.0, 0.0, 1.0 ),
	};

	vec2 texCoords[ 6 ] =
	{
		vec2( 0.0, 0.0 ), vec2( 10.0, 0.0 ),
		vec2( 10.0, 10.0 ), vec2( 10.0, 10.0 ),
		vec2( 0.0, 10.0 ), vec2( 0.0, 0.0 )
	};

	// for calculation plane:(v0,v1,v2)
	vec3 planeVec[ 18 ];
	
	// make unit vector(uv <---> xyz)---------
	for (int i = 0; i < 2;/*polygons*/ i++)
	{
		vec2 u0  = texCoords[i*3];
		vec2 u10 = texCoords[i*3+1] - u0;
		vec2 u20 = texCoords[i*3+2] - u0;

		float p,q,r,s;
		float det_inv;
		det_inv = 1.0 / (u10.x * u20.y - u20.x * u10.y);
		p =  u20.y * det_inv;
		q = -u10.y * det_inv;
		r = -u20.x * det_inv;
		s =  u10.x * det_inv;
		/*cout<<"p:"<<p<<endl;
		cout<<"q:"<<q<<endl;
		cout<<"r:"<<r<<endl;
		cout<<"s:"<<s<<endl;
		*/
		vec2 check = p*u10 + q*u20;
		//cout<<"check:"<<check.x<<" "<<check.y<<endl;


		vec3 v0  = points[i*3];
		vec3 v10 = points[i*3+1] - v0;
		vec3 v20 = points[i*3+2] - v0;

		vec3 vu = p*v10 + q*v20;
		vec3 vv = r*v10 + s*v20;
		
		//cout<<"vu: "<< vu.x <<" "<< vu.y <<" "<< vu.z <<endl;
		//cout<<"vv: "<< vv.x <<" "<< vv.y <<" "<< vv.z <<endl;


		for (int j = 0; j < 3; j++)
		{
			planeVec[i*9 + j*3]     = v0;
			planeVec[i*9 + j*3 + 1] = vu;
			planeVec[i*9 + j*3 + 2] = vv;
		}
	}

	vec3 v3;


	cout<<"TexCoordTest"<<endl;
	
	
	/*for (int i = 0; i < 3; i++)
	{
		float tlen = glm::length(texCoords[0] - texCoords[i]);
		float a,b;
		if( tlen != 0 )
		{
			a = texCoords[i].x * 1.0/tlen;
			b = texCoords[i].y * 1.0/tlen;
		}
		else
		{
			a = texCoords[i].x;
			b = texCoords[i].y;
		}

		v3 = v0 + a * v10  + b * v20;
		cout<< v3.x <<", "<< v3.y <<", "<< v3.z <<endl;
	}*/
	cout<<"---------------------"<<endl;
}


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/projection.hpp>

void TextureAppTest::testAffine()
{
	vec3 p(1.0f, 0.0f, 0.0f);
	mat4 m(1.0);
	
	m = glm::rotate( m, 90.0f, vec3(1,0,0) );
	m = glm::rotate( m, 90.0f, vec3(0,1,0) );
	m = glm::translate( m, vec3(2.0,0.0,0.0) );

	
	p = vec3( m * vec4(p,1.0) );
	cout<<""<<endl;
	cout<<"affine test"<<endl;
	cout<< p.x <<" "<<p.y<<" "<<p.z<<endl;
	cout<<""<<endl;
}