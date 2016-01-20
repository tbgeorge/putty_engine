//=================================================================================
// BinaryBufferParser.cpp
// Author: Tyler George
// Date  : August 23, 2015
//=================================================================================


////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================

#include "Engine/Utilities/BinaryBufferParser.hpp"


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
BinaryBufferParser::BinaryBufferParser( const unsigned char* buffer, const size_t& bufferSize )
    : m_buffer( buffer )
    , m_bufferSize( bufferSize )
    , m_bufferPosition( 0 )
    , m_hasReachedEndOfData( false )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
BinaryBufferParser::BinaryBufferParser( const ByteVector& byteVector )
    : m_buffer(byteVector.data() )
    , m_bufferSize(byteVector.size() )
    , m_bufferPosition( 0 )
    , m_hasReachedEndOfData( false )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
BinaryBufferParser::BinaryBufferParser()
    : m_buffer( nullptr )
    , m_bufferSize( 0 )
    , m_bufferPosition( 0 )
    , m_hasReachedEndOfData( false )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
BinaryBufferParser::~BinaryBufferParser()
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
char BinaryBufferParser::ReadChar()
{
    size_t advance = sizeof( char );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    char returnVal = *((char*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
short BinaryBufferParser::ReadShort()
{
    size_t advance = sizeof( short );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();
   
    short returnVal = *((short*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
int BinaryBufferParser::ReadInt()
{
    size_t advance = sizeof( int );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    int returnVal = *((int*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned char BinaryBufferParser::ReadUnsignedChar()
{
    size_t advance = sizeof( unsigned char );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    unsigned char returnVal = m_buffer[m_bufferPosition];

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned short BinaryBufferParser::ReadUnsignedShort()
{
    size_t advance = sizeof( unsigned short );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    unsigned short returnVal = *((unsigned short*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int BinaryBufferParser::ReadUnsignedInt()
{
    size_t advance = sizeof( unsigned int );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    unsigned int returnVal = *((unsigned int*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float BinaryBufferParser::ReadFloat()
{
    size_t advance = sizeof( float );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    float returnVal = *((float*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
double BinaryBufferParser::ReadDouble()
{
    size_t advance = sizeof( double );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    double returnVal = *((double*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;


}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadBool()
{
    size_t advance = sizeof( bool );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    bool returnVal = *((bool*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::string BinaryBufferParser::ReadString()
{
    std::string returnVal = "";
    char nextCharacter = *((char*)(m_buffer + m_bufferPosition));
    m_bufferPosition += sizeof( char );
    if (m_bufferPosition == m_bufferSize)
        __debugbreak();

    while (nextCharacter != '\0')
    {
        returnVal += nextCharacter;

        nextCharacter = *((char*)(m_buffer + m_bufferPosition));
        m_bufferPosition += sizeof( char );
        if (m_bufferPosition == m_bufferSize)
            __debugbreak();
    }

    return returnVal;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Vector2 BinaryBufferParser::ReadVector2()
{
    size_t advance = sizeof( Vector2 );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    Vector2 returnVal = *((Vector2*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Vector3 BinaryBufferParser::ReadVector3()
{
    size_t advance = sizeof( Vector3 );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    Vector3 returnVal = *((Vector3*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Vector4f BinaryBufferParser::ReadVector4f()
{
    size_t advance = sizeof( Vector4f );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    Vector4f returnVal = *((Vector4f*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Vector4d BinaryBufferParser::ReadVector4d()
{
    size_t advance = sizeof( Vector4d );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    Vector4d returnVal = *((Vector4d*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Matrix4f BinaryBufferParser::ReadMatrix4f()
{
    size_t advance = sizeof( Matrix4f );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    Matrix4f returnVal = *((Matrix4f*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Matrix4d BinaryBufferParser::ReadMatrix4d()
{
    size_t advance = sizeof( Matrix4d );
    if (m_bufferPosition + advance > m_bufferSize)
        __debugbreak();

    Matrix4d returnVal = *((Matrix4d*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;
    return returnVal;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Rgba BinaryBufferParser::ReadRgba()
{
    unsigned char r = ReadUnsignedChar();
    unsigned char g = ReadUnsignedChar();
    unsigned char b = ReadUnsignedChar();
    unsigned char a = ReadUnsignedChar();

    Rgba returnVal( r, g, b, a );

    return returnVal;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadChar( char& c )
{
    size_t advance = sizeof( char );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    c = *((char*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadShort( short& s )
{
    size_t advance = sizeof( short );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    s = *((short*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadInt( int& i )
{
    size_t advance = sizeof( int );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    i = *((int*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadUnsignedChar( unsigned char& uc )
{
    size_t advance = sizeof( unsigned char );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    uc = m_buffer[m_bufferPosition];

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadUnsignedShort( unsigned short& us )
{
    size_t advance = sizeof( unsigned short );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    us = *((unsigned short*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadUnsignedInt( unsigned int& ui )
{
    size_t advance = sizeof( unsigned int );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    ui = *((unsigned int*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadFloat( float& f )
{
    size_t advance = sizeof( float );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    f = *((float*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadDouble( double& d )
{
    size_t advance = sizeof( double );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    d = *((double*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadBool( bool& b )
{
    size_t advance = sizeof( bool );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    b = *((bool*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadString( std::string& str )
{
    str = "";
    char nextCharacter = *((char*)(m_buffer + m_bufferPosition));
    m_bufferPosition += sizeof( char );

    while (nextCharacter != '\0')
    {
        str += nextCharacter;

        nextCharacter = *((char*)(m_buffer + m_bufferPosition));
        m_bufferPosition += sizeof( char );

        if (m_bufferPosition == m_bufferSize)
            return false;
    }

    return true;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadVector2( Vector2& v2 )
{
    size_t advance = sizeof( Vector2 );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    v2 = *((Vector2*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadVector3( Vector3& v3 )
{
    size_t advance = sizeof( Vector3 );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    v3 = *((Vector3*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadVector4f( Vector4f& v4f )
{
    size_t advance = sizeof( Vector4f );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    v4f = *((Vector4f*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadVector4d( Vector4d& v4d )
{
    size_t advance = sizeof( Vector4d );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    v4d = *((Vector4d*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadMatrix4f( Matrix4f& m4f )
{
    size_t advance = sizeof( Matrix4f );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    m4f = *((Matrix4f*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadMatrix4d( Matrix4d& m4d )
{
    size_t advance = sizeof( Matrix4d );
    if (m_bufferPosition + advance > m_bufferSize)
        return false;

    m4d = *((Matrix4d*)(m_buffer + m_bufferPosition));

    m_bufferPosition += advance;
    if (m_bufferPosition == m_bufferSize)
        m_hasReachedEndOfData = true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool BinaryBufferParser::ReadRgba( Rgba& rgba )
{
    bool rSuccess = ReadUnsignedChar( rgba.a );
    if (!rSuccess)
        return rSuccess;
    bool gSuccess = ReadUnsignedChar( rgba.g );
    if (!gSuccess)
        return gSuccess;
    bool bSuccess = ReadUnsignedChar( rgba.b );
    if (!bSuccess)
        return bSuccess;
    bool aSuccess = ReadUnsignedChar( rgba.a );
    if (!aSuccess)
        return aSuccess;

    return true;
}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================


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

