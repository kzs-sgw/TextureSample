#include "kzsGLSL.h"

kzsGLSL::kzsGLSL(void)
{
}


kzsGLSL::~kzsGLSL(void)
{
}


int kzsGLSL::readShaderSource( GLuint shader, const char* file )
{
	FILE *fp;
	char *buf;
	GLsizei size;

	// Open File
	fp = fopen( file, "rb" );
	if ( !fp ) 
	{
		printf( "do not exist file %s\n", file );
		return -1;
	}

	// move to the end of file
	fseek( fp, 0, SEEK_END );
	size = ftell( fp ); // get file size

	// reserve memory
	buf = ( GLchar* ) malloc( size );
	if ( buf == NULL )
	{
		printf( "could not allocate read buffer\n" );
		return -1;
	}

	// read file from head
	fseek( fp, 0, SEEK_SET );
	fread( buf, 1, size, fp );

	// set program to shader object
	glShaderSource( shader, 1, ( const GLchar** ) &buf, &size );

	free( buf );
	fclose( fp );

	return 0;
}


void kzsGLSL::CompileShader( GLuint shader, const char* errorMessage )
{
	glCompileShader( shader );
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	//printShaderInfoLog( shader );
	if ( compiled == GL_FALSE )
	{
		fprintf( stderr, errorMessage );

		// get error log length
		int logLen, charsWritten;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLen );

		// get error log
		char* infoLog = new char[ logLen ];
		glGetShaderInfoLog( shader, logLen, &charsWritten, infoLog );

		printf( "%s", infoLog );
		delete[] infoLog;


		exit( 1 );
	}
}


void kzsGLSL::CreateProgramObject( bool IsFragExist )
{
	shaderProgram = glCreateProgram();

	glAttachShader( shaderProgram, vertShader );
	if ( IsFragExist )
	{
	glAttachShader( shaderProgram, fragShader );
	}


	glDeleteShader( vertShader );
	if ( IsFragExist )
	{
	glDeleteShader( fragShader );
	}
}


void kzsGLSL::LinkProgram()
{
	glLinkProgram( shaderProgram );
	glGetProgramiv( shaderProgram, GL_LINK_STATUS, &linked );
	//printProgramInfoLog( shaderProgram );
	if ( linked == GL_FALSE )
	{
		fprintf( stderr, "Link error.\n" );
		exit( 1 );
	}
}


int kzsGLSL::getUniformLocation( const char* varname )
{
	return glGetUniformLocation( shaderProgram, varname );
}


void kzsGLSL::initShaderProgram( const char* vertFile )
{
	vertShader = glCreateShader( GL_VERTEX_SHADER );
	 
	if ( readShaderSource( vertShader, vertFile ) ) exit( 1 );
	
	CompileShader( vertShader, "Compile error in vertex shader.\n\n" );
	
	CreateProgramObject( FRAG_NONE );
	LinkProgram();
}


void kzsGLSL::initShaderProgram( const char* vertFile, const char* fragFile )
{	
	vertShader = glCreateShader( GL_VERTEX_SHADER );
	fragShader = glCreateShader( GL_FRAGMENT_SHADER );

	 
	if ( readShaderSource( vertShader, vertFile ) ) exit( 1 );
	if ( readShaderSource( fragShader, fragFile ) ) exit( 1 );

	
	CompileShader( vertShader, "Compile error in vertex shader.\n\n" );
	CompileShader( fragShader, "Compile error in fragment shader.\n\n" );
	
	CreateProgramObject( FRAG_EXIST );
	LinkProgram();
}


void kzsGLSL::BeginShader()
{
	glUseProgram( shaderProgram );
}


void kzsGLSL::EndShader()
{
	glUseProgram( 0 );
}


int kzsGLSL::getHandle()
{
	return shaderProgram;
}


int kzsGLSL::getAttribLocation( const char* varname )
{
	return glGetAttribLocation( shaderProgram, varname );
}


void kzsGLSL::bindAttribLocation( GLuint location, const char* varname )
{
	glBindAttribLocation( shaderProgram, location, varname );
}


void kzsGLSL::bindFragDataLocation( GLuint location, const char* varname )
{
	glBindFragDataLocation( shaderProgram, location, varname );
}

//****Uniform setter****

void kzsGLSL::setUniform( const char* varname, float x, float y, float z )
{
	int loc = getUniformLocation( varname );
	if ( loc >= 0 )
	{
		glUniform3f( loc, x, y, z );
	}
}


void kzsGLSL::setUniform( const char* varname, const vec3 & v )
{
	this->setUniform( varname, v.x, v.y, v.z );
}


void kzsGLSL::setUniform( const char* varname, const vec4 & v )
{
	int loc = getUniformLocation( varname );
	if ( loc >= 0 )
	{
		glUniform4f( loc, v.x, v.y, v.z, v.w );
	}
}


void kzsGLSL::setUniform( const char* varname, const mat4 & m )
{
	int loc = getUniformLocation( varname );
	if ( loc >= 0 )
	{
		glUniformMatrix4fv( loc, 1, GL_FALSE, &m[ 0 ][ 0 ] );
	}
}


void kzsGLSL::setUniform( const char* varname, const mat3 & m )
{
	int loc = getUniformLocation( varname );
	if ( loc >= 0 )
	{
		glUniformMatrix3fv( loc, 1, GL_FALSE, &m[ 0 ][ 0 ] );
	}
}


void kzsGLSL::setUniform( const char* varname, float val )
{
	int loc = getUniformLocation( varname );
	if ( loc>= 0 )
	{
		glUniform1f( loc, val );
	}
}


void kzsGLSL::setUniform( const char* varname, int val )
{
	int loc = getUniformLocation( varname );
	if ( loc>= 0 )
	{
		glUniform1i( loc, val );
	}
}


void kzsGLSL::setUniform( const char* varname, bool val )
{
	int loc = getUniformLocation( varname );
	if ( loc>= 0 )
	{
		glUniform1i( loc, val );
	}
}


void kzsGLSL::printActiveUniforms()
{
	GLint		nUniforms, size, location, maxLen;
	GLchar*		name;
	GLsizei		written;
	GLenum		type;

	glGetProgramiv( shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen );
	glGetProgramiv( shaderProgram, GL_ACTIVE_UNIFORMS, &nUniforms );

	name = ( GLchar* ) malloc( maxLen );

	printf( " Location | Name\n" );
	printf( "------------------------------------------------\n" );
	for (int i = 0; i < nUniforms; i++)
	{
		glGetActiveUniform( shaderProgram, i, maxLen, &written, &size, &type, name );
		location = glGetUniformLocation( shaderProgram, name );
		printf(" %-8d | %s\n",location, name);
	}
	printf( "\n" );

	free( name );

}


void kzsGLSL::printActiveAttribs()
{
    GLint		written, size, location, maxLength, nAttribs;
    GLenum		type;
    GLchar*		name;

    glGetProgramiv( shaderProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength );
    glGetProgramiv( shaderProgram, GL_ACTIVE_ATTRIBUTES, &nAttribs );

    name = ( GLchar* ) malloc( maxLength );

    printf( " Index | Name\n" );
    printf( "------------------------------------------------\n" );
    for( int i = 0; i < nAttribs; i++ ) {
        glGetActiveAttrib( shaderProgram, i, maxLength, &written, &size, &type, name );
        location = glGetAttribLocation( shaderProgram, name );
        printf( " %-5d | %s\n",location, name );
    }
	printf( "\n" );

    free( name );
}