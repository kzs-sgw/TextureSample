#pragma once

#include "kzsGLUtil.h"

namespace kzsGL
{

class GLMaterial
{
private:
	GLenum face_; // default GL_FRONT

public:
	GLMaterial(void);
	GLMaterial( GLenum face );
	GLMaterial( GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat shininess = 100.0f );
	GLMaterial( GLColor  ambient, GLColor  diffuse, GLColor  specular, GLfloat shininess = 100.0f );
	GLMaterial( GLenum face, GLfloat* ambient, GLfloat* diffuse,
		                     GLfloat* specular, GLfloat shininess = 100.0f );
	GLMaterial( GLenum face, GLColor  ambient, GLColor  diffuse,
		                     GLColor  specular, GLfloat shininess = 100.0f );
	~GLMaterial(void);
	
	void SetFaceType( GLenum face );
	void ActivateMaterial( GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat shininess = 100.0f );
	void ActivateMaterial( GLColor  ambient, GLColor  diffuse, GLColor  specular, GLfloat shininess = 100.0f );
};


} // end of namespace "kzsGL"