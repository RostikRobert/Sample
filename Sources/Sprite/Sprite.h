#pragma once

#include "Math/esMath.h"

class Sprite
{
public:
	// default {ctor} and {dtor}
	//
	Sprite();
	//virtual ~Sprite() = default;
	~Sprite() {};

	// setter / getter parent transform
	//
	void setParentTM( Matrix matTM );
	Matrix getParentTM();

	// setter / getter rotation
	//
	void setRotation( float fAngle );
	float getRotation();

	// setter / getter position
	//
	void setPosition( Vector2 pos );
	Vector2 getPosition();

	// setter / getter scale
	//
	void setScale( Vector2 scale );
	Vector2 getScale();

	// setter / getter active texture
	//
	void setTexture( int texID );
	int getTexture();

	// setter/ getter tint color
	//
	void setTint( Vector4 color );
	Vector4 getTint();

	// setter/ getter depth
	//
	void setDepth( float value );
	float getDepth();

	// getter object TM
	//
	Matrix getObjectTM();

	// updating object transform
	//
	void MakeObjectTM( );

protected:
	// base TM of create time
	//
	Matrix _parentTM;

	// Postion
	//
	Vector2 _Position;

	// color value
	//
	Vector4 _ColorTint;

	// rotation value( due to 2D - only one axis )
	//
	float _fRotation;

	// scale value
	//
	Vector2 _Scale;

	// index texture in the system
	// should not be assigned manually
	//
	int _TextureIndex;

	// result matrix after all transformations
	//
	Matrix _TM;

	// 
	//
	float _fDepth;
};