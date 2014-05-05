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


namespace kzsGL{

	class GLCamera
	{
	private:
		GLuint			blockIndex;
		GLuint			uboHandle;
		GLubyte*		blockBuffer;
		GLint			blockSize;
		GLint			offset[ 3 ];

		GLPosition		cameraPos;
		GLPosition		targetPos;
		GLPosition		viewUpVec;
		glm::mat4		modelview;
		glm::mat4		projection;

		int				oldX;
		int				oldY;
		int				camera_state;

		float			rX;
		float			rY;
		float			dist;

	public:
		GLCamera(void);
		GLCamera( GLPosition cPos, GLPosition tPos, GLPosition vuVec = GLPosition( 0.0f, 1.0f, 0.0f ) );
		~GLCamera(void);
		

		void			SetCameraPosition( GLPosition cPos  );
		void			SetTargetPosition( GLPosition tPos  );
		void			SetViewUpVector  ( GLPosition vuVec );
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

		mat4			getModelViewInv();
		vec3			getCameraPosition();

	};

}