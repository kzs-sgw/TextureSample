// This class is your Application
// Add code to this file
// 
// display() is called as the DisplayFunction in main.cpp
// OnRender() includes PhysX functions with OpenGL
// make sure OnRender() is called in display()
//
// namespace "physx", "std", "kzsGL" are activated
// 
//

#pragma once

#include "kzsGLSL.h"
#include "kzsPhysXFramework.h"
#include "kzsGLObject.h"
#include "DrawPxShapes.h"
#include "GLCamera.h"
#include "GLLight.h"
#include "GLMaterial.h"
#include "sceneCamera.h"

#include <glm/glm.hpp>

//#include "GLOBAL_VARIABLES.h"

//+++++++++++++++++++++++++++++++
// add includes
#include "SceneTextureTest.h"
//+++++++++++++++++++++++++++++++


using glm::vec2;
using glm::vec3;

class PxUserApp : public kzsPhysXFramework,
	public kzsGLObject
{
public:
	PxUserApp();
	~PxUserApp(void);

	// Rendering Functions
	void			resize( int nw, int nh );
	void			mouse ( int button, int s, int x, int y );
	void			motion( int x, int y );
	virtual void	display();
	void			idle();
	void			keyboard( unsigned char key, int x, int y );
	void			mousewheel( int wheel_number, int direction, int x, int y );
	virtual void	initGL();

	// PhysX Functions
	void			initPhysX();

	//User Functions

	// Variables
	const PxVec3	globalGravity;

	kzsGLSL			testShader;
	sceneCamera		camera;

	

};

