#include "kzsGLUtil.h"


namespace kzsGL{
//===============================================================================
// kzsGLUtil class

kzsGLUtil::kzsGLUtil(void)
{
	startTime	= 0;
	totalFrames = 0;
	fps			= 0;
}

 
kzsGLUtil::~kzsGLUtil(void)
{
}


void kzsGLUtil::RenderFramerate( int window_width, int window_height )
{
	totalFrames++;

	int current = glutGet( GLUT_ELAPSED_TIME );
	
	if ( ( current - startTime ) > 1000 )
	{
		float elapsedTime = float ( current - startTime );
		fps = ( ( totalFrames * 1000.0f ) / elapsedTime );
		startTime = current;
		totalFrames = 0;
	}


	sprintf_s( buffer, " *** ROTATE WITH MOUSE RIGHT_BUTTON  |  FPS: %3.2f",fps);

	setOrthoForFont( window_width, window_height );
	glColor3f( 1, 1, 1 );
	RenderSpacedBitmapString( 20, 20, 0, GLUT_BITMAP_HELVETICA_12, buffer );
	ResetPerspectiveProjection();
}


void kzsGLUtil::setOrthoForFont( int WINDOW_WIDTH, int WINDOW_HEIGHT )
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0, WINDOW_WIDTH, 0, WINDOW_HEIGHT );
	glScalef( 1, -1, 1 );
	glTranslatef( 0, -WINDOW_HEIGHT, 0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}


void kzsGLUtil::RenderSpacedBitmapString( int x, int y, int spacing, void* font, char* string )
{
	char*	c;
	int		x1 = x;
	for ( c = string; *c != '\0'; c++ )
	{
		glRasterPos2i( x1, y );
		glutBitmapCharacter( font, *c );
		x1 = x1 + glutBitmapWidth( font, *c ) + spacing;
	}
}


void kzsGLUtil::ResetPerspectiveProjection()
{
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
}



void kzsGLUtil::drawAxes()
{
	glPushMatrix();
	glColor3f( 0, 0, 1 );
	glPushMatrix();
	glTranslatef( 0, 0, 0.8f );
	glutSolidCone( 0.0325, 0.2, 4, 1 );
	// Draw label Z
	glTranslatef(0,0.0625,0.225f);
	RenderSpacedBitmapString(0,0,0,GLUT_BITMAP_HELVETICA_10, "Z");
	glPopMatrix();
	glutSolidCone(0.0225,1, 4,1);


	glColor3f( 1, 0, 0 );
	glRotatef( 90, 0, 1, 0 );
	glPushMatrix();
	glTranslatef( 0, 0, 0.8f );
	glutSolidCone( 0.0325, 0.2, 4, 1 );
	// Draw label X
	glTranslatef( 0, 0.0625, 0.225f );
	RenderSpacedBitmapString( 0, 0, 0, GLUT_BITMAP_HELVETICA_10, "X" );
	glPopMatrix();
	glutSolidCone( 0.0225, 1, 4, 1 );

	glColor3f( 0, 1, 0 );
	glRotatef( 90, -1, 0, 0 );
	glPushMatrix();
	glTranslatef( 0, 0, 0.8f );
	glutSolidCone( 0.0325, 0.2, 4, 1 );
	// Draw label Y
	glTranslatef( 0, 0.0625, 0.225f );
	RenderSpacedBitmapString( 0, 0, 0, GLUT_BITMAP_HELVETICA_10, "Y" );
	glPopMatrix();
	glutSolidCone( 0.0225, 1, 4, 1 );
	glPopMatrix();
}


void kzsGLUtil::drawGrid( int GRID_SIZE )
{
	glBegin( GL_LINES );

	for (int i = -GRID_SIZE; i <= GRID_SIZE; i++)
	{
		glVertex3f( ( float ) i, 0, ( float ) -GRID_SIZE );
		glVertex3f( ( float ) i, 0, ( float )  GRID_SIZE );

		glVertex3f( ( float ) -GRID_SIZE, 0, ( float ) i );
		glVertex3f( ( float )  GRID_SIZE, 0, ( float ) i );
	}
	glEnd();
}


void kzsGLUtil::drawGrid( int GRID_SIZE, GLColor clr )
{
	glBegin( GL_LINES );
	glColor3fv( clr.pointer() );

	for (int i = -GRID_SIZE; i <= GRID_SIZE; i++)
	{
		glVertex3f( ( float ) i, 0, ( float ) -GRID_SIZE );
		glVertex3f( ( float ) i, 0, ( float )  GRID_SIZE );

		glVertex3f( ( float ) -GRID_SIZE, 0, ( float ) i );
		glVertex3f( ( float )  GRID_SIZE, 0, ( float ) i );
	}
	glEnd();
}


void kzsGLUtil::toggleFaceDirection()
{
	static int face_dir = GL_CCW; // default state is "GL_CCW" in OpenGL
	face_dir = ( face_dir == GL_CCW ? GL_CW : GL_CCW );
	glFrontFace( face_dir );
}


//===============================================================================
//===============================================================================
// GLColor class

GLColor::GLColor()
{
	this->color_[ R ] = 0.0;
	this->color_[ G ] = 0.0;
	this->color_[ B ] = 0.0;
	this->color_[ A ] = 1.0;
}


GLColor::GLColor( GLfloat r, GLfloat g, GLfloat b )
{
	this->color_[ R ] = r;
	this->color_[ G ] = g;
	this->color_[ B ] = b;
	this->color_[ A ] = 1.0;
}


GLColor::GLColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	this->color_[ R ] = r;
	this->color_[ G ] = g;
	this->color_[ B ] = b;
	this->color_[ A ] = a;
}


GLColor::~GLColor()
{
}

void GLColor::showValue()
{
	std::cout << std::fixed << std::setprecision( 4 )
			  << "R:" << std::setw( 4 ) << this->color_[ R ] << ", "
		      << "G:" << std::setw( 4 ) << this->color_[ G ] << ", "
			  << "B:" << std::setw( 4 ) << this->color_[ B ] << ", "
			  << "A:" << std::setw( 4 ) << this->color_[ A ] 
			  << std::resetiosflags( std::ios::fixed ) << std::endl;
}

GLfloat GLColor::r() { return color_[ R ]; }

GLfloat GLColor::g() { return color_[ G ]; }

GLfloat GLColor::b() { return color_[ B ]; }

GLfloat GLColor::a() { return color_[ A ]; }

GLfloat* GLColor::pointer() { return color_; }

//===============================================================================
//===============================================================================
// GLPosition class

GLPosition::GLPosition()
{
	this->position_[ X ] = 0.0f;
	this->position_[ Y ] = 0.0f;
	this->position_[ Z ] = 0.0f;
	this->position_[ W ] = 1.0f;
}


GLPosition::GLPosition( GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
	this->position_[ X ] = x;
	this->position_[ Y ] = y;
	this->position_[ Z ] = z;
	this->position_[ W ] = w;
}


GLPosition::~GLPosition()
{
}


GLPosition GLPosition::operator+( const GLPosition& rhs )
{
	GLPosition tmp;
	tmp.setPosition( position_[ X ] + rhs.position_[ X ],
		             position_[ Y ] + rhs.position_[ Y ],
					 position_[ Z ] + rhs.position_[ Z ],
					 position_[ W ] + rhs.position_[ W ] );
	return tmp;
}


GLPosition& GLPosition::operator+=( const GLPosition& rhs )
{
	position_[ X ] += rhs.position_[ X ];
	position_[ Y ] += rhs.position_[ Y ];
	position_[ Z ] += rhs.position_[ Z ];
	position_[ W ] += rhs.position_[ W ];
	
	return *this;
}


GLPosition GLPosition::operator*( const GLfloat& scale )
{
	GLPosition tmp;
	tmp.setPosition( position_[ X ] * scale,
		             position_[ Y ] * scale,
					 position_[ Z ] * scale,
					 position_[ W ] * scale );
	return tmp;
}



GLPosition& GLPosition::operator*=( const GLfloat& scale )
{
	position_[ X ] *= scale;
	position_[ Y ] *= scale;
	position_[ Z ] *= scale;
	position_[ W ] *= scale;

	return *this;
}


void GLPosition::scaleXYZ( GLfloat scale )
{
	position_[ X ] *= scale;
	position_[ Y ] *= scale;
	position_[ Z ] *= scale;
}


void GLPosition::setPosition( GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
	this->position_[ X ] = x;
	this->position_[ Y ] = y;
	this->position_[ Z ] = z;
	this->position_[ W ] = w;
}


void GLPosition::showValue()
{
	std::cout << std::fixed << std::setprecision( 4 )
			  << "X:" << this->position_[ X ] << ", "
		      << "Y:" << this->position_[ Y ] << ", "
			  << "Z:" << this->position_[ Z ] << ", "
			  << "W:" << this->position_[ W ] 
			  << std::resetiosflags( std::ios::fixed ) << std::endl;
}

GLfloat& GLPosition::x() { return this->position_[ X ]; }

GLfloat& GLPosition::y() { return this->position_[ Y ]; }

GLfloat& GLPosition::z() { return this->position_[ Z ]; }

GLfloat& GLPosition::w() { return this->position_[ W ]; }

GLfloat* GLPosition::pointer() { return position_; }


} // end of namespace "kzsGL"