#include "SceneTextureTest.h"
//#include <QtOpenGL\qgl.h>

using glm::vec2;
using glm::vec3;

using std::cout;
using std::endl;

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform2.hpp>
#include <glm\gtx\projection.hpp>

SceneTextureTest::SceneTextureTest(void)
{
}


SceneTextureTest::~SceneTextureTest(void)
{
}


void SceneTextureTest::initScene()
{
	

	glEnable( GL_DEPTH_TEST );
	vec2 points[ 6 ] =
	{
		vec2( -5, -5 ), vec2(  5, -5 ),
		vec2(  5,  5 ), vec2(  5,  5 ),
		vec2( -5,  5 ), vec2( -5, -5 ) 
	};

	
	vec3 colors[ 6 ] = 
	{
		vec3( 1.0, 0.0, 0.0 ), vec3( 0.0, 1.0, 0.0 ),
		vec3( 0.0, 0.0, 1.0 ), vec3( 1.0, 1.0, 0.0 ),
		vec3( 1.0, 0.0, 1.0 ), vec3( 0.0, 1.0, 1.0 )
	};

	GLuint vao, buffer[ 2 ];
	
	glGenBuffers( 2, buffer );
	GLuint positionVBO = buffer[ 0 ];
	GLuint colorVBO    = buffer[ 1 ];
	
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( points ), points, GL_STATIC_DRAW );
	
	glBindBuffer( GL_ARRAY_BUFFER, colorVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( colors ), colors, GL_STATIC_DRAW );

	//----------------------------------------------------------------
	
	// GLSL shader Initialization
	shader.initShaderProgram( "shader/test/simple.vert", "shader/test/simple.frag" );
	shader.printActiveAttribs();
	shader.printActiveUniforms();
	shader.BeginShader();


	// VBO Settings
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	GLuint pos_loc = shader.getAttribLocation( "vPosition" );
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
	glEnableVertexAttribArray( pos_loc );
	glVertexAttribPointer( pos_loc, 2, GL_FLOAT, GL_FALSE, 0, 0 );

	GLuint clr_loc = shader.getAttribLocation( "vColor" );
	glBindBuffer( GL_ARRAY_BUFFER, colorVBO );
	glEnableVertexAttribArray( clr_loc );
	glVertexAttribPointer( clr_loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	
	glBindVertexArray( vao );


	// camera Matrices Initialization
	camera.SetMatrices( shader );

	/*
	// Texture Settings
	const char* texName = "texture/check_bw.jpg";
	QImage texImg = QGLWidget::convertToGLFormat( QImage( texName, "JPG" ) );
	// Copy to OpenGL
	glActiveTexture( GL_TEXTURE0 );
	GLuint texId;
	glGenTextures( 1, &texId );
	glBindTexture( GL_TEXTURE_2D, texId );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texImg.width(),
				texImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texImg.bits() );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	// sampler uniform settings
	int loc = glGetUniformLocation( shader.getHandle(), "Tex1" );
	if ( loc >= 0 )	glUniform1i( loc, 0 );
	else fprintf( stderr, "Uniform Variable \"Tex1\" Not Found!\n" );
	*/
	
}


void SceneTextureTest::update( float t )
{
}


void SceneTextureTest::render()
{
	glClear( GL_COLOR_BUFFER_BIT );//| GL_DEPTH_BUFFER_BIT );

	camera.SetCameraPosition( GLPosition( 20 * sin( 0.0 ), 20, 20 * cos( 0.0 ) ) );
	
	//camera.SetMatrices( shader );
	
	camera.updataMatrices();

	
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glutSwapBuffers();
}


void SceneTextureTest::resize( int w, int h )
{
}