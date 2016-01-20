//=================================================================================
// BinaryBufferParser.hpp
// Author: Tyler George
// Date  : August 23, 2015
//=================================================================================

#pragma once

#ifndef __included_BinaryBufferParser__
#define __included_BinaryBufferParser__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Utilities/FileUtilities.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Matrix4.hpp"

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------


////===========================================================================================
///===========================================================================================
// BinaryBufferParser Class
///===========================================================================================
////===========================================================================================
class BinaryBufferParser
{
public:
	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
    BinaryBufferParser( const unsigned char* buffer, const size_t& bufferSize );
    BinaryBufferParser( const ByteVector& byteVector );
    BinaryBufferParser();
    ~BinaryBufferParser();

	///---------------------------------------------------------------------------------
	/// Initialization
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
    bool HasReachedEndOfData() { return m_hasReachedEndOfData; }

    char ReadChar();
    short ReadShort();
    int ReadInt();
    unsigned char ReadUnsignedChar();
    unsigned short ReadUnsignedShort();
    unsigned int ReadUnsignedInt();
    float ReadFloat();
    double ReadDouble();
    bool ReadBool();
    std::string ReadString();
    Vector2 ReadVector2();
    Vector3 ReadVector3();
    Vector4f ReadVector4f();
    Vector4d ReadVector4d();
    Matrix4f ReadMatrix4f();
    Matrix4d ReadMatrix4d();
    Rgba ReadRgba();

    bool ReadChar( char& c );
    bool ReadShort( short& s );
    bool ReadInt( int& i );
    bool ReadUnsignedChar( unsigned char& uc );
    bool ReadUnsignedShort( unsigned short& us );
    bool ReadUnsignedInt( unsigned int& ui );
    bool ReadFloat( float& f );
    bool ReadDouble( double& d );
    bool ReadBool( bool& b );
    bool ReadString( std::string& str );
    bool ReadVector2( Vector2& v2 );
    bool ReadVector3( Vector3& v3 );
    bool ReadVector4f( Vector4f& v4f );
    bool ReadVector4d( Vector4d& v4d );
    bool ReadMatrix4f( Matrix4f& m4f );
    bool ReadMatrix4d( Matrix4d& m4d );
    bool ReadRgba( Rgba& rgba );
         

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Update
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Render
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Public Member Variables
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Static Variables
	///---------------------------------------------------------------------------------


private:
	///---------------------------------------------------------------------------------
	/// Private Functions
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Private Member Variables
	///---------------------------------------------------------------------------------

protected:
    ///---------------------------------------------------------------------------------
    /// Protected Member Variables
    ///---------------------------------------------------------------------------------
    const unsigned char* m_buffer;
    size_t m_bufferSize;
    size_t m_bufferPosition;
    bool m_hasReachedEndOfData;
};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

#endif