//=================================================================================
// FileUtilities.cpp
// Author: Tyler George
// Date  : April 9, 2015
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================

#include <vector>
#include "Engine/Utilities/FileUtilities.hpp"

#ifndef WIND32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <io.h>
#include "Console.hpp"
#include "Error.hpp"


////===========================================================================================
///===========================================================================================
// Load File Job class
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
LoadFileJob::LoadFileJob( const std::string filePath )
    : m_filePath( filePath )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
LoadFileJob::~LoadFileJob()
{
}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void LoadFileJob::Run()
{
    size_t bufferSize = 0;
    m_buffer = LoadBinaryFileToNewBuffer( m_filePath, bufferSize );

    LoadFileData* returnData = new LoadFileData();
    returnData->buffer = m_buffer;
    returnData->bufferSize = bufferSize;

    SetCallbackArg( (void*)returnData );
}


////===========================================================================================
///===========================================================================================
// Save File Job class
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
SaveFileJob::SaveFileJob( const std::string filePath, unsigned char* buffer, size_t bufferSize )
    : m_filePath( filePath )
    , m_buffer( buffer )
    , m_bufferSize( bufferSize )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
SaveFileJob::~SaveFileJob()
{
}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SaveFileJob::Run()
{
    
    bool* success = new bool( WriteBufferToFile( m_filePath, m_buffer, m_bufferSize ) );

    SetCallbackArg( (void*)success );
}

////===========================================================================================
///===========================================================================================
// Hash Buffer Job class
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
HashBufferJob::HashBufferJob( unsigned char* buffer )
    : m_buffer( buffer )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
HashBufferJob::~HashBufferJob()
{
}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void HashBufferJob::Run()
{
    unsigned long* hashValue = new unsigned long( HashBuffer_DJB2( m_buffer ) );

    SetCallbackArg( (void*)hashValue );
}


////===========================================================================================
///===========================================================================================
// Reverse Buffer Job class
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
ReverseBufferJob::ReverseBufferJob( unsigned char* buffer, size_t bufferSize )
    : m_buffer( buffer )
    , m_bufferSize( bufferSize )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
ReverseBufferJob::~ReverseBufferJob()
{
}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ReverseBufferJob::Run()
{
    ReverseBuffer( m_buffer, m_bufferSize );

    SetCallbackArg( (void*)m_buffer );
}

////===========================================================================================
///===========================================================================================
// Generate Files Job class
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
GenerateFilesJob::GenerateFilesJob( int numFiles, int fileSize )
    : m_numFiles( numFiles )
    , m_fileSize( fileSize )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
GenerateFilesJob::~GenerateFilesJob()
{
}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void GenerateFilesJob::Run()
{
    GenerateFiles( m_numFiles, m_fileSize );
}

////===========================================================================================
///===========================================================================================
// Helper Functions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
size_t GetFileLength( FILE* fileHandle )
{
    fseek( fileHandle, 0, SEEK_END );
    size_t fileSize = ftell( fileHandle );
    rewind( fileHandle );
    return fileSize;
}


////===========================================================================================
///===========================================================================================
// Writing
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool WriteBufferToFile( const std::string& filePath, const unsigned char* buffer, const size_t& bufferSize )
{
	FILE* file;
	fopen_s( &file, filePath.c_str(), "wb" );

	if( !file )
	{
		// FREAK OUT
		return false;
	}


	fwrite( buffer, sizeof( unsigned char ), bufferSize, file );
	fclose( file );
	return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool WriteByteVectorToFile( const std::string& filePath, const ByteVector& byteVector )
{
    FILE* file;
    fopen_s( &file, filePath.c_str(), "wb" );

    if (!file)
    {
        //freak out
        return false;
    }

    fwrite( byteVector.data(), sizeof( unsigned char ), byteVector.size(), file );
    fclose( file );

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool WriteTextBufferToFile( const std::string& filePath, const char* buffer, const size_t& bufferSize )
{
    FILE* file;
    fopen_s( &file, filePath.c_str(), "wb" );

    if (!file)
    {
        //freak out
        return false;
    }

    fwrite( buffer, sizeof( char ), bufferSize - 1, file );
    fclose( file );

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool WriteStringToFile( const std::string& filePath, const std::string& stringBuffer )
{
    FILE* file;
    fopen_s( &file, filePath.c_str(), "wb" );

    if (!file)
    {
        //freak out
        return false;
    }

    fwrite( stringBuffer.data(), sizeof( char ), stringBuffer.size(), file );
    fclose( file );

    return true;
}

////===========================================================================================
///===========================================================================================
// Loading
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool LoadBinaryFileToExistingBuffer( const std::string& filePath, unsigned char* existingBuffer, const size_t& existingBufferSize )
{
	FILE* file;
	fopen_s( &file, filePath.c_str(), "rb" );

	if( !file )
	{
		// freak out
		return false;
	}

    size_t numBytes = GetFileLength( file );
    if (numBytes > existingBufferSize)
        return false;

    fread( existingBuffer, sizeof( unsigned char ), existingBufferSize, file );
    fclose( file );

	return true;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool LoadBinaryFileToExistingByteVector( const std::string& filePath, ByteVector& existingVectorBuffer )
{
    FILE* file;
    fopen_s( &file, filePath.c_str(), "rb" );

    if (!file)
    {
        // freak out
        return false;
    }


    size_t neededBufferSize = GetFileLength( file );

    // Grow/Shrink
    existingVectorBuffer.resize( neededBufferSize );

    fread( existingVectorBuffer.data(), sizeof( unsigned char ), neededBufferSize, file );
    fclose( file );

    return true;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned char* LoadBinaryFileToNewBuffer( const std::string& filePath, size_t& out_bufferSize )
{
	FILE* file;
	fopen_s( &file, filePath.c_str(), "rb" );

	if( !file )
	{
		// freak out
		return NULL;
	}

    out_bufferSize = GetFileLength( file );

	unsigned char* newBuffer = new unsigned char[ out_bufferSize ];
	fread( newBuffer, sizeof( unsigned char ), out_bufferSize, file );
	fclose( file );

	return newBuffer;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool LoadTextFileToExistingBuffer( const std::string& filePath, char* existingBuffer, const size_t& existingBufferSize )
{
    FILE* file;
    fopen_s( &file, filePath.c_str(), "rb" );

    if (!file)
    {
        // freak out
        return false;
    }

    size_t numBytes = GetFileLength( file );

    if (numBytes > existingBufferSize)
        return false;

    size_t numberOfElementsRead = fread( existingBuffer, sizeof( char ), existingBufferSize - 1, file );
    fclose( file );

    existingBuffer[numberOfElementsRead] = '\0';

    // buffer is bigger than data read
    if (numberOfElementsRead < numBytes)
        return false; 

    return true;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool LoadTextFileToExistingString( const std::string& filePath, std::string& existingString )
{
    FILE* file;
    fopen_s( &file, filePath.c_str(), "rb" );

    if (!file)
    {
        // freak out
        return false;
    }

    size_t neededStringSize = GetFileLength( file );

    // Grow/shrink
    existingString.resize( neededStringSize );
    fread( (void*)existingString.data(), sizeof( unsigned char ), neededStringSize, file );

    fclose( file );
    return true;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
char* LoadTextFileToNewBuffer( const std::string& filePath )
{
    FILE* file;
    fopen_s( &file, filePath.c_str(), "rb" );

    if (!file)
    {
        // freak out
        return NULL;
    }

    size_t numBytes = GetFileLength( file );

    char* newBuffer = new char[ numBytes + 1 ];
    fread( newBuffer, sizeof( char ), numBytes, file );
    fclose( file );

    newBuffer[numBytes] = '\0';

    return newBuffer;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool FindAllFilesOfType( const std::string& directory, const std::string& searchPattern, Strings& out_filesFound )
{
#ifdef WIN32

    _finddata_t fd;

    std::string searchPath = directory + searchPattern;
    intptr_t searchHandle = _findfirst( searchPath.c_str(), &fd );

    if (searchHandle != -1)
    {
        do
        {
            ConsolePrintf( "%s\n", fd.name );
            out_filesFound.push_back( directory + std::string( fd.name ) );
        } while (_findnext( searchHandle, &fd ) == 0);

    }
    else return false;

    if (_findclose( searchHandle ) == 0 && out_filesFound.size() != 0)
        return true;
    return false;
#else
    return false;
#endif


}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DoesFileExist( const std::string& filePath )
{
    bool fileExists = false;

#ifdef WIN32

    _finddata_t fd;

    std::string searchPath = filePath;
    intptr_t searchHandle = _findfirst( searchPath.c_str(), &fd );

    if (searchHandle != -1)
        fileExists = true;

    if (_findclose( searchHandle ) != 0)
        return false;

#endif

    return fileExists;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool DeleteFile_Relative( const std::string& filePath )
{
    std::wstring temp = std::wstring( filePath.begin(), filePath.end() );
    LPCWSTR file = temp.c_str();
    bool deleteSuccessful = DeleteFile( file ) != 0;

    return deleteSuccessful;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool CreateDirectory_Relative( const std::string& parentDirectory, const std::string& directoryName )
{
    std::string directoryStr = parentDirectory + directoryName;
    std::wstring temp = std::wstring( directoryStr.begin(), directoryStr.end() );
    LPCWSTR directory = temp.c_str();
    bool success = CreateDirectory( directory, NULL ) != 0;

    return success;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool GenerateFiles( int numFiles, int fileSizeInKB )
{
    CreateDirectory_Relative( "Data/", "Generated" );

    int numBytes = fileSizeInKB * 1024;
    unsigned char* buffer = new unsigned char[numBytes];

    for (int byteNum = 0; byteNum < numBytes; ++byteNum)
    {
        unsigned char byte = (unsigned char)GetRandomIntInRange( 0, 255 );
        buffer[byteNum] = byte;
    }

    for (int fileNum = 0; fileNum < numFiles; ++fileNum)
    {

        std::string fileName = "Data/Generated/generated_" + std::to_string( fileSizeInKB ) + "kb_file_" + std::to_string( fileNum + 1 ) + ".dat";

        FILE* file;
        fopen_s( &file, fileName.c_str(), "wb" );

        if (!file)
            FATAL_ERROR();

        for (int i = 0; i < numBytes; ++i)
            fputc( buffer[i], file );
       // fwrite( buffer, sizeof( unsigned char ), numBytes, file );
        fclose( file );
    }

    delete[] buffer;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned long HashBuffer_DJB2( unsigned char* buffer )
{
    unsigned long hash = 5381;
    int c;

    while (c = *buffer++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void ReverseBuffer( unsigned char* buffer, size_t bufferSize )
{
    unsigned char* low = buffer;
    unsigned char* hi = buffer + bufferSize - 1;

    unsigned char swap;

    while (low < hi)
    {
        swap = *low;
        *low++ = *hi;
        *hi-- = swap;
    }
}