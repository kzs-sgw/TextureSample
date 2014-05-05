#include "GLMaterial.h"

namespace kzsGL
{

GLMaterial::GLMaterial(void)
{
	face_ = GL_FRONT;
	GLColor ambient, diffuse, specular;
	ActivateMaterial( ambient, diffuse, specular );
}


GLMaterial::GLMaterial( GLenum face )
{
	face_ = face;
	GLColor ambient, diffuse, specular;
	ActivateMaterial( ambient, diffuse, specular );
}


GLMaterial::GLMaterial( GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat shininess )
{
	face_ = GL_FRONT;
	ActivateMaterial( ambient, diffuse, specular, shininess );
}


GLMaterial::GLMaterial( GLColor  ambient, GLColor  diffuse, GLColor  specular, GLfloat shininess )
{
	face_ = GL_FRONT;
	ActivateMaterial( ambient, diffuse, specular, shininess );
}


GLMaterial::GLMaterial( GLenum face, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat shininess )
{
	face_ = face;
	ActivateMaterial( ambient, diffuse, specular, shininess );
}


GLMaterial::GLMaterial( GLenum face, GLColor  ambient, GLColor  diffuse, GLColor  specular, GLfloat shininess )
{
	face_ = face;
	ActivateMaterial( ambient, diffuse, specular, shininess );
}


GLMaterial::~GLMaterial(void)
{
}


void GLMaterial::SetFaceType( GLenum face )
{
	face_ = face;
}


void GLMaterial::ActivateMaterial( GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat shininess )
{
	glMaterialfv( face_, GL_AMBIENT  , ambient   );
	glMaterialfv( face_, GL_DIFFUSE  , diffuse   );
	glMaterialfv( face_, GL_SPECULAR , specular  );
	glMaterialf ( face_, GL_SHININESS, shininess );
}


void GLMaterial::ActivateMaterial( GLColor ambient, GLColor diffuse, GLColor specular, GLfloat shininess)
{
	glMaterialfv( face_, GL_AMBIENT  , ambient .pointer() );
	glMaterialfv( face_, GL_DIFFUSE  , diffuse .pointer() );
	glMaterialfv( face_, GL_SPECULAR , specular.pointer() );
	glMaterialf ( face_, GL_SHININESS, shininess          );
}


} // end of namespace "kzsGL"