#pragma once

#include "kzsGLUtil.h"

namespace kzsGL
{

class GLLight
{
private:
	GLColor ambient_;
	GLColor diffuse_;
	GLColor specular_;

	GLPosition position_;
	GLenum lightNum_; // GL_LIGHTn

public:
	GLLight( GLenum GL_LIGHTn );
	GLLight( GLenum GL_LIGHTn, GLColor ambient, GLColor diffuse, GLColor specular, GLPosition position );
	~GLLight(void);


	void			SetAmbient ( float r, float g, float b, float a );
	void			SetAmbient ( GLColor ambient );
	void			SetDiffuse ( float r, float g, float b, float a );
	void			SetDiffuse ( GLColor diffuse );
	void			SetSpecular( float r, float g, float b, float a );
	void			SetSpecular( GLColor specular );
	void			SetPosition( float x, float y, float z, float w );
	void			SetPosition( GLPosition position );
	void			SetParams( GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat*   position );
	void			SetParams( GLColor  ambient, GLColor  diffuse, GLColor  specular, GLPosition position );
	void			showLightInfo();


};

} // end of namespace "kzsGL"