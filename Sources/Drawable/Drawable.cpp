#include "pch.h"
#include "Drawable.h"
#include "Sprite/Sprite.h"
#include "esUtils/esUtil.h"


#include "Common/Vertex.h"
#include "Common/Globals.h"
#include "TGA/TGA.h"

Drawable::Drawable()
	: _uiVBO( 0 )
	, _uiProgram( 0 )
	, _uiVertexShader( 0 )
	, _uiPixelShader( 0 )
	, _uiPosLAttribute( -1 )
	, _uiUVAttribute( -1 )
	, _uiColorUni( -1 )
	, _uiTextureSampler( -1 )
	, _uiTM( -1 )
{
}

Drawable::~Drawable()
{

}

void Drawable::Init()
{
	// making ortho projection 
	this->projOrtho.SetOrtho( 0.0f, ( float )Globals::screenWidth, ( float )Globals::screenHeight, 0.0f, 0.0f, 100.0f );

	//this->projOrtho.SetPerspective( 45.0f * 3.14f / 180.0f, ( float )Globals::screenWidth / ( float )Globals::screenHeight, 0.1f, 100.0f );

	// by default, cem looks at Z direction,  so , use only traslation
	//this->matView.SetTranslation( Vector3( 0.0f, 0.0f, -12.0f ) );

	// init VBO
	{
		//triangle data (heap)
		Vertex2D verticesData[ 6 ];

		// our particle will be 32x32
		Vector2 size;
		size.x = 1.0f / 2.0f;
		size.y = 1.0f / 2.0f;

		verticesData[ 0 ].pos.x = -size.x;  verticesData[ 0 ].pos.y = -size.y; verticesData[ 0 ].pos.z = 1.0f;
		verticesData[ 1 ].pos.x = -size.x;  verticesData[ 1 ].pos.y = +size.y; verticesData[ 1 ].pos.z = 1.0f;
		verticesData[ 2 ].pos.x = +size.x;  verticesData[ 2 ].pos.y = +size.y; verticesData[ 2 ].pos.z = 1.0f;


		verticesData[ 3 ].pos.x = -size.x;  verticesData[ 3 ].pos.y = -size.y; verticesData[ 3 ].pos.z = 1.0f;
		verticesData[ 4 ].pos.x = +size.x;  verticesData[ 4 ].pos.y = +size.y; verticesData[ 4 ].pos.z = 1.0f;
		verticesData[ 5 ].pos.x = +size.x;  verticesData[ 5 ].pos.y = -size.y; verticesData[ 5 ].pos.z = 1.0f;

		verticesData[ 0 ].uv.x = 0.0f;  verticesData[ 0 ].uv.y = 0.0f;
		verticesData[ 1 ].uv.x = 0.0f;  verticesData[ 1 ].uv.y = 1.0f;
		verticesData[ 2 ].uv.x = 1.0f;  verticesData[ 2 ].uv.y = 1.0f;

		verticesData[ 3 ].uv.x = 0.0f;  verticesData[ 3 ].uv.y = 0.0f;
		verticesData[ 4 ].uv.x = 1.0f;  verticesData[ 4 ].uv.y = 1.0f;
		verticesData[ 5 ].uv.x = 1.0f;  verticesData[ 5 ].uv.y = 0.0f;


		//vert[ 0 ].position = Vector3( -1.0f,  1.0f, 0.0f );
		//vert[ 1 ].position = Vector3( -1.0f, -1.0f, 0.0f );
		//vert[ 2 ].position = Vector3(  1.0f,  1.0f, 0.0f );

		//vert[ 3 ].position = Vector3(  1.0f, -1.0f, 0.0f );

		//unsigned int Indexes[ 6 ] =
		//{
		//	0, 1, 2,
		//	1, 3, 2
		//};

		//buffer object
		glGenBuffers( 1, &this->_uiVBO );
		glBindBuffer( GL_ARRAY_BUFFER, this->_uiVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex2D ) * 6, verticesData, GL_STATIC_DRAW );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );

	}

	// shader
	{
		this->_uiVertexShader = esLoadShader( GL_VERTEX_SHADER, "../Resources/Shaders/Particle.vs" );
		this->_uiPixelShader = esLoadShader( GL_FRAGMENT_SHADER, "../Resources/Shaders/Particle.fs" );

		this->_uiProgram = esLoadProgram( this->_uiVertexShader, this->_uiPixelShader );


		//finding location of uniforms / attributes
		this->_uiPosLAttribute = glGetAttribLocation( this->_uiProgram, "a_posL" );
		this->_uiUVAttribute = glGetAttribLocation( this->_uiProgram, "a_uv" );


		this->_uiColorUni = glGetUniformLocation( this->_uiProgram, "u_PartColor" );
		this->_uiTextureSampler = glGetUniformLocation( this->_uiProgram, "u_texture" );
		//this->_uiPos= glGetUniformLocation( this->_uiProgram, "posNDC" );
		this->_uiTM = glGetUniformLocation( this->_uiProgram, "TM" );

	}

	// textures
	{
		auto createTex = []( std::string sPath ) -> GLuint
		{
			GLuint textureHandle;
			glGenTextures( 1, &textureHandle );
			glBindTexture( GL_TEXTURE_2D, textureHandle );

			int width, height, bpp;
			char *bufferTGA = LoadTGA( sPath.c_str(), &width, &height, &bpp );

			if( bpp == 24 )
			{
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bufferTGA );
			}
			else
			{
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferTGA );
			}

			delete[] bufferTGA;

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			glGenerateMipmap( GL_TEXTURE_2D );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

			glBindTexture( GL_TEXTURE_2D, 0 );
			return  textureHandle;
		};

		const std::vector< std::string >& vecTex = { 
			"fire.tga",
			"noise.tga",
			"smoke.tga",
			"star.tga"};
		for( const std::string& sName : vecTex )
		{
			std::string sFullPath = std::string( "../Resources/Textures/" ) + sName;
			GLuint tex = createTex( sFullPath );
			Globals::Texures[ sName ] = tex;
		}

	}
}

void Drawable::PreDraw()const
{
	glUseProgram( this->_uiProgram );
	glBindBuffer( GL_ARRAY_BUFFER, this->_uiVBO );

	unsigned int uiOffset = 0;
	if( this->_uiPosLAttribute != -1 )
	{
		glEnableVertexAttribArray( this->_uiPosLAttribute );
		glVertexAttribPointer( this->_uiPosLAttribute, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex2D ), (void*)uiOffset );
		uiOffset += 12;  // Vector3 offset for position
	}
	if( this->_uiUVAttribute != -1 )
	{
		glEnableVertexAttribArray( this->_uiUVAttribute );
		glVertexAttribPointer( this->_uiUVAttribute, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex2D ), (void*)uiOffset );

	}
}

void Drawable::Draw( Sprite part )const
{
	if( this->_uiColorUni != -1 )
	{
		const Vector4& color = part.getTint();
		glUniform4fv( this->_uiColorUni, 1, &color.x );
	}

	if( this->_uiTM != -1 )
	{
		//Matrix viewProj = this->matView * this->projOrtho;

		Matrix viewProj = this->projOrtho;

		Matrix res = part.getObjectTM();
		res = res * viewProj;
		glUniformMatrix4fv( this->_uiTM, 1, GL_FALSE, &res.m[ 0 ][ 0 ] );
	}

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, part.getTexture() ); // start.tga
	glUniform1i( this->_uiTextureSampler, 0 );

	// draw particle
	glDrawArrays( GL_TRIANGLES, 0, 6 );
}

void Drawable::PostDraw()const
{
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void Drawable::Clean()
{
	glDeleteBuffers( 1, &this->_uiVBO );
	glDeleteProgram( this->_uiProgram );
	glDeleteShader( this->_uiVertexShader );
	glDeleteShader( this->_uiPixelShader );

	// texts
	for( decltype( Globals::Texures )::const_iterator itStart = Globals::Texures.begin(), itEnd = Globals::Texures.end();
		itStart != itEnd; ++itStart )
	{
		glDeleteBuffers( 1, &itStart->second );
	}
}

Matrix Drawable::getView()
{
	return this->matView;
}
