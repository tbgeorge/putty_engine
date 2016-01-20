//=================================================================================
// DirectedLineSegment3D.cpp
// Author: Tyler George
// Date  : February 18, 2015
//=================================================================================

#include "Engine/Math/DirectedLineSegment3D.hpp"

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float DirectedLineSegment3D::CalcLength() const
{
	float lengthSquared = CalcLengthSquared();
	float length = sqrtf( lengthSquared );
	return length;
}