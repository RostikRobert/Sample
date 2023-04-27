#include "pch.h"
#include <OpenGL/GLES2/gl2.h>
#include <OpenGL/EGL/egl.h>
#include <stdarg.h>



#ifdef WIN32
#pragma warning( disable: 4996)
#endif


// esLogMessage()
//    Log an error message to the debug output for the platform
void esLogMessage(const char *formatStr, ...)
{
	va_list params;
	char buf[2048];

	va_start(params, formatStr);
	vsprintf(buf, formatStr, params);

	printf("%s", buf);
	fprintf(stderr, "%s", buf);

	va_end(params);
}

/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param shaderSrc Shader source string
/// \return A new shader object on success, 0 on failure
//
GLuint esLoadShader ( GLenum type, char * filename )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == 0 )
		return 0;

	// Load the shader source
	FILE * pf = fopen(filename, "rb" );
	if (pf == NULL)
		return NULL;
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * shaderSrc = new char[size + 1];
	fread(shaderSrc, sizeof(char), size, pf);
	shaderSrc[size] = 0;
	fclose(pf);

	glShaderSource ( shader, 1, (const char **)&shaderSrc, NULL );
	delete [] shaderSrc;

	// Compile the shader
	glCompileShader ( shader );

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled ) 
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char  [infoLen];


			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
			esLogMessage ( "Error compiling shader <%s>:\n%s\n", filename, infoLog );            

			delete [] infoLog;
		}

		glDeleteShader ( shader );
		return 0;
	}

	return shader;
}

/// \brief Create a program object, link program.
//         Errors output to log.
/// \param vertShaderSrc Vertex shader source code
/// \param fragShaderSrc Fragment shader source code
/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure

GLuint esLoadProgram ( GLuint vertexShader, GLuint fragmentShader )
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram ( );

	if ( programObject == 0 )
		return 0;

	glAttachShader ( programObject, vertexShader );
	glAttachShader ( programObject, fragmentShader );

	// Link the program
	glLinkProgram ( programObject );

	// Check the link status
	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked ) 
	{
		GLint infoLen = 0;

		glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char[sizeof(char) * infoLen];


			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
			esLogMessage ( "Error linking program:\n%s\n", infoLog );            

			delete[] infoLog;
		}

		glDeleteProgram ( programObject );
		return 0;
	}

	return programObject;
}