/* メインとなるワークスペースです。OpenGLで呼び出される関数を定義します。 */



#include "kzsGLObject.h"


// **DisplayFunc**
void kzsGLObject::display( void )
{
}


// **ReshapeFunc**
void kzsGLObject::resize( int w, int h )
{
}


// **MouseFunc**
void kzsGLObject::mouse( int button, int state, int x, int y )
{
}


// **MotionFunc**
void kzsGLObject::motion( int x, int y )
{
}


// **KeyboardFunc**
void kzsGLObject::keyboard( unsigned char key, int x, int y )
{
	switch ( key ) {
		case '\033':
			exit( 0 );

		case 'n':
			glUtil.toggleFaceDirection();

		default:
			break;
	}
}


// **IdleFunc**
void kzsGLObject::idle()
{
	glutPostRedisplay();
}


// **MouseWheelFunc**
void kzsGLObject::mousewheel( int wheel_number, int direction, int x, int y )
{
	 printf("WheelNumber:%d,  direction:%d, at (%d, %d)\n",wheel_number,direction,x,y);
}

// **Initialization**
void kzsGLObject::initGL( void )
{
	//glClearColor( .7f, .7f, .7f, 1.0f);
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	//glEnable( GL_DEPTH_TEST );
	//glDepthMask( GL_TRUE );

	if ( isGlewActive )
	{
		GLenum err = glewInit();
		if ( err != GLEW_OK ) printf( "Error: %s\n", glewGetErrorString( err ) );
		printf( "GLSL Activated\n" );
	}

}


// **kzsGlutInit**
void kzsGLObject::kzsGlutInit( void )
{
	//glutInitWindowPosition( 400, 400 );
	glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	
}


// **ActivateGLSL**
void kzsGLObject::ActivateGLSL()
{
	isGlewActive = true;
}



// **************************************