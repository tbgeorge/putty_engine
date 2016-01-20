//=================================================================================
// PuttyMesh.hpp
// Author: Tyler George
// Date  : June 24, 2015
//=================================================================================

#pragma once

#ifndef __included_PuttyMesh__
#define __included_PuttyMesh__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Renderer/Vertex3D.hpp"
#include "Engine/Utilities/BinaryBufferBuilder.hpp"

struct DrawInstructions
{
    GLenum drawMode;
    unsigned int numVerts;
    unsigned int numIndicies;
    bool isIndexedCall;

    DrawInstructions() {}

    DrawInstructions( GLenum dm, unsigned int nv, unsigned int ni, bool indexed )
        : drawMode( dm ), numVerts( nv ), numIndicies( ni ), isIndexedCall( indexed ) {}
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
struct C23Header
{
    char				m_fourCC[4];
    unsigned char		m_subType;
    unsigned char		m_version;
};

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class PuttyMesh
{
public:
	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
    PuttyMesh( OpenGLRenderer* renderer );
    ~PuttyMesh();

	///---------------------------------------------------------------------------------
	/// Initialization
	///---------------------------------------------------------------------------------
    bool LoadMeshFromC23File( const std::string& filePath );
    static bool WriteMeshToC23File( const PUCNTB_Bones_Vertexes& meshData, const char* c23FilePath, const char* fileComment, unsigned char subType, unsigned char version );
    static void WriteMeshToC23Buffer_StaticIndexedMesh( const char* fileComment, const PUCNTB_Bones_Vertexes& meshData, BinaryBufferBuilder& buffer );

	
	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
    DrawInstructions GetDrawInstructions() { return m_drawInstructions; }
    VertexInformation GetVertInfo() { return m_vertInfo; }
    unsigned int GetVBOID() { return m_vboID; }
    unsigned int GetIBOID() { return m_iboID; }

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
    void SetVertexData( void* data, DrawInstructions drawInstr, VertexInformation vertInfo );
    void SetIndexData( void* data, int numIndicies );
    void SetDrawInstructions( DrawInstructions dI ) { m_drawInstructions = dI; }

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
    OpenGLRenderer* m_renderer;

    unsigned int m_vboID;
    unsigned int m_iboID;
    DrawInstructions m_drawInstructions;
    VertexInformation m_vertInfo;

};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

#endif