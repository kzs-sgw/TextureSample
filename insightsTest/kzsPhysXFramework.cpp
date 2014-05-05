#include "kzsPhysXFramework.h"

/////////////////////////////////////////////////////////////////
// GLOBALS

static PxDefaultErrorCallback    gDefaultErrorCallback;
static PxDefaultAllocator        gDefaultAllocatorCallback;
static PxSimulationFilterShader  gDefaultFilterShader = physx::PxDefaultSimulationFilterShader;

/////////////////////////////////////////////////////////////////




kzsPhysXFramework::kzsPhysXFramework(void)
{
	// global variables	
	gScene = 0;
	myTimestep = 1.0f/60.0f;
}


kzsPhysXFramework::~kzsPhysXFramework(void)
{
}




// **StepPhysX**
void kzsPhysXFramework::StepPhysX() {
	gScene->simulate( myTimestep );

	while ( !gScene->fetchResults() )
	{
		// do something useful
	}
}


// **initPx_Foundation_Physics_Extensions**
void kzsPhysXFramework::initPx_Foundation_Physics_Extensions()
{
	printf( "creating Foundation\n" );
	// create foundation object with default error and allocator callbacks.
	mFoundation = PxCreateFoundation( PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback );
	if ( !mFoundation ) cerr << "PxCreateFoundation failed!" << endl;

	
	printf( "creating Physics\n" );
	// create Physics oject with the created foundation and with a 'default' scale tolerance.
	gPhysicsSDK = PxCreatePhysics( PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale() );
	if ( gPhysicsSDK == NULL )
	{
		cerr << "Error creating PhysX3 divice." << endl;
		cerr << "Existing..." << endl;
		exit( 1 );
	}
	
	PxInitExtensions( *gPhysicsSDK );
	if ( !PxInitExtensions( *gPhysicsSDK ) )cerr << "PxInitExtensions failed!" << endl;
}


// **initPx_CreateScene**
void kzsPhysXFramework::initPx_CreateScene( PxVec3 _gravity )
{
	printf( "creating the scene\n" );
	// create the scene
	PxSceneDesc sceneDesc( gPhysicsSDK->getTolerancesScale() );
	sceneDesc.gravity = _gravity;

	if ( !sceneDesc.cpuDispatcher )
	{
		PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}
	
	if ( !sceneDesc.filterShader )
	{
		sceneDesc.filterShader = gDefaultFilterShader;
	}
	
	gScene = gPhysicsSDK->createScene( sceneDesc );
	if ( !gScene ) cerr << "createScene failed!" << endl;
}


// **initPhysX**
void kzsPhysXFramework::initPhysX() {
	
	printf( "**initializing PhysX**\n" );
	
	initPx_Foundation_Physics_Extensions();
	
	initPx_CreateScene( PxVec3( 0.0f, -9.81f, 0.0f ) );
	
	printf( "**PhysX initialized**\n\n" );
}


// **ShutdownPhysX**
void kzsPhysXFramework::ShutdownPhysX() {
	printf( "shutting down\n" );
	gScene->release();
	gPhysicsSDK->release();
}


// **OnShutdown**
void kzsPhysXFramework::OnShutdown() {
	ShutdownPhysX();
}


// **OnRender**
void kzsPhysXFramework::OnRender() {
	
	if ( gScene ) StepPhysX();

	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	glutSwapBuffers();
}