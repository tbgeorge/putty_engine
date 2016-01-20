//=================================================================================
// DirectedLineSegment2D.cpp
// Author: Tyler George
// Date  : January 23, 2015
//=================================================================================

#include "Engine/Math/DirectedLineSegment2D.hpp"


///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float DirectedLineSegment2D::CalcLength() const
{
	float lengthSquared = CalcLengthSquared();
	float length = sqrtf( lengthSquared );
	return length;
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float DirectedLineSegment2D::CalcHeadingDegrees() const
{
	Vector2 displacement = m_end - m_start;
	float headingDegrees = displacement.CalcHeadingDegrees();
	return headingDegrees;
}

///---------------------------------------------------------------------------------
/// very slow
///---------------------------------------------------------------------------------
float DirectedLineSegment2D::CalcHeadingRadians() const
{
	Vector2 displacement = m_end - m_start;
	float headingRadians = displacement.CalcHeadingRadians();
	return headingRadians;
}