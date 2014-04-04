#ifndef VARIANT_H
#define VARIANT_H

#include "HashString.h"
#include "defines.h"
#include <iostream>

namespace rho
{

/// Variant of varriable types
/// now supports: int, float, bool, hashed string id ( int )
struct Variant
{
	enum VARIANT_TYPE
	{
		TYPE_NONE = 0,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_BOOL,
		TYPE_HASHEDSTRING_ID,
		TYPE_VOID_PTR
	};

	/// \brief What type of data is this variant holding?
	VARIANT_TYPE type;

	union
	{
		int 		m_asInteger;
		float 		m_asFloat;
		bool 		m_asBool;
		StringID 	m_asHashedStringID;
		void * 		m_asVoidPtr;
	};

	// Constructors
	Variant() 							: type(TYPE_NONE) {}
	Variant( int const & val ) 			: type( TYPE_INT ), m_asInteger( val ) {}
	Variant( float const & val ) 		: type( TYPE_FLOAT ), m_asFloat( val ) {}
	Variant( bool const & val ) 		: type( TYPE_BOOL ), m_asBool( val ) {}
	Variant( unsigned int const & val ) : type( TYPE_HASHEDSTRING_ID ), m_asHashedStringID( val ) {}
    Variant( void * val ) 				: type( TYPE_VOID_PTR ), m_asVoidPtr( val ) {}

	inline bool isInt() const 			{ return type == TYPE_INT; }
	inline bool isFloat() const 		{ return type == TYPE_FLOAT; }
	inline bool isBool() const 			{ return type == TYPE_BOOL; }
	inline bool isHashStringID() const 	{ return type == TYPE_HASHEDSTRING_ID; }
	inline bool isVoidPtr() const 		{ return type == TYPE_VOID_PTR; }

	inline int getAsInt() const
	{
		assert_msg ( type == TYPE_INT, "Variant type conversion from non-int to int" );

		return m_asInteger;
	}

	inline float getAsFloat() const
	{
		assert_msg ( type == TYPE_FLOAT, "Variant type conversion from non-float to float" );

		return m_asFloat;
	}

	inline bool getAsBool() const
	{
		assert_msg ( type == TYPE_BOOL, "Variant type conversion from non-bool to bool" );

		return m_asBool;
	}

	inline StringID getAsStringID() const
	{
		assert_msg ( type == TYPE_HASHEDSTRING_ID, "Variant type conversion from non-string to string_id" );

		return m_asHashedStringID;
	}

	inline HashString getAsHashedString() const
	{
		assert_msg ( type == TYPE_HASHEDSTRING_ID, "Variant type conversion from non-string to HashString" );

		return HashString( m_asHashedStringID );
	}

	inline std::string getAsString() const
	{
		assert_msg ( type == TYPE_HASHEDSTRING_ID, "Variant type conversion from non-string to string" );

		return HashString( m_asHashedStringID ).getString();
	}

    inline void * getAsVoidPtr() const
    {
        assert_msg ( type == TYPE_VOID_PTR, "Variant type conversion from non-void ptr to void ptr" );

		return m_asVoidPtr;
    }
};

};

#endif
