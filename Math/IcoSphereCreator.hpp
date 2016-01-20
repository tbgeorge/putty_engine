//=================================================================================
// IcoSphereCreator.hpp
// Author: Tyler George
// Date  : May 28, 2015
//=================================================================================

#pragma once

#ifndef __included_IcoSphereCreator__
#define __included_IcoSphereCreator__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <map>
#include <vector>
#include <stdint.h>
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Renderer/Vertex3D.hpp"

enum IntendedRenderMode {
    TRIANGLES,
    LINES
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
struct TriangleIndices
{
	int m_v1;
	int m_v2;
	int m_v3;

	TriangleIndices( int v1, int v2, int v3 )
	{
		m_v1 = v1;
		m_v2 = v2;
		m_v3 = v3;
	}
};

typedef std::vector< TriangleIndices* > TriangleIndicesList;

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class IcoSphereCreator
{
public:
	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Initialization
	///---------------------------------------------------------------------------------
	
	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
	static IcoSphereCreator* GetInstance();

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
	PUC_Vertexes CreateIcoSphere( int recusionLevel, IntendedRenderMode mode, Rgba color = Rgba() );

	///---------------------------------------------------------------------------------
	/// Public Member Variables
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Public Static Variables
	///---------------------------------------------------------------------------------

private:
	///---------------------------------------------------------------------------------
	/// Private Functions
	///---------------------------------------------------------------------------------
	IcoSphereCreator() {}
	~IcoSphereCreator() {}

	int AddVertex( Vector3 vertex );
// 	int GetMidPoint( int pointIndexA, int pointIndexB );

	///---------------------------------------------------------------------------------
	/// Private Member Variables
	///---------------------------------------------------------------------------------
// 	std::map< int64_t, int > m_midPointIndexCache;
	Vector3s m_points;
	PUC_Vertexes m_vertexes;
	int m_index;

	///---------------------------------------------------------------------------------
	/// Private Static Variables
	///---------------------------------------------------------------------------------
	static IcoSphereCreator* s_theIcoSphereCreator;

};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

#endif