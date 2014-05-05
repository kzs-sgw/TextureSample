#include "sceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/projection.hpp>



sceneCamera::sceneCamera(void)
{
	modelview  = glm::mat4( 1.0f );
	projection = glm::mat4( 1.0f );

	cameraPos = vec3( 5.0f, 5.0f, 5.0f );
	targetPos = vec3( 0.0f, 0.0f, 0.0f );
	viewUpVec = vec3( 0.0f, 1.0f, 0.0f );

	oldX  = 0;
	oldY  = 0;
	camera_state = 0;
	rX    = 0;
	rY    = -50;
	dist  = 1;
}


sceneCamera::~sceneCamera(void)
{
}


void sceneCamera::SetCameraPosition( vec3 cPos )
{
	cameraPos = cPos;
}

void sceneCamera::SetTargetPosition( vec3 tPos )
{
	targetPos = tPos;
}

void sceneCamera::SetViewUpVector( vec3 vuVec )
{
	viewUpVec = vuVec;
}

void sceneCamera::SetModelViewMatrix( kzsGLSL shader, const char* varname )
{
	modelview = glm::mat4( 1.0f ); // LoadIdentity
	modelview = glm::translate( modelview, vec3( 0.0, 0.0, dist ) );
		
	modelview *= glm::lookAt( cameraPos, targetPos, viewUpVec );
		
	// viewUpVecの値次第で変化させる必要あり。（下式はY軸上方向固定で成り立つ）
	modelview = glm::rotate( modelview, rX, vec3( 1, 0, 0 ) );
	modelview = glm::rotate( modelview, rY, vec3( 0, 1, 0 ) );

		shader.setUniform( varname, modelview ); // ModelViewMatrix
}

void sceneCamera::SetModelViewProjectionMatrix( kzsGLSL shader, const char* varname )
{ 
	modelview = glm::mat4( 1.0f ); // LoadIdentity
	modelview = glm::translate( modelview, vec3( 0.0, 0.0, dist ) );
	
	modelview *= glm::lookAt( cameraPos, targetPos, viewUpVec );
	
	// viewUpVecの値次第で変化させる必要あり。（下式はY軸上方向固定で成り立つ）
	modelview = glm::rotate( modelview, rX, vec3( 1, 0, 0 ) );
	modelview = glm::rotate( modelview, rY, vec3( 0, 1, 0 ) );

	shader.setUniform( varname, projection * modelview ); // ModelViewProjectionMatrix
}

void sceneCamera::SetMatrices( kzsGLSL shader )
{
	/*
	NEED TO DEFINE THESE VARIABLES IN SHADER PROGRAM FILE:
	------------------------------------------------------
	uniform cameraMat
	{
	  mat4 ModelViewMatrix;
	  mat4 ModelViewProjectionMatrix;
	  mat3 NormalMatrix;
	};
	------------------------------------------------------
	*/
	GLuint progHandle = shader.getHandle();
	blockIndex = glGetUniformBlockIndex( progHandle, "cameraMat" );
	//GLint blockSize;
	glGetActiveUniformBlockiv( progHandle, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize );
	blockBuffer = ( GLubyte* ) malloc( blockSize );
		// 各ブロック変数のオフセットを問い合わせる
	const GLchar* names[] = { "ModelViewMatrix", "ModelViewProjectionMatrix", "NormalMatrix" };
	GLuint indices[ 3 ];
	glGetUniformIndices( progHandle, 3, names, indices );
		//GLint offset[ 3 ];
	glGetActiveUniformsiv( progHandle, 3, indices, GL_UNIFORM_OFFSET, offset );
		// ModelViewMatrix Data
	modelview = glm::mat4( 1.0f ); // LoadIdentity
	modelview = glm::translate( modelview, vec3( 0.0, 0.0, dist ) );
	
	modelview *= glm::lookAt( cameraPos, targetPos, viewUpVec );
	
		// viewUpVecの値次第で変化させる必要あり。（下式はY軸上方向固定で成り立つ）
	modelview = glm::rotate( modelview, rX, vec3( 1, 0, 0 ) );
	modelview = glm::rotate( modelview, rY, vec3( 0, 1, 0 ) ); 
	
	// ModelViewProjectionMatrix Data
	glm::mat4 MVP = projection * modelview;
		// NormalMatrix Data
	glm::mat3 normal = glm::mat3( vec3( modelview[0] ), vec3( modelview[1] ), vec3( modelview[2] ) );

	memcpy( blockBuffer + offset[ 0 ], &modelview, sizeof( glm::mat4 ) );
	memcpy( blockBuffer + offset[ 1 ], &MVP      , sizeof( glm::mat4 ) );
	memcpy( blockBuffer + offset[ 2 ], &normal   , sizeof( glm::mat3 ) );
		//GLuint uboHandle;
	glGenBuffers( 1, &uboHandle );
	glBindBuffer( GL_UNIFORM_BUFFER, uboHandle );
	glBufferData( GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW );
	glBindBufferBase( GL_UNIFORM_BUFFER, blockIndex, uboHandle );
}

void sceneCamera::updataMatrices()
{
	glBindBuffer( GL_UNIFORM_BUFFER, uboHandle );
	
	// ModelViewMatrix Data
	modelview = glm::mat4( 1.0f ); // LoadIdentity
	modelview = glm::translate( modelview, vec3( 0.0, 0.0, dist ) );
	
	modelview *= glm::lookAt( cameraPos, targetPos, viewUpVec );
	
	// viewUpVecの値次第で変化させる必要あり。（下式はY軸上方向固定で成り立つ）
	modelview = glm::rotate( modelview, rX, vec3( 1, 0, 0 ) );
	modelview = glm::rotate( modelview, rY, vec3( 0, 1, 0 ) ); 
	
	// ModelViewProjectionMatrix Data
	glm::mat4 MVP = projection * modelview;
		// NormalMatrix Data
	glm::mat3 normal = glm::mat3( vec3( modelview[0] ), vec3( modelview[1] ), vec3( modelview[2] ) );

	memcpy( blockBuffer + offset[ 0 ], &modelview, sizeof( glm::mat4 ) );
	memcpy( blockBuffer + offset[ 1 ], &MVP      , sizeof( glm::mat4 ) );
	memcpy( blockBuffer + offset[ 2 ], &normal   , sizeof( glm::mat3 ) );
	glBufferSubData( GL_UNIFORM_BUFFER, blockIndex, blockSize, blockBuffer );
		// 例外チェック(動作不具合)
	/*
	int bufferforCheck = 0;
	glGetBufferParameteriv( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferforCheck );
	if ( blockSize != bufferforCheck )
	{
		std::cerr<< "Can not Create UBO\n";
		glDeleteBuffers( 1, &uboHandle );
		exit(0);
	}*/
}

void sceneCamera::InReshape( int nw, int nh, GLdouble persAngular )
{
	glViewport(0,0,nw, nh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projection = glm::mat4( 1.0 ); // LoadIdentity
	projection = glm::perspective<float>( persAngular, (GLfloat)nw / (GLfloat)nh, 0.1f, 1000.0f );
	glMatrixMode(GL_MODELVIEW);
}

void sceneCamera::InMouse( int button, int state, int x, int y )
{
	if ( state == GLUT_DOWN )
	{
		oldX = x;
		oldY = y;
	}
		if( button == GLUT_RIGHT_BUTTON )
		camera_state = 1;
	else camera_state = 0;
}

void sceneCamera::InMotion( int x, int y, bool doRedisplay )
{
	if ( camera_state == 1 )
	{
		rY += ( x - oldX )/5.0f;
		rX += ( y - oldY )/5.0f;
	}
		oldX = x;
	oldY = y;
	if ( doRedisplay )	glutPostRedisplay();
}

	void sceneCamera::InMouseWheel( int wheel_number, int direction, int x, int y )
{
	if ( direction ==  1 ) ZoomIn();
	if ( direction == -1 ) ZoomOut();
}
	void sceneCamera::ZoomIn()	 { dist += 1.0; }
void sceneCamera::ZoomOut() { dist -= 1.0; }

void sceneCamera::showValues()
{
	std::cout<< "oldX: " << oldX <<std::endl;
	std::cout<< "oldY: " << oldY <<std::endl;
	std::cout<< "rX  : " << rX   <<std::endl;
	std::cout<< "rY  : " << rY   <<std::endl;
	std::cout<< "dist: " << dist <<std::endl;
	std::cout<<std::endl;
}


void sceneCamera::SetModelViewInv( kzsGLSL shader, const char* varname )
{
	mat4 mvi = glm::inverse( modelview );
	shader.setUniform( varname, mvi );
}


vec3 sceneCamera::getCameraPosition()
{
	// 見かけのカメラ位置を求める。
	vec3 cPos = vec3( glm::inverse( modelview ) * vec4(0.0f,0.0f,0.0f,1.0f) );
	//std::cout<<cPos.x<<" "<<cPos.y<<" "<<cPos.z<<std::endl;
	return cPos;
}
	
