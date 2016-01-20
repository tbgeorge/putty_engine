//=================================================================================
// MeshCreator.hpp
// Author: Tyler George
// Date  : June 29, 2015
//=================================================================================

#pragma once

#ifndef __included_MeshCreator__
#define __included_MeshCreator__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Utilities/Rgba.hpp"
#include "Engine/Renderer/Vertex3D.hpp"

PUTBNC_Vertexes GenerateUVSphereVertexes( const Vector3& center, const float& radius, const int& slices, const int& stacks, const Rgba& color );
PUTBNC_Vertexes GenerateQuadVertexes( const Vector3& mins, const Vector3& maxs, const Rgba& color ); // 2D

PUTBNC_Vertexes GenerateLineVertexes( const Vector3& startPos, const Vector3& endPos, const Rgba& color );

PUTBNC_Vertexes GenerateCameraShapeVertexes( const Rgba& color );

#endif