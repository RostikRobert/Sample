#include "pch.h"
#include "Sprite.h"

Sprite::Sprite()
	: _Position( 0.0f, 0.0f )
	, _ColorTint( 1.0f, 1.0f, 1.0f, 1.0f)// Color setup by default
	, _fRotation( 1.0f )
	, _Scale( 1.0f, 1.0f )
	, _TextureIndex( -1 )
	, _fDepth( 0.0f )
{
	this->_TM.SetIdentity();
	this->_parentTM.SetIdentity();
}

void Sprite::setParentTM( Matrix matTM )
{
	this->_parentTM = matTM;
}

Matrix Sprite::getParentTM()
{
	return this->_parentTM;
}

void Sprite::setRotation( float fAngle )
{
	this->_fRotation = fAngle;
}

float Sprite::getRotation()
{
	return this->_fRotation;
}

void Sprite::setPosition( Vector2 pos )
{
	this->_Position = pos;
}

Vector2 Sprite::getPosition()
{
	return this->_Position;
}

void Sprite::setScale( Vector2 scale )
{
	this->_Scale = scale;
}

Vector2 Sprite::getScale()
{
	return this->_Scale;
}

void Sprite::setTexture( int texID )
{
	this->_TextureIndex = texID;
}

int Sprite::getTexture()
{
	return this->_TextureIndex;
}

void Sprite::setTint( Vector4 color )
{
	this->_ColorTint = color;
}

Vector4 Sprite::getTint()
{
	return this->_ColorTint;
}

void Sprite::setDepth( float value )
{
	this->_fDepth = value;
}

float Sprite::getDepth()
{
	return this->_fDepth;
}

Matrix Sprite::getObjectTM()
{
	return this->_TM;
}

void Sprite::MakeObjectTM()
{
	// calculate result of TM
	Matrix scale;

	// using -scale.y -> quick hack for get rid of extra math
	scale.SetScale( this->_Scale.x, -this->_Scale.y, 1.0f );

	Matrix rot;
	rot.SetRotationZ( this->_fRotation * 3.14f / 180.0f );


	Matrix pos;
	pos.SetTranslation( this->_Position.x, this->_Position.y, this->_fDepth );

	this->_TM = ( scale * rot * pos ) * this->_parentTM;
}


