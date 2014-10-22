#include "TextureAppTest.h"

#include "TextureManager.h"
#pragma comment(lib, "FreeImage.lib")


TextureAppTest::TextureAppTest(void) //: mesh(true)
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

	// obj data-----------------------------------------------------
	mesh.ObjLoad( "./obj/cubeSubdived.obj" );

	vec3 *points    = new vec3 [ 3*mesh.getNumPolygon() ];
	vec3 *normals   = new vec3 [ 3*mesh.getNumPolygon() ];
	vec2 *texCoords = new vec2 [ 3*mesh.getNumPolygon() ];

	for (int i = 0; i < mesh.getNumPolygon(); i++)
	{
		points[i*3]   = mesh.vertex[i*3];
		points[i*3+1] = mesh.vertex[i*3+1];
		points[i*3+2] = mesh.vertex[i*3+2];
	}
	for (int i = 0; i < mesh.getNumPolygon(); i++)
	{
		normals[i*3]   = mesh.normal[i*3];
		normals[i*3+1] = mesh.normal[i*3+1];
		normals[i*3+2] = mesh.normal[i*3+2];
	}
	for (int i = 0; i < mesh.getNumPolygon(); i++)
	{
		texCoords[i*3]   = mesh.texcoord[i*3] * 300.0f ;
		texCoords[i*3+1] = mesh.texcoord[i*3+1] * 300.0f ;
		texCoords[i*3+2] = mesh.texcoord[i*3+2] * 300.0f ;
	}

	for (int i = 0; i < 3*mesh.getNumPolygon(); i++)
	{
		//cout<<"points["<< i <<"] "<<points[i].x <<" "<< points[i].y<<" "<<points[i].z <<endl;
		//cout<<"normals["<< i <<"] "<<normals[i].x <<" "<< normals[i].y<<" "<<normals[i].z <<endl;
		//cout<<"texCoords["<< i <<"] "<<texCoords[i].x <<" "<< texCoords[i].y <<endl;
	}

	//----------------------------------------------------------------

	// for calculation plane:(v0,vu,vv)
	vec3 *planeVec = new vec3 [ 9 * mesh.getNumPolygon() ];
	// face normal
	vec3 *fNormal  = new vec3 [ 3 * mesh.getNumPolygon() ];

	// make unit vector(uv <---> xyz)---------
	for (int i = 0; i < mesh.getNumPolygon();/*polygons*/ i++)
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
		vec3 vc = v0 - texCoords[i*3].x * vu - texCoords[i*3].y * vv;

		//cout<<"vu: "<< vu.x <<" "<< vu.y <<" "<< vu.z <<endl;
		//cout<<"vv: "<< vv.x <<" "<< vv.y <<" "<< vv.z <<endl;

		// 3頂点に対して
		for (int j = 0; j < 3; j++)
		{
			planeVec[i*9 + j*3]     = vc;
			planeVec[i*9 + j*3 + 1] = vu;
			planeVec[i*9 + j*3 + 2] = vv;
		}

		// face normal---------------------------------
		vec3 fN = normalize( glm::cross( v10, v20 ) );
		fNormal[i*3]     = fN;
		fNormal[i*3 + 1] = fN;
		fNormal[i*3 + 2] = fN;
		// --------------------------------------------
		
	}

	//cout<<"planeVec"<<endl;
	for (int i = 0; i < 9*mesh.getNumPolygon(); i++)
	{
		//cout<< "planeVec["<< i <<"]" << planeVec[i].x <<" "<< planeVec[i].y <<" "<< planeVec[i].z <<endl;
	}
	cout<<"-----------------------------------"<<endl;


	//----------------------------------------
	//=================================================================================================


	GLuint buffer[ 5 ];
	
	glGenBuffers( 5, buffer );
	GLuint positionVBO = buffer[ 0 ];
	GLuint normalVBO   = buffer[ 1 ];
	GLuint texCoordVBO = buffer[ 2 ];
	GLuint planeVecVBO = buffer[ 3 ];
	GLuint fNormalVBO  = buffer[ 4 ];

	cout<<"polygon number "<<mesh.getNumPolygon()<<endl;

	glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 )*3*mesh.getNumPolygon(), points, GL_STATIC_DRAW );
	
	glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 )*3*mesh.getNumPolygon(), normals, GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, texCoordVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vec2 )*3*mesh.getNumPolygon(), texCoords, GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, planeVecVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 )*9*mesh.getNumPolygon(), planeVec, GL_STATIC_DRAW );
		
	glBindBuffer( GL_ARRAY_BUFFER, fNormalVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 )*3*mesh.getNumPolygon(), fNormal, GL_STATIC_DRAW );

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
	glVertexAttribPointer( pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0 );

	GLuint nrm_loc = shader.getAttribLocation( "vNormal" );
	glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
	glEnableVertexAttribArray( nrm_loc );
	glVertexAttribPointer( nrm_loc, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0 );
	
	GLuint tex_loc = shader.getAttribLocation( "vTexCoord" );
	glBindBuffer( GL_ARRAY_BUFFER, texCoordVBO );
	glEnableVertexAttribArray( tex_loc );
	glVertexAttribPointer( tex_loc, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0 );


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

	GLuint fN_loc = shader.getAttribLocation( "fN" );
	glBindBuffer( GL_ARRAY_BUFFER, fNormalVBO );
	glEnableVertexAttribArray( fN_loc );
	glVertexAttribPointer( fN_loc, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0 );


	glBindVertexArray( vao[0] );


	// Camera Matrices Initialization   
	camera.SetMatrices( shader );

	// Texture Settings
	TextureManager::Inst()->LoadTexture( "texture/spot.jpg", 0 );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	// sampler uniform settings
	int loc = glGetUniformLocation( shader.getHandle(), "Tex1" );
	if ( loc >= 0 )	glUniform1i( loc, 0 );
	else fprintf( stderr, "Uniform Variable \"Tex1\" Not Found!\n" );


	glClearColor( 0.0, 0.0, 0.0, 1.0 );


	// Release
	delete points;
	delete normals;
	delete texCoords;
	delete planeVec;
	delete fNormal;
	
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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//-------------------------------------------------
	// add Object to draw
	drawAxes();

	shader.BeginShader();
	camera.updataMatrices();
	//camera.SetModelViewInv( shader, "MVI" );
	/*cout<< "cameraPos: "
		<< camera.getCameraPosition().x <<", "
		<< camera.getCameraPosition().y <<", "
		<< camera.getCameraPosition().z <<endl;*/
	shader.setUniform("camera", camera.getCameraPosition() );
	//t+=0.03;

	TextureManager::Inst()->BindTexture(0);
	glBindVertexArray( vao[0] );
	glDrawArrays( GL_TRIANGLES, 0, 3*mesh.getNumPolygon() );

	// glUseProgramによって(kzsGLSL)simpleにバインドされます。


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