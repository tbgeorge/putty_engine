//=================================================================================
// Material.cpp
// Author: Tyler George
// Date  : June 29, 2015
//=================================================================================

#include "Engine/Renderer/Material.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Material::Material( OpenGLRenderer* renderer, const unsigned int& shaderID, const RenderState& renderstate )
    : m_renderer( renderer )
    , m_shaderID( shaderID )
    , m_renderState( renderstate )
    , m_currentTexIndex( 0 )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Material::~Material()
{
    for (Uniforms::iterator uniformIter = m_uniforms.begin(); uniformIter != m_uniforms.end();)
    {
        UniformBase* uniform = *uniformIter;
        uniformIter = m_uniforms.erase( uniformIter );
        delete uniform;
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
UniformBase* Material::FindUniform( const int& bindPoint )
{
    for (Uniforms::const_iterator uniformIter = m_uniforms.begin(); uniformIter != m_uniforms.end(); ++uniformIter)
    {
        UniformBase* uniform = *uniformIter;
        if (uniform->bindPoint == bindPoint)
            return uniform;
    }
    return nullptr;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Material::SetFloatUniform( const std::string& name, const float& val )
{
    int bindPoint = m_renderer->GetUniformLocation( m_shaderID, name );

    UniformBase* uniform = FindUniform( bindPoint );
    if (uniform)
    {
        UniformFloat* uniformFloat = dynamic_cast <UniformFloat*> (uniform);
        uniformFloat->SetFloat( val );
    }

    if (!uniform && bindPoint > 0)
    {
        m_uniforms.push_back( new UniformFloat( val, name, bindPoint ) );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Material::SetIntUniform( const std::string& name, const int& val )
{
    int bindPoint = m_renderer->GetUniformLocation( m_shaderID, name );

    UniformBase* uniform = FindUniform( bindPoint );
    if (uniform)
    {
        UniformInt* uniformInt = dynamic_cast <UniformInt*> (uniform);
        uniformInt->SetInt( val );
    }

    if (!uniform && bindPoint > 0)
    {
        m_uniforms.push_back( new UniformInt( val, name, bindPoint ) );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Material::SetVec2Uniform( const std::string& name, const Vector2& vec2 )
{
    int bindPoint = m_renderer->GetUniformLocation( m_shaderID, name );

    UniformBase* uniform = FindUniform( bindPoint );
    if (uniform)
    {
        UniformVec2* uniformVec2 = dynamic_cast <UniformVec2*> (uniform);
        uniformVec2->SetVec2( vec2 );
    }

    if (!uniform && bindPoint > 0)
    {
        m_uniforms.push_back( new UniformVec2( vec2, name, bindPoint ) );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Material::SetVec3Uniform( const std::string& name, const Vector3& vec3 )
{
    int bindPoint = m_renderer->GetUniformLocation( m_shaderID, name );

    UniformBase* uniform = FindUniform( bindPoint );
    if (uniform)
    {
        UniformVec3* uniformVec3 = dynamic_cast <UniformVec3*> (uniform);
        uniformVec3->SetVec3( vec3 );
    }

    if (!uniform && bindPoint > 0)
    {
        m_uniforms.push_back( new UniformVec3( vec3, name, bindPoint ) );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Material::SetVec4fUniform( const std::string& name, const Vector4f& vec4 )
{
    int bindPoint = m_renderer->GetUniformLocation( m_shaderID, name );

    UniformBase* uniform = FindUniform( bindPoint );
    if (uniform)
    {
        UniformVec4f* uniformVec4f = dynamic_cast <UniformVec4f*> (uniform);
        uniformVec4f->SetVec4f( vec4 );
    }

    if (!uniform && bindPoint > 0)
    {
        m_uniforms.push_back( new UniformVec4f( vec4, name, bindPoint ) );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Material::SetMatrix4fUniform( const std::string& name, const Matrix4f& mat )
{
    int bindPoint = m_renderer->GetUniformLocation( m_shaderID, name );

    UniformBase* uniform = FindUniform( bindPoint );
    if (uniform)
    {
        UniformMatrixf* uniformMatrixf = dynamic_cast <UniformMatrixf*> (uniform);
        uniformMatrixf->SetMatrixf( mat );
    }

    if (!uniform && bindPoint > 0)
    {
        m_uniforms.push_back( new UniformMatrixf( mat, name, bindPoint ) );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Material::SetTextureUniform( const std::string& name, Texture* tex, const int& sampler /* = -1 */ )
{
    int bindPoint = m_renderer->GetUniformLocation( m_shaderID, name );

    UniformBase* uniform = FindUniform( bindPoint );
    if (uniform)
    {
        UniformTexture* uniformTex = dynamic_cast <UniformTexture*> (uniform);
        uniformTex->SetTexture( m_renderer, tex, sampler );
    }

    if (!uniform && bindPoint > 0)
    {
        UniformTexture* texture = new UniformTexture( m_renderer, tex, name, m_currentTexIndex++, bindPoint );
        m_uniforms.push_back(  texture );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Material::BindAllUniforms()
{
    for (Uniforms::const_iterator uniformIter = m_uniforms.begin(); uniformIter != m_uniforms.end(); ++uniformIter)
    {
        UniformBase* uniform = *uniformIter;
        uniform->Bind( m_renderer, m_shaderID );
    }
}