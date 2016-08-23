//=================================================================================
// ByteBuffer.hpp
// Author: Tyler George
// Date  : January 27, 2016
//=================================================================================
#pragma once

#ifndef __included_ByteBuffer__
#define __included_ByteBuffer__

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class ByteBuffer
{
public:
    unsigned char* m_buf;
    size_t m_maxSize;

    size_t m_writeIndex;
    size_t m_readIndex;

    ///---------------------------------------------------------------------------------
    /// Initialization
    ///---------------------------------------------------------------------------------
    void Startup( void* buffer, size_t maxSize );

    ///---------------------------------------------------------------------------------
    /// Accessors
    ///---------------------------------------------------------------------------------
    size_t GetLength() const;
    size_t GetRemainingSize() const { return m_maxSize - m_writeIndex; }
    size_t ReadBytes( void* out_data, size_t size );
    size_t ReadString( char*& str );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void SetLength( size_t len );
    virtual bool WriteBytes( void* data, size_t size );
    virtual bool WriteByte( unsigned char byte );
    virtual bool WriteString( const char* str );



};


#endif