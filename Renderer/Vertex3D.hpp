//=================================================================================
// Vertex3D.hpp
// Author: Tyler George
// Date  : April 14, 2015
//=================================================================================

#pragma once

#ifndef __included_Vertex3D__
#define __included_Vertex3D__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Utilities/Rgba.hpp"
#include "Engine/Renderer/OpenGLDefines.hpp"

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
struct VertexAttributeProperties
{
    VertexAttributeProperties( const char* name, int count, unsigned int type, unsigned char normalize, size_t stride, int offset )
        : name( name ), count( count ), type( type ), normalize( normalize ), stride( stride ), offset( offset ) {}

    const char* name;
    int count;
    unsigned int type;
    unsigned char normalize;
    size_t stride;
    int offset;

};
typedef std::vector< VertexAttributeProperties > VertexLayout;


//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
struct VertexInformation
{
    size_t stride;
    VertexLayout layout;
};

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
struct Vertex3D_PC;
typedef std::vector< Vertex3D_PC > PC_Vertexes;


struct Vertex3D_PC
{
    Vector3 position;
    Rgba color;

    Vertex3D_PC( Vector3 p, Rgba c )
        : position( p ), color( c ) {}

    Vertex3D_PC() {}

    static VertexLayout GetVertexLayout()
    {
        static VertexLayout layout;

        if (layout.size() == 0)
        {
            layout.push_back( VertexAttributeProperties( "inPosition", 3, GL_FLOAT, false, sizeof( Vertex3D_PC ), offsetof( Vertex3D_PC, position ) ) );
            layout.push_back( VertexAttributeProperties( "inColor", 4, GL_UNSIGNED_BYTE, true, sizeof( Vertex3D_PC ), offsetof( Vertex3D_PC, color ) ) );
        }

        return layout;

    }

    static VertexLayout GetVertexLayout( PC_Vertexes& vertexes )
    {
        VertexLayout layout;

        layout.push_back( VertexAttributeProperties( "inPosition", 3, GL_FLOAT, false, sizeof( Vertex3D_PC ), (int) &vertexes[0].position ) );
        layout.push_back( VertexAttributeProperties( "inColor", 4, GL_UNSIGNED_BYTE, true, sizeof( Vertex3D_PC ), (int) &vertexes[0].color ) );

        return layout;
    }

    static VertexInformation GetVertexInfo()
    {
        static VertexInformation info;

        info.stride = sizeof( Vertex3D_PC );
        info.layout = GetVertexLayout();

        return info;
    }
};


//---------------------------------------------------------------------------------
// Deprecated
//---------------------------------------------------------------------------------
struct Vertex3D_PCT
{
	Vector3 position;
	Rgba	color;
	Vector2 texCoords;

    Vertex3D_PCT( Vector3 p, Rgba c, Vector2 t )
        : position( p ), color( c ), texCoords( t ) {}

    Vertex3D_PCT() {}

    static VertexLayout GetVertexLayout()
    {
        static VertexLayout layout;

        if (layout.size() == 0)
        {
            layout.push_back( VertexAttributeProperties( "inPosition", 3, GL_FLOAT, false, sizeof( Vertex3D_PCT ), offsetof( Vertex3D_PCT, position ) ) );
            layout.push_back( VertexAttributeProperties( "inColor", 4, GL_UNSIGNED_BYTE, false, sizeof( Vertex3D_PCT ), offsetof( Vertex3D_PCT, color ) ) );
            layout.push_back( VertexAttributeProperties( "inTexCoords", 2, GL_FLOAT, true, sizeof( Vertex3D_PCT ), offsetof( Vertex3D_PCT, texCoords ) ) );
        }

        return layout;

    }

    static VertexInformation GetVertexInfo()
    {
        static VertexInformation info;

        info.stride = sizeof( Vertex3D_PCT );
        info.layout = GetVertexLayout();

        return info;
    }
};

// Deprecated
typedef std::vector< Vertex3D_PCT > PCT_Vertexes;


//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
struct Vertex3D_PUC;
typedef std::vector< Vertex3D_PUC > PUC_Vertexes;

struct Vertex3D_PUC
{
    Vector3 position;
    Vector2 uv;
    Rgba color;

    Vertex3D_PUC( Vector3 p, Vector2 uv, Rgba c )
        : position( p ), uv( uv ), color( c ) {}

    Vertex3D_PUC() {}

    static VertexLayout GetVertexLayout()
    {
        static VertexLayout layout;

        if (layout.size() == 0)
        {
            layout.push_back( VertexAttributeProperties( "inPosition", 3, GL_FLOAT, false, sizeof( Vertex3D_PUC ), offsetof( Vertex3D_PUC, position ) ) );
            layout.push_back( VertexAttributeProperties( "inUV", 2, GL_FLOAT, false, sizeof( Vertex3D_PUC ), offsetof( Vertex3D_PUC, uv ) ) );
            layout.push_back( VertexAttributeProperties( "inColor", 4, GL_UNSIGNED_BYTE, true, sizeof( Vertex3D_PUC ), offsetof( Vertex3D_PUC, color ) ) );
        }


        return layout;

    }

    static VertexLayout GetVertexLayout( const PUC_Vertexes& vertexes )
    {
        VertexLayout layout;

        layout.push_back( VertexAttributeProperties( "inPosition", 3, GL_FLOAT, false, sizeof( Vertex3D_PUC ), (int)&vertexes[0].position ) );
        layout.push_back( VertexAttributeProperties( "inUV", 2, GL_FLOAT, false, sizeof( Vertex3D_PUC ), (int)&vertexes[0].uv ) );
        layout.push_back( VertexAttributeProperties( "inColor", 4, GL_UNSIGNED_BYTE, true, sizeof( Vertex3D_PUC ), (int)&vertexes[0].color ) );

        return layout;
    }

    static VertexInformation GetVertexInfo()
    {
        static VertexInformation info;

        info.stride = sizeof( Vertex3D_PUC );
        info.layout = GetVertexLayout();

        return info;
    }
};

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
struct Vertex3D_PUN
{
    Vector3 position;
    Vector2 uv;
    Vector3 normal;

    Vertex3D_PUN( Vector3 p, Vector2 uv, Vector3 n )
        : position( p ), uv( uv ), normal( n ) {}

    Vertex3D_PUN() {}

    static VertexLayout GetVertexLayout()
    {
        static VertexLayout layout;

        if (layout.size() == 0)
        {
            layout.push_back( VertexAttributeProperties( "inPosition", 3, GL_FLOAT, false, sizeof( Vertex3D_PUN ), offsetof( Vertex3D_PUN, position ) ) );
            layout.push_back( VertexAttributeProperties( "inUV", 2, GL_FLOAT, false, sizeof( Vertex3D_PUN ), offsetof( Vertex3D_PUN, uv ) ) );
            layout.push_back( VertexAttributeProperties( "inNormal", 3, GL_FLOAT, true, sizeof( Vertex3D_PUN ), offsetof( Vertex3D_PUN, normal ) ) );
        }


        return layout;

    }

    static VertexInformation GetVertexInfo()
    {
        static VertexInformation info;

        info.stride = sizeof( Vertex3D_PUN );
        info.layout = GetVertexLayout();

        return info;
    }
};

typedef std::vector< Vertex3D_PUN > PUN_Vertexes;

//---------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
struct Vertex3D_PUTBNC
{
    Vector3 position;
    Vector2 uv;
    Vector3 normal;
    Vector3 tangent;
    Vector3 bitangent;
    Rgba color;

    Vertex3D_PUTBNC( Vector3 p, Vector2 uv, Vector3 n, Vector3 t, Vector3 b, Rgba c )
        : position( p ), uv( uv ), normal( n ), tangent( t), bitangent(b), color(c) {}

    Vertex3D_PUTBNC() {}

    static VertexLayout GetVertexLayout()
    {
        static VertexLayout layout;

        if (layout.size() == 0)
        {
            layout.push_back( VertexAttributeProperties( "inPosition", 3, GL_FLOAT, false, sizeof( Vertex3D_PUTBNC ), offsetof( Vertex3D_PUTBNC, position ) ) );
            layout.push_back( VertexAttributeProperties( "inUV", 2, GL_FLOAT, false, sizeof( Vertex3D_PUTBNC ), offsetof( Vertex3D_PUTBNC, uv ) ) );
            layout.push_back( VertexAttributeProperties( "inNormal", 3, GL_FLOAT, true, sizeof( Vertex3D_PUTBNC ), offsetof( Vertex3D_PUTBNC, normal ) ) );
            layout.push_back( VertexAttributeProperties( "inTangent", 3, GL_FLOAT, true, sizeof( Vertex3D_PUTBNC ), offsetof( Vertex3D_PUTBNC, tangent ) ) );
            layout.push_back( VertexAttributeProperties( "inBitangent", 3, GL_FLOAT, true, sizeof( Vertex3D_PUTBNC ), offsetof( Vertex3D_PUTBNC, bitangent ) ) );
            layout.push_back( VertexAttributeProperties( "inColor", 4, GL_UNSIGNED_BYTE, true, sizeof( Vertex3D_PUTBNC ), offsetof( Vertex3D_PUTBNC, color ) ) );
        }


        return layout;

    }

    static VertexInformation GetVertexInfo()
    {
        static VertexInformation info;

        info.stride = sizeof( Vertex3D_PUTBNC );
        info.layout = GetVertexLayout();

        return info;
    }
};

typedef std::vector< Vertex3D_PUTBNC > PUTBNC_Vertexes;


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
struct Vertex3D_PUCNTB_Bones
{
    Vector3 position;
    Vector2 uv;
    Rgba color;
    Vector3 normal;
    Vector3 tangent;
    Vector3 bitangent;

    int boneIDs[3];
    Vector3 boneWeights;

    Vertex3D_PUCNTB_Bones( Vector3 p, Vector2 uv, Vector3 n, Vector3 t, Vector3 b, Rgba c )
        : position( p ), uv( uv ), normal( n ), tangent( t ), bitangent( b ), color( c ) {}

    Vertex3D_PUCNTB_Bones() {}

    static VertexLayout GetVertexLayout()
    {
        static VertexLayout layout;

        if (layout.size() == 0)
        {
            layout.push_back( VertexAttributeProperties( "inPosition", 3, GL_FLOAT, false, sizeof( Vertex3D_PUCNTB_Bones ), offsetof( Vertex3D_PUCNTB_Bones, position ) ) );
            layout.push_back( VertexAttributeProperties( "inUV", 2, GL_FLOAT, false, sizeof( Vertex3D_PUCNTB_Bones ), offsetof( Vertex3D_PUCNTB_Bones, uv ) ) );
            layout.push_back( VertexAttributeProperties( "inColor", 4, GL_UNSIGNED_BYTE, true, sizeof( Vertex3D_PUCNTB_Bones ), offsetof( Vertex3D_PUCNTB_Bones, color ) ) );
            layout.push_back( VertexAttributeProperties( "inNormal", 3, GL_FLOAT, true, sizeof( Vertex3D_PUCNTB_Bones ), offsetof( Vertex3D_PUCNTB_Bones, normal ) ) );
            layout.push_back( VertexAttributeProperties( "inTangent", 3, GL_FLOAT, true, sizeof( Vertex3D_PUCNTB_Bones ), offsetof( Vertex3D_PUCNTB_Bones, tangent ) ) );
            layout.push_back( VertexAttributeProperties( "inBitangent", 3, GL_FLOAT, true, sizeof( Vertex3D_PUCNTB_Bones ), offsetof( Vertex3D_PUCNTB_Bones, bitangent ) ) );
        }


        return layout;

    }

    static VertexInformation GetVertexInfo()
    {
        static VertexInformation info;

        info.stride = sizeof( Vertex3D_PUCNTB_Bones );
        info.layout = GetVertexLayout();

        return info;
    }
};

typedef std::vector< Vertex3D_PUCNTB_Bones > PUCNTB_Bones_Vertexes;

#endif