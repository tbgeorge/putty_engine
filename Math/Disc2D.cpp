//=================================================================================
// Disc2D.cpp
// Author: Tyler George
// Date  : January 21, 2015
//=================================================================================

#include "Engine/Math/Disc2D.hpp"

const Disc2D Disc2D::UNIT_CIRCLE( Vector2( 0.0f, 0.0f ), 1.0f );

///---------------------------------------------------------------------------------
/// very slow ( CalcDistance uses sqrt )
///---------------------------------------------------------------------------------
void Disc2D::StretchToIncludePoint( const Vector2& point )
{
	if( !IsPointInside( point ) )
		m_radius = CalcDistance( m_center, point );
}