//=================================================================================
// FileUtilities.hpp
// Author: Tyler George
// Date  : April 9, 2015
//=================================================================================

#pragma once
#ifndef __included_FileUtilities__
#define __included_FileUtilities__

#include <stdio.h>
#include <string>
#include <vector>
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Multi-Threading/JobManager.hpp"

typedef std::vector< unsigned char > ByteVector;

////===========================================================================================
///===========================================================================================
// Load File Job
///===========================================================================================
////===========================================================================================
class LoadFileJob : public Job
{


public:
    ///---------------------------------------------------------------------------------
    /// Internal Structs
    ///---------------------------------------------------------------------------------
    struct LoadFileData
    {
        unsigned char* buffer;
        size_t bufferSize;
    };

    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    LoadFileJob( const std::string filePath );
    ~LoadFileJob();

protected:
    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    virtual void Run();

private:
    ///---------------------------------------------------------------------------------
    /// Private Member variables
    ///---------------------------------------------------------------------------------
    std::string m_filePath;
    unsigned char* m_buffer;
};

////===========================================================================================
///===========================================================================================
// Save File Job
///===========================================================================================
////===========================================================================================
class SaveFileJob : public Job
{


public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    SaveFileJob( const std::string filePath, unsigned char* buffer, size_t bufferSize );
    ~SaveFileJob();

protected:
    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    virtual void Run();

private:
    ///---------------------------------------------------------------------------------
    /// Private Member variables
    ///---------------------------------------------------------------------------------
    std::string m_filePath;
    unsigned char* m_buffer;
    size_t m_bufferSize;
};


////===========================================================================================
///===========================================================================================
// Generate Files Job
///===========================================================================================
////===========================================================================================
class GenerateFilesJob : public Job
{


public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    GenerateFilesJob( int numFiles, int fileSize );
    ~GenerateFilesJob();

protected:
    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    virtual void Run();

private:
    ///---------------------------------------------------------------------------------
    /// Private Member variables
    ///---------------------------------------------------------------------------------
    int m_numFiles;
    int m_fileSize;
};

////===========================================================================================
///===========================================================================================
// Hash Buffer Job
///===========================================================================================
////===========================================================================================
class HashBufferJob : public Job
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    HashBufferJob( unsigned char* buffer );
    ~HashBufferJob();

protected:
    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    virtual void Run();

private:
    ///---------------------------------------------------------------------------------
    /// Private Member variables
    ///---------------------------------------------------------------------------------
    unsigned char* m_buffer;
};

////===========================================================================================
///===========================================================================================
// Reverse Buffer Job
///===========================================================================================
////===========================================================================================
class ReverseBufferJob : public Job
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    ReverseBufferJob( unsigned char* buffer, size_t bufferSize );
    ~ReverseBufferJob();

protected:
    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    virtual void Run();

private:
    ///---------------------------------------------------------------------------------
    /// Private Member variables
    ///---------------------------------------------------------------------------------
    unsigned char* m_buffer;
    size_t m_bufferSize;
};

////===========================================================================================
///===========================================================================================
// External Functions
///===========================================================================================
////===========================================================================================
size_t GetFileLength( FILE* fileHandle );

bool WriteBufferToFile( const std::string& filePath, const unsigned char* buffer, const size_t& bufferSize );
bool WriteByteVectorToFile( const std::string& filePath, const ByteVector& byteVector );
bool WriteTextBufferToFile( const std::string& filePath, const char* buffer, const size_t& bufferSize );
bool WriteStringToFile( const std::string& filePath, const std::string& stringBuffer );

bool LoadBinaryFileToExistingBuffer( const std::string& filePath, unsigned char* existingBuffer, const size_t& existingBufferSize );
bool LoadBinaryFileToExistingByteVector( const std::string& filePath, ByteVector& existingVectorBuffer );
unsigned char* LoadBinaryFileToNewBuffer( const std::string& filePath, size_t& out_bufferSize );

bool LoadTextFileToExistingBuffer( const std::string& filePath, char* existingBuffer, const size_t& existingBufferSize );
bool LoadTextFileToExistingString( const std::string& filePath, std::string& existingString );
char* LoadTextFileToNewBuffer( const std::string& filePath );

bool FindAllFilesOfType( const std::string& directory, const std::string& searchString, Strings& out_filesFound );
bool DoesFileExist( const std::string& filePath );
bool DeleteFile_Relative( const std::string& filePath );
bool CreateDirectory_Relative( const std::string& parentDirectory, const std::string& directoryName );

bool GenerateFiles( int numFiles, int fileSizeInKB );

unsigned long HashBuffer_DJB2( unsigned char* buffer );
void ReverseBuffer( unsigned char* buffer, size_t bufferSize );

#endif