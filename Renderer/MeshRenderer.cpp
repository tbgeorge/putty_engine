//=================================================================================
// MeshRenderer.cpp
// Author: Tyler George
// Date  : June 29, 2015
//=================================================================================

#include "Engine/Renderer/MeshRenderer.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
MeshRenderer::MeshRenderer( OpenGLRenderer* renderer )
    : m_renderer( renderer )
    , m_mesh( nullptr )
    , m_material( nullptr )
{
    m_vaoID = m_renderer->CreateVAO();
    m_renderer->GLCheckError();

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
MeshRenderer::~MeshRenderer()
{
    m_renderer->DestroyVAO( m_vaoID );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void MeshRenderer::SetMesh( PuttyMesh* mesh )
{
    m_mesh = mesh;

    if (m_material)
    {
        m_renderer->BindVertexArray( m_vaoID );
        m_renderer->GLCheckError();

        m_renderer->BindVAOBuffer( m_material->GetShaderID(), m_vaoID, m_mesh->GetVBOID(), m_mesh->GetVertInfo().layout );
        m_renderer->GLCheckError();

        m_renderer->BindVertexArray( NULL );
        m_renderer->GLCheckError();

    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void MeshRenderer::SetMaterial( Material* material, bool bindLights )
{
    m_material = material;
    m_bindLights = bindLights;

    if (m_mesh)
    {
        m_renderer->BindVertexArray( m_vaoID );
        m_renderer->GLCheckError();

        m_renderer->BindVAOBuffer( m_material->GetShaderID(), m_vaoID, m_mesh->GetVBOID(), m_mesh->GetVertInfo().layout );
        m_renderer->GLCheckError();

        m_renderer->BindVertexArray( NULL );
        m_renderer->GLCheckError();

    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void MeshRenderer::Render( const Matrix4f& model, const Matrix4f& view, const Matrix4f& proj, const Lights& lights )
{
    if (!m_renderer)
        return;

    m_renderer->GLCheckError();

    m_renderer->BindProgram( m_material->GetShaderID() );
    m_renderer->GLCheckError();

    m_renderer->BindVertexArray( m_vaoID );
    m_renderer->GLCheckError();

    m_renderer->BindRenderState( m_material->GetRenderState() );
    m_renderer->GLCheckError();


    m_renderer->ProgramBindMatrix( m_material->GetShaderID(), "gModel", model );
    m_renderer->GLCheckError();

    m_renderer->ProgramBindMatrix( m_material->GetShaderID(), "gView", view );
    m_renderer->GLCheckError();

    m_renderer->ProgramBindMatrix( m_material->GetShaderID(), "gProj", proj );
    m_renderer->GLCheckError();

    if (m_bindLights)
    {
        m_renderer->ProgramBindLights( m_material->GetShaderID(), "gLightBlock", lights );
        m_renderer->GLCheckError();
    }


    m_material->BindAllUniforms();
    m_renderer->GLCheckError();

    if ( m_mesh->GetDrawInstructions().isIndexedCall )
    {
        m_renderer->BindIndexArray( m_mesh->GetIBOID() );
        m_renderer->DrawElements( m_mesh->GetDrawInstructions().drawMode, m_mesh->GetDrawInstructions().numIndicies );
        m_renderer->GLCheckError();
    }
    else
    {
        m_renderer->DrawArrays( m_mesh->GetDrawInstructions().drawMode, 0, m_mesh->GetDrawInstructions().numVerts );
        m_renderer->GLCheckError();
    }


    m_renderer->BindProgram( NULL );
    m_renderer->BindVertexArray( NULL );
}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void MeshRenderer::Render( const Matrix4f& model, const Matrix4f& view, const Matrix4f& proj )
{
    if (!m_renderer)
        return;

    m_renderer->GLCheckError();

    m_renderer->BindProgram( m_material->GetShaderID() );
    m_renderer->GLCheckError();

    m_renderer->BindVertexArray( m_vaoID );
    m_renderer->GLCheckError();

    m_renderer->BindRenderState( m_material->GetRenderState() );
    m_renderer->GLCheckError();


    m_renderer->ProgramBindMatrix( m_material->GetShaderID(), "gModel", model );
    m_renderer->GLCheckError();

    m_renderer->ProgramBindMatrix( m_material->GetShaderID(), "gView", view );
    m_renderer->GLCheckError();

    m_renderer->ProgramBindMatrix( m_material->GetShaderID(), "gProj", proj );
    m_renderer->GLCheckError();


    m_material->BindAllUniforms();
    m_renderer->GLCheckError();


    if (m_mesh->GetDrawInstructions().isIndexedCall)
    {
        m_renderer->BindIndexArray( m_mesh->GetIBOID() );
        m_renderer->DrawElements( m_mesh->GetDrawInstructions().drawMode, m_mesh->GetDrawInstructions().numIndicies );
        m_renderer->GLCheckError();
    }
    else
    {
        m_renderer->DrawArrays( m_mesh->GetDrawInstructions().drawMode, 0, m_mesh->GetDrawInstructions().numVerts );
        m_renderer->GLCheckError();
    }


    m_renderer->BindProgram( NULL );
    m_renderer->BindVertexArray( NULL );
}