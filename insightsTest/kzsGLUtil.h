#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>


#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/OpenGL.h>
#elif defined _WIN32 || defined _WIN64
	#include <GL\glut.h>
#endif 


// classes
namespace kzsGL{
	class kzsGLUtil;
	class GLColor;
	class GLPosition;
}


namespace kzsGL{

	// index names
	static const int R = 0;
	static const int G = 1;
	static const int B = 2;
	static const int A = 3;
	static const int X = 0;
    static const int Y = 1;
    static const int Z = 2;
	static const int W = 3;



	// **kzsGLUtil**
	class kzsGLUtil
	{
	public:
		kzsGLUtil(void);
		~kzsGLUtil(void);

		void RenderFramerate( int window_width, int window_height );
		void setOrthoForFont( int WINDOW_WIDTH, int WINDOW_HEIGHT );
		void RenderSpacedBitmapString( int x, int y, int spacing, void* font, char* string );
		void ResetPerspectiveProjection();
		void drawAxes();
		void drawGrid( int GRID_SIZE );
		void drawGrid( int GRID_SIZE, GLColor clr );
		void toggleFaceDirection();

		int   startTime, totalFrames;
		float fps;
		char  buffer[ 16384 ];
	};


	// **GLColor**
	class GLColor
	{
	private:
		
		GLfloat color_[ 4 ];

	public:
		GLColor(void);
		GLColor( GLfloat r, GLfloat g, GLfloat b ); //アルファ値を指定しない場合は1.0
		GLColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
		~GLColor(void);

		void showValue();

		GLfloat r();
		GLfloat g();
		GLfloat b();
		GLfloat a();

		GLfloat* pointer();
		
	};

	
	// **GLPosition**
	class GLPosition
	{
	private:
		GLfloat position_[ 4 ];
	public:
		GLPosition();
		GLPosition( GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f );
		~GLPosition();

		GLPosition  operator+ ( const GLPosition& rhs );
		GLPosition& operator+=( const GLPosition& rhs );
		GLPosition  operator* ( const GLfloat& scale );
		GLPosition& operator*=( const GLfloat& scale );

		void scaleXYZ( GLfloat scale ); // value of "w" will not be manipulated
		void setPosition( GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f );
		void showValue();

		GLfloat& x();
		GLfloat& y();
		GLfloat& z();
		GLfloat& w();

		GLfloat* pointer();

	};

}