//=================================================================================
// BinaryBufferBuilder.cpp
// Author: Tyler George
// Date  : August 25, 2015
//=================================================================================


////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================

#include "Engine/Utilities/BinaryBufferBuilder.hpp"


////===========================================================================================
///===========================================================================================
// Static Variable Initialization
///===========================================================================================
////===========================================================================================



////===========================================================================================
///===========================================================================================
// Constructors/Destructors
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
BinaryBufferBuilder::BinaryBufferBuilder( size_t estimatedSize )
{
    m_buffer.resize( 0 );
    m_buffer.reserve( estimatedSize );
    m_bufferPosition = 0;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
BinaryBufferBuilder::~BinaryBufferBuilder()
{

}

////===========================================================================================
///===========================================================================================
// Initialization
///===========================================================================================
////===========================================================================================


////===========================================================================================
///===========================================================================================
// Accessors/Queries
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferBuilder::WriteBufferToFile( const std::string& filePath ) const
{
    return WriteByteVectorToFile( filePath, m_buffer );
}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendChar( const char& c )
{
    if (m_bufferPosition + sizeof( char ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( char ) );

    *((char*)(m_buffer.data() + m_bufferPosition)) = c;
    
    m_bufferPosition += sizeof( char );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendShort( const short& s )
{
    if (m_bufferPosition + sizeof( short ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( short ) );

    *((short*)(m_buffer.data() + m_bufferPosition)) = s;

    m_bufferPosition += sizeof( short );

}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendInt( const int& i )
{
    if (m_bufferPosition + sizeof( int ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( int ) );

    *((int*)(m_buffer.data() + m_bufferPosition)) = i;

    m_bufferPosition += sizeof( int );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendUnsignedChar( const unsigned char& uc )
{
    if (m_bufferPosition + sizeof( unsigned char ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( unsigned char ) );

    *((unsigned char*)(m_buffer.data() + m_bufferPosition)) = uc;

    m_bufferPosition += sizeof( unsigned char );

}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendUnsignedShort( const unsigned short& us )
{
    if (m_bufferPosition + sizeof( unsigned short ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( unsigned short ) );

    *((unsigned short*)(m_buffer.data() + m_bufferPosition)) = us;

    m_bufferPosition += sizeof( unsigned short );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendUnsignedInt( const unsigned int& ui )
{
    if (m_bufferPosition + sizeof( unsigned int ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( unsigned int ) );

    *((unsigned int*)(m_buffer.data() + m_bufferPosition)) = ui;

    m_bufferPosition += sizeof( unsigned int );

}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendFloat( const float& f )
{

    if (m_bufferPosition + sizeof( float ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( float ) );

    *((float*)(m_buffer.data() + m_bufferPosition)) = f;

    m_bufferPosition += sizeof( float );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendDouble( const double& d )
{
    if (m_bufferPosition + sizeof( double ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( double ) );

    *((double*)(m_buffer.data() + m_bufferPosition)) = d;

    m_bufferPosition += sizeof( double );

}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendBool( const bool& b )
{
    if (m_bufferPosition + sizeof( bool ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( bool ) );

    *((bool*)(m_buffer.data() + m_bufferPosition)) = b;

    m_bufferPosition += sizeof( bool );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendString( const std::string& s )
{
    if (m_bufferPosition + s.size() + 1 > m_buffer.size())
        m_buffer.resize( m_bufferPosition + s.size() + 1);

    for (std::string::const_iterator strIter = s.begin(); strIter != s.end(); ++strIter)
    {
        *((char*)(m_buffer.data() + m_bufferPosition)) = *strIter;
        m_bufferPosition += sizeof( char );
    }
    *((char*)(m_buffer.data() + m_bufferPosition)) = '\0';

    m_bufferPosition += sizeof( char );
}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendVector2( const Vector2& v2 )
{
    if (m_bufferPosition + sizeof( Vector2 ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( Vector2 ) );

    *((Vector2*)(m_buffer.data() + m_bufferPosition)) = v2;

    m_bufferPosition += sizeof( Vector2 );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendVector3( const Vector3& v3 )
{
    if (m_bufferPosition + sizeof( Vector3 ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( Vector3 ) );

    *((Vector3*)(m_buffer.data() + m_bufferPosition)) = v3;

    m_bufferPosition += sizeof( Vector3 );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendVector4f( const Vector4f& v4f )
{
    if (m_bufferPosition + sizeof( Vector4f ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( Vector4f ) );

    *((Vector4f*)(m_buffer.data() + m_bufferPosition)) = v4f;

    m_bufferPosition += sizeof( Vector4f );

}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendVector4d( const Vector4d& v4d )
{
    if (m_bufferPosition + sizeof( Vector4d ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( Vector4d ) );

    *((Vector4d*)(m_buffer.data() + m_bufferPosition)) = v4d;

    m_bufferPosition += sizeof( Vector4d );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendMatrix4f( const Matrix4f& m4f )
{
    if (m_bufferPosition + sizeof( Matrix4f ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( Matrix4f ) );

    *((Matrix4f*)(m_buffer.data() + m_bufferPosition)) = m4f;

    m_bufferPosition += sizeof( Matrix4f );

}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendMatrix4d( const Matrix4d& m4d )
{
    if (m_bufferPosition + sizeof( Matrix4d ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( Matrix4d ) );

    *((Matrix4d*)(m_buffer.data() + m_bufferPosition)) = m4d;

    m_bufferPosition += sizeof( Matrix4d );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BinaryBufferBuilder::AppendRgba( const Rgba& rgba )
{
    if (m_bufferPosition + sizeof( Rgba ) > m_buffer.size())
        m_buffer.resize( m_bufferPosition + sizeof( Rgba ) );

    *((Rgba*)(m_buffer.data() + m_bufferPosition)) = rgba;

    m_bufferPosition += sizeof( Rgba );
}

////===========================================================================================
///===========================================================================================
// Update
///===========================================================================================
////===========================================================================================


////===========================================================================================
///===========================================================================================
// Render
///===========================================================================================
////===========================================================================================


////===========================================================================================
///===========================================================================================
// Private Functions
///===========================================================================================
////===========================================================================================

