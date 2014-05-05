#pragma once

#include "kzsPhysXFramework.h"
#include <foundation/PxMat33.h>

#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/OpenGL.h>
#elif defined _WIN32 || defined _WIN64
	#include <GL\glut.h>
#endif 


class DrawPxShapes
{
public:
	DrawPxShapes(void);
	~DrawPxShapes(void);

	void			getColumnMajor( physx::PxMat33 m, physx::PxVec3 t, float* mat );
	
	void			DrawBox   ( PxShape* pShape );
	void			DrawSphere( PxShape* pShape );
	void			DrawShape ( PxShape* pShape );
	void			DrawActor( PxRigidActor* actor );
};

