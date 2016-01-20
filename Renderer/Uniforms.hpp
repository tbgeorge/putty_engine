//=================================================================================
// Uniforms.hpp
// Author: Tyler George
// Date  : June 29, 2015
//=================================================================================

#pragma once

#ifndef __included_Uniforms__
#define __included_Uniforms__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class UniformBase
{
public:
    UniformBase( const std::string& n, const int& bp, const GLenum& t )
        : name( n )
        , bindPoint( bp )
        , type( t ) {}

    virtual void Bind( OpenGLRenderer* renderer, const unsigned int& shaderID ) = 0;

    int bindPoint;
    GLenum type;
    std::string name;
};

typedef std::vector< UniformBase* > Uniforms;

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class UniformFloat : public UniformBase
{
public:

    UniformFloat( const float& val, const std::string& name, const int& bp )
        : UniformBase( name, bp, GL_FLOAT )
        , value( val )
    {
    }

    void SetFloat( const float& val )
    {
        value = val;
    }

    void Bind( OpenGLRenderer* renderer, const unsigned int& shaderID )
    {
        renderer->ProgramBindFloat( shaderID, name.c_str(), value );
    }

    float value;
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class UniformInt : public UniformBase
{
public:

    UniformInt( const int& val, const std::string& name, const int& bp )
        : UniformBase( name, bp, GL_INT )
        , value( val )
    {
    }

    void SetInt( const int& val )
    {
        value = val;
    }

    void Bind( OpenGLRenderer* renderer, const unsigned int& shaderID )
    {
        renderer->ProgramBindInt32( shaderID, name.c_str(), value );
        renderer->GLCheckError();

    }

    int value;
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class UniformVec2 : public UniformBase
{
public:

    UniformVec2( const Vector2& val, const std::string& name, const int& bp )
        : UniformBase( name, bp, GL_FLOAT_VEC2 )
        , value( val )
    {
    }

    void SetVec2( const Vector2& val )
    {
        value = val;
    }

    void Bind( OpenGLRenderer* renderer, const unsigned int& shaderID )
    {
        renderer->ProgramBindVec2( shaderID, name.c_str(), value );
        renderer->GLCheckError();

    }

    Vector2 value;
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class UniformVec3 : public UniformBase
{
public:

    UniformVec3( const Vector3& val, const std::string& name, const int& bp )
        : UniformBase(name, bp, GL_FLOAT_VEC3 )
        , value( val )
    {
    }

    void SetVec3( const Vector3& val )
    {
        value = val;
    }

    void Bind( OpenGLRenderer* renderer, const unsigned int& shaderID )
    {
        renderer->ProgramBindVec3( shaderID, name.c_str(), value );
        renderer->GLCheckError();

    }

    Vector3 value;
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class UniformVec4f : public UniformBase
{
public:

    UniformVec4f( const Vector4f& val, const std::string& name, const int& bp )
        : UniformBase(name, bp, GL_FLOAT_VEC4)
        , value( val )
    {
    }

    void SetVec4f( const Vector4f& val )
    {
        value = val;
    }

    void Bind( OpenGLRenderer* renderer, const unsigned int& shaderID )
    {
        renderer->ProgramBindVec4f( shaderID, name.c_str(), value );
        renderer->GLCheckError();

    }

    Vector4f value;
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class UniformMatrixf : public UniformBase
{
public:

    UniformMatrixf( const Matrix4f& val, const std::string& name, const int& bp )
        : UniformBase(name, bp, GL_FLOAT_MAT4 )
        , value( val )
    {
    }

    void SetMatrixf( const Matrix4f& val )
    {
        value = val;
    }

    void Bind( OpenGLRenderer* renderer, const unsigned int& shaderID )
    {
        renderer->ProgramBindMatrix( shaderID, name.c_str(), value );
        renderer->GLCheckError();

    }

    Matrix4f value;
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class UniformTexture : public UniformBase
{
public:
    UniformTexture( OpenGLRenderer* renderer, Texture* t, const std::string& name, const int& textureIndex, const int& bp, const int& sampler = -1 )
        : UniformBase( name, bp, GL_TEXTURE )
        , texture( t )
        , texIndex( textureIndex )
    {
        if (sampler < 0)
            samplerID = renderer->GetDefaultSampler();
        else
            samplerID = (unsigned int)sampler;
    }

    void SetTexture( OpenGLRenderer* renderer, Texture* t, const int& sampler = -1 )
    {
        if (sampler < 0)
            samplerID = renderer->GetDefaultSampler();
        else
            samplerID = (unsigned int)sampler;

        texture = t;
    }

    void Bind( OpenGLRenderer* renderer, const unsigned int& shaderID )
    {
        
        renderer->ProgramBindSamplerIndex( shaderID, name.c_str(), texIndex );
        renderer->GLCheckError();

        renderer->SetActiveTexture( GL_TEXTURE0 + texIndex );
        renderer->GLCheckError();

        if (texture == nullptr)
            renderer->BindTexture( GL_TEXTURE_2D, NULL );
        else
        {
            renderer->BindTexture( GL_TEXTURE_2D, texture->GetPlatformHandle() );
            renderer->GLCheckError();
        }

        renderer->BindSampler( texIndex, samplerID );
        renderer->GLCheckError();

    }

    unsigned int samplerID;
    Texture* texture;
    int texIndex;
};

#endif