#pragma once

namespace Base
{
	// call {Ctor} for ptr
	//
	template< typename type >
	inline type* Contruct( void* ptr )
	{
		return new( ptr ) type;
	}

	// call {dtor} for ptr
	//
	template< typename type>
	inline void Destroy( type *const _pPointer )
	{
		// destroy object at _Ptr
		_pPointer->~type();
	}

	// Release COM object
	template< typename Type>
	inline void Release( Type &pObject )
	{
		if( pObject )
		{
			pObject->Release();
			pObject = nullptr;
		}
	}

	// Delete data from pointer
	//
	template< typename Type>
	inline void Delete( Type &pObject )
	{
		if( pObject )
		{
			delete pObject;
			pObject = nullptr;
		}
	}

	// delete array data from pointer
	//
	template< typename Type>
	inline void DeleteArray( Type &pObject )
	{
		if( pObject )
		{
			delete[] pObject;
			pObject = nullptr;
		}
	}

	// clean up data for pointer, force setup null
	//
	template< typename Type >
	inline void Null( Type& pObject )
	{
		if( pObject != nullptr )
		{
			pObject = nullptr;
		}
	}

	// allocate memory WITHOUT zerofying memory
	//
	inline void* Malloc( size_t size )
	{
		if( size == 0 )
		{
			return nullptr;
		}
		return malloc( size );
	}

	// allocate memory for "type" WITHOUT zerofying memory 
	//
	template< typename Type>
	inline Type* Malloc( size_t uiCount )
	{
		if( uiCount == 0 )
		{
			return nullptr;
		}
		return ( Type* )Base::Malloc( sizeof( Type ) * uiCount );
	}

	// allocate memory with init 0 for each element
	//
	inline void* Calloc( size_t size )
	{
		if( size == 0 )
		{
			return nullptr;
		}
		return calloc( 1, size );
	}
	// allocate memory with init 0 for known "type" element
	//
	template< typename Type>
	inline Type* Calloc( size_t uiCount )
	{
		if( uiCount == 0 )
		{
			return nullptr;
		}
		return ( Type* )calloc( uiCount, sizeof( Type ) );
	}


	// free memory without calling dtors
	//
	template< typename Type>
	inline void Free( Type *& pData )
	{
		if( pData == nullptr )
		{
			return;
		}
		free( pData );
		pData = nullptr;
	}


	// getting min from two value
	//
	template< typename Type >
	inline const Type& Min( const Type& a, const Type& b )
	{
		return a < b ? a : b;
	}

	template< typename Type >
	inline const Type MinEqual( const Type& a, const Type& b )
	{
		return a <= b ? a : b;
	}

	// getting max from to value
	//
	template< typename Type >
	inline const Type& Max( const Type& a, const Type& b )
	{
		return a > b ? a : b;
	}

	template< typename Type >
	inline const Type& MaxEqueal( const Type& a, const Type& b )
	{
		return a >= b ? a : b;
	}

	// clamping value to range[min, max]
	//
	template<typename type>
	inline const type& Clamp( const type& value, const type& minimal, const type& maximal )
	{
		//a <= b 
		if( value <= minimal )
		{
			// value is less or equal to min
			return minimal;
		}
		// value is not less, so we need to take min of max values
		return Base::Min( value, maximal );
	}

	template< typename type >
	inline type Lerp( const type& lValue, const type& rValue, float t )
	{
		t = Clamp( t, 0.0f, 1.0f );
		return lValue + ( rValue - lValue )* t;
	}

	//
	//
	constexpr float kRand_Inv = 1.0f / ( float )RAND_MAX;

	//return rand float [0, 1]
	//
	inline float RandF()
	{
		return ( float )( rand() ) * kRand_Inv;
	}

	//return rand from [a,b]
	//
	inline float RandF( float a, float b )
	{
		return a + RandF()*( b - a );
	}
};


