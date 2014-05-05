#pragma once

#include "kzsGLSL.h"
#include <stdlib.h>
#include <stdio.h>
#include "kzsGLUtil.h"
#include <glm/glm.hpp>


#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/OpenGL.h>
#elif defined _WIN32 || defined _WIN64
	#include <GL\glut.h>
#endif 


class sceneCamera
{
private:
	GLuint			blockIndex;
	GLuint			uboHandle;
	GLubyte*		blockBuffer;
	GLint			blockSize;
	GLint			offset[ 3 ];

	vec3		cameraPos;
	vec3		targetPos;
	vec3		viewUpVec;
	mat4		modelview;
	mat4		projection;

	int			oldX;
	int			oldY;
	int			camera_state;

	float		rX;
	float		rY;
	float		dist;
	
public:
	sceneCamera(void);
	~sceneCamera(void);

	void			SetCameraPosition( vec3 cPos  );
	void			SetTargetPosition( vec3 tPos  );
	void			SetViewUpVector  ( vec3 vuVec );
	void			SetModelViewMatrix( kzsGLSL shader, const char* varname );
	void			SetModelViewProjectionMatrix( kzsGLSL shader, const char* varname );
	void			SetMatrices( kzsGLSL shader ); // ModelView, ModelViewProjection, Normal
	void			updataMatrices(); // DO NOT CALL BEFORE SetMatrices()
	void			InReshape( int nw, int nh, GLdouble persAngular );
	void			InMouse( int button, int state, int x, int y );
	void			InMotion( int x, int y, bool doRedisplay = true );
	void			InMouseWheel( int wheel_number, int direction, int x, int y );
	void			ZoomIn();
	void			ZoomOut();
	void			showValues();

	void			SetModelViewInv( kzsGLSL shader, const char* varname );
	vec3			getCameraPosition();
};

