//=================================================================================
// PuttyMesh.cpp
// Author: Tyler George
// Date  : June 24, 2015
//=================================================================================

#include "Engine/Renderer/PuttyMesh.hpp"
#include "Engine/Utilities/BinaryFileParser.hpp"
#include "../Utilities/BinaryBufferBuilder.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PuttyMesh::PuttyMesh( OpenGLRenderer* renderer )
    : m_renderer( renderer )
{
    m_vboID = m_renderer->CreateBuffer();
    m_iboID = m_renderer->CreateBuffer();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PuttyMesh::~PuttyMesh()
{
    m_renderer->DestroyBuffer( m_vboID );
    m_renderer->DestroyBuffer( m_iboID );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool PuttyMesh::LoadMeshFromC23File( const std::string& filePath )
{
    BinaryFileParser file( filePath );

    // header
    std::string fourCharacterCode;
    fourCharacterCode.resize( 4 );
    file.ReadChar( fourCharacterCode[0] ); 
    file.ReadChar( fourCharacterCode[1] );
    file.ReadChar( fourCharacterCode[2] );
    file.ReadChar( fourCharacterCode[3] );

    if (fourCharacterCode != "GC23")
        return false;
  
    unsigned char subType = file.ReadUnsignedChar();
    unsigned char version = file.ReadUnsignedChar();
    UNUSED( subType );
    UNUSED( version );

    std::string comment = file.ReadString();

    // vertexes
    unsigned int numVerticies = file.ReadUnsignedInt();

    PUTBNC_Vertexes verts;
    for (unsigned int vertIndex = 0; vertIndex < numVerticies; ++vertIndex)
    {
        Vertex3D_PUTBNC vertex;

        vertex.position = file.ReadVector3();
        vertex.color = file.ReadRgba();
        vertex.uv = file.ReadVector2();
        vertex.normal = file.ReadVector3();
        vertex.bitangent = file.ReadVector3();
//         ConsolePrintf( "Bitangent: %0.3f, %0.3f, %0.3f\n", vertex.bitangent.x, vertex.bitangent.y, vertex.bitangent.z );

        vertex.tangent = file.ReadVector3();
//         ConsolePrintf( "tangent: %0.3f, %0.3f, %0.3f\n", vertex.tangent.x, vertex.tangent.y, vertex.tangent.z );
        
        // boneIDs and weights
//         file.ReadInt();
//         file.ReadInt();
//         file.ReadInt();
//         file.ReadInt();
//         file.ReadVector3();

        verts.push_back( vertex );
    }

    unsigned int numIndicies = file.ReadUnsignedInt();

    std::vector< int > indicies;

    for (unsigned int indicieIndex = 0; indicieIndex < numIndicies; ++indicieIndex)
        indicies.push_back( file.ReadUnsignedInt() );


    DrawInstructions modelDrawInstructions( GL_TRIANGLES, verts.size(), indicies.size(), numIndicies == 0 ? false : true );
    VertexInformation modelVertInfo = Vertex3D_PUTBNC::GetVertexInfo();


    SetVertexData( verts.data(), modelDrawInstructions, modelVertInfo );
    SetIndexData( indicies.data(), numIndicies );
    

    return true;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool PuttyMesh::WriteMeshToC23File( const PUCNTB_Bones_Vertexes& meshData, const char* c23FilePath, const char* fileComment, unsigned char subType, unsigned char version )
{
    C23Header header;
    memcpy( header.m_fourCC, "GC23", 4 );
    header.m_subType = subType;
    header.m_version = version;

    size_t commentLength = strlen( fileComment );
    size_t estimatedBufferSize = sizeof( C23Header );
    estimatedBufferSize += commentLength + 1; // including zero terminator
    estimatedBufferSize += sizeof( meshData.size() );
    estimatedBufferSize += meshData.size() * sizeof( Vertex3D_PUCNTB_Bones );
    if (subType == 2)
    {
        estimatedBufferSize += sizeof( meshData.size() );
        estimatedBufferSize += meshData.size() * sizeof( unsigned int );
    }

    //std::vector< unsigned char > bufferData;
    BinaryBufferBuilder buffer( 0 );
    // 	buffer.WritePodStruct( header );

    buffer.AppendChar( header.m_fourCC[0] );
    buffer.AppendChar( header.m_fourCC[1] );
    buffer.AppendChar( header.m_fourCC[2] );
    buffer.AppendChar( header.m_fourCC[3] );
    buffer.AppendUnsignedChar( header.m_subType );
    buffer.AppendUnsignedChar( header.m_version );


    if (subType == 2)
    {
        WriteMeshToC23Buffer_StaticIndexedMesh( fileComment, meshData, buffer );
    }
    else
    {
        //		RECOVERABLE_ERROR( Stringf( "Cannot write C23 file \"%s\" - unknown subtype #%i requested!", c23FilePath, subType ) );
        return false;
    }

    buffer.WriteBufferToFile( c23FilePath );
    //WriteBinaryBufferToFile( buffer.GetBufferData(), buffer.GetBufferDataSize(), c23FilePath );
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void PuttyMesh::WriteMeshToC23Buffer_StaticIndexedMesh( const char* fileComment, const PUCNTB_Bones_Vertexes& meshData, BinaryBufferBuilder& buffer )
{
    unsigned int numVertexes = (unsigned int)meshData.size();
    unsigned int numIndexes = 0;// (unsigned int)meshData.size();
    buffer.AppendString( fileComment );

    buffer.AppendUnsignedInt( numVertexes );
    for (unsigned int vertexNum = 0; vertexNum < numVertexes; ++vertexNum)
    {
        const Vertex3D_PUCNTB_Bones& vertex = meshData[vertexNum];
        // 		buffer.AppendPodStruct( vertex );

        buffer.AppendVector3( vertex.position );
        buffer.AppendRgba( vertex.color );
        buffer.AppendVector2( vertex.uv );
        buffer.AppendVector3( vertex.normal );
        buffer.AppendVector3( vertex.tangent );
        buffer.AppendVector3( vertex.bitangent );
        buffer.AppendInt( vertex.boneIDs[0] );
        buffer.AppendInt( vertex.boneIDs[1] );
        buffer.AppendInt( vertex.boneIDs[2] );
        buffer.AppendInt( vertex.boneIDs[3] );
        buffer.AppendVector3( vertex.boneWeights );


    }

    if (numIndexes > 0)
    {
        buffer.AppendUnsignedInt( numIndexes );
        for (unsigned int indexNum = 0; indexNum < numIndexes; ++indexNum)
        {
//             buffer.AppendUnsignedInt( meshData[indexNum] );
        }
    }
    else
    {
        // Synthesize "fake" indexes; this file format subType demands indexed triangles
        buffer.AppendUnsignedInt( numVertexes );
        for (unsigned int vertexNum = 0; vertexNum < numVertexes; ++vertexNum)
        {
            buffer.AppendUnsignedInt( vertexNum );
        }
    }
}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void PuttyMesh::SetVertexData( void* data, DrawInstructions drawInstr, VertexInformation vertInfo )
{
    m_renderer->BindBufferData( GL_ARRAY_BUFFER, m_vboID, data, drawInstr.numVerts * vertInfo.stride );
    m_drawInstructions = drawInstr;
    m_vertInfo = vertInfo;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void PuttyMesh::SetIndexData( void* data, int numIndicies )
{
    m_renderer->BindBufferData( GL_ELEMENT_ARRAY_BUFFER, m_iboID, data, numIndicies * sizeof( int ) );
}