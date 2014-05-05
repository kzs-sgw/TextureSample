#include "GLLight.h"

namespace kzsGL
{


GLLight::GLLight( GLenum GL_LIGHTn )
{
	glEnable( GL_LIGHTING );
	lightNum_ = GL_LIGHTn;
	glEnable ( lightNum_ );

	SetAmbient (  ambient_ );
	SetDiffuse (  diffuse_ );
	SetSpecular( specular_ );
	SetPosition( position_ );

}


GLLight::GLLight( GLenum GL_LIGHTn, GLColor ambient, GLColor diffuse, GLColor specular, GLPosition position )
{
	glEnable( GL_LIGHTING );
	lightNum_ = GL_LIGHTn;
	glEnable ( lightNum_ );

	SetAmbient (  ambient_ );
	SetDiffuse (  diffuse_ );
	SetSpecular( specular_ );
	SetPosition( position_ );
}


GLLight::~GLLight(void)
{
}


void GLLight::SetAmbient( float r, float g, float b, float a )
{
	ambient_ = GLColor( r, g, b, a );
	glLightfv( lightNum_, GL_AMBIENT, ambient_.pointer() );
}


void GLLight::SetAmbient( GLColor ambient )
{
	ambient_ = ambient;
	glLightfv( lightNum_, GL_AMBIENT, ambient_.pointer() );
}


void GLLight::SetDiffuse( float r, float g, float b, float a )
{
	diffuse_ = GLColor( r, g, b, a );
	glLightfv( lightNum_, GL_DIFFUSE, diffuse_.pointer() );
}


void GLLight::SetDiffuse( GLColor diffuse )
{
	diffuse_ = diffuse;
	glLightfv( lightNum_, GL_DIFFUSE, diffuse_.pointer() );
}


void GLLight::SetSpecular( float r, float g, float b, float a )
{
	specular_ = GLColor( r, g, b, a );
	glLightfv( lightNum_, GL_SPECULAR, specular_.pointer() );
}


void GLLight::SetSpecular( GLColor specular )
{
	specular_ = specular;
	glLightfv( lightNum_, GL_SPECULAR, specular_.pointer() );
}


void GLLight::SetPosition( float x, float y, float z, float w )
{
	position_.setPosition( x, y, z, w );
	glLightfv( lightNum_, GL_POSITION, position_.pointer() );
}


void GLLight::SetPosition( GLPosition position )
{
	position_ = position;
	glLightfv( lightNum_, GL_POSITION, position_.pointer() );
}

void GLLight::SetParams( GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat* position )
{
	ambient_  = GLColor   ( ambient [ 0 ], ambient [ 1 ], ambient [ 2 ], ambient [ 3 ] );
	diffuse_  = GLColor   ( diffuse [ 0 ], diffuse [ 1 ], diffuse [ 2 ], diffuse [ 3 ] );
	specular_ = GLColor   ( specular[ 0 ], specular[ 1 ], specular[ 2 ], specular[ 3 ] );
	position_ = GLPosition( position[ 0 ], position[ 1 ], position[ 2 ], position[ 3 ] );

	glLightfv( lightNum_, GL_AMBIENT , ambient_ .pointer() );
	glLightfv( lightNum_, GL_DIFFUSE , diffuse_ .pointer() );
	glLightfv( lightNum_, GL_SPECULAR, specular_.pointer() );
	glLightfv( lightNum_, GL_POSITION, position_.pointer() );
}


void GLLight::SetParams( GLColor ambient, GLColor diffuse, GLColor specular, GLPosition position )
{
	ambient_  = ambient;
	diffuse_  = diffuse;
	specular_ = specular;
	position_ = position;

	glLightfv( lightNum_, GL_AMBIENT , ambient_ .pointer() );
	glLightfv( lightNum_, GL_DIFFUSE , diffuse_ .pointer() );
	glLightfv( lightNum_, GL_SPECULAR, specular_.pointer() );
	glLightfv( lightNum_, GL_POSITION, position_.pointer() );
}


void GLLight::showLightInfo()
{
	std::cout << ":::GL_LIGHT" << lightNum_ - 16384 << ":::" << std::endl;
	printf( "Ambient::: " );  ambient_.showValue();
	printf( "Diffuse::: " );  diffuse_.showValue();
	printf( "Specular:: " ); specular_.showValue();
	printf( "Position:: " ); position_.showValue();
	printf( "\n" );
}



} // end of namespace "kzsGL"