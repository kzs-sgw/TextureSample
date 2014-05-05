#include "PxUserApp.h"

//#include <QtOpenGL\qgl.h>



PxUserApp::PxUserApp() : globalGravity( 0.0f, -9.81f, 0.0f )
{	
	initPhysX();
}


PxUserApp::~PxUserApp(void)
{
}


//===============================================================================
// Rendering Functions

// **resize**
void PxUserApp::resize( int nw, int nh )
{
	camera.InReshape( nw, nh, 50 );
}


// **mouse**
void PxUserApp::mouse( int button, int s, int x, int y )
{
	camera.InMouse( button, s, x, y );
}


// **motion**
void PxUserApp::motion( int x, int y )
{
	camera.InMotion( x, y );
}


// **display**
void PxUserApp::display()
{
}


// **idle**
void PxUserApp::idle()
{
	glutPostRedisplay();
}


// **keyboard**
void PxUserApp::keyboard( unsigned char key, int x, int y )
{
	switch ( key ) {
		case '\033':
			exit( 0 );
			break;

		case 'n':
			glUtil.toggleFaceDirection();
			break;

		default:
			break;  
	}
}


// **mousewheel**
void PxUserApp::mousewheel( int wheel_number, int direction, int x, int y )
{
	camera.InMouseWheel( wheel_number, direction, x, y );
}


// **initGL**
void PxUserApp::initGL()
{
}


//===============================================================================
//===============================================================================
// PhysX simulation Functions
void PxUserApp::initPhysX()
{
	printf( "**initializing PhysX**\n" );
	initPx_Foundation_Physics_Extensions();
	initPx_CreateScene( globalGravity );

	//-------------------------------------------------
			// add code to initialize
	//-------------------------------------------------
	
	printf( "**PhysX initialized**\n\n" );
}


//===============================================================================
//===============================================================================
// User Added Funcitons
