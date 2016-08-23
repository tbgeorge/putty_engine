//=================================================================================
// Rgba.hpp
// Author: Tyler George
// Date  : January 28, 2015
//=================================================================================

#pragma once

#ifndef __included_Rgba__
#define __included_Rgba__

#include <vector>
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vector4.hpp"

class Rgba
{
public:

	///---------------------------------------------------------------------------------
	/// Constructors
	///---------------------------------------------------------------------------------
	Rgba() : r( 255 ), g( 255 ), b( 255 ), a( 255 ) {}
	explicit Rgba( unsigned char rparam, unsigned char gparam, unsigned char bparam, unsigned char aparam = 255 );
    explicit Rgba( unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255 );
	explicit Rgba( float fR, float fG, float fB, float fA = 1.0f );
	explicit Rgba( int hexColor, float alpha = 1.0f );
    explicit Rgba( unsigned int hexColor );

	///---------------------------------------------------------------------------------
	/// Accessors
	///---------------------------------------------------------------------------------
	float Rf() const;
	float Gf() const;
	float Bf() const;
	float Af() const;

    Vector4f GetAsVector4f() const;

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void SetAlpha( float alpha );

	///---------------------------------------------------------------------------------
	/// Operators
	///---------------------------------------------------------------------------------
	const Rgba operator * ( float scale ) const;
	const Rgba operator + ( const Rgba& colorToAdd ) const;
	const Rgba operator - ( const Rgba& colorToSubtract ) const;
    bool operator == (const Rgba& colorToCompare) const;

    ///---------------------------------------------------------------------------------
    ///
    ///---------------------------------------------------------------------------------
//     friend const Rgba Interpolate( const Rgba& startColor, const Rgba& endColor, const float& fraction );

	///---------------------------------------------------------------------------------
	/// Public Member Variables
	///---------------------------------------------------------------------------------
	unsigned char r; // 0 - 255 Red value
	unsigned char g; // 0 - 255 Green value
	unsigned char b; // 0 - 255 Blue value
	unsigned char a; // 0 - 255 Alpha value
    
	///---------------------------------------------------------------------------------
	/// Static Member Variables
	///---------------------------------------------------------------------------------
	static const Rgba WHITE;
	static const Rgba BLACK;
	static const Rgba TRANSPARENT_WHITE;
	static const Rgba TRANSPARENT_BLACK;
	static const Rgba AQUA;
	static const Rgba BLUE;
	static const Rgba RED;
	static const Rgba GREEN;
	static const Rgba YELLOW;
	static const Rgba ORANGE;
	static const Rgba GREY;
	static const Rgba MAGENTA;
};
typedef std::vector< Rgba > RgbaList;
///---------------------------------------------------------------------------------
/// Inline Functions
///---------------------------------------------------------------------------------
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline Rgba::Rgba( unsigned char rp, unsigned char gp, unsigned char bp, unsigned char ap )
	: r( rp )
	, g( gp )
	, b( bp )
    , a( ap )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline Rgba::Rgba( unsigned int r, unsigned int g, unsigned int b, unsigned int a /* = 255 */ )
{
    if (r > 255)
        r = 255;
    if (g > 255)
        g = 255;
    if (b > 255)
        b = 255;
    if (a > 255)
        a = 255;

    this->r = (unsigned char) r;
    this->g = (unsigned char) g;
    this->b = (unsigned char) b;
    this->a = (unsigned char) a;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
// inline const Rgba Interpolate( const Rgba& startColor, const Rgba& endColor, const float& fraction )
// {
//     float r = Interpolate( startColor.Rf(), endColor.Rf(), fraction );
//     float g = Interpolate( startColor.Gf(), endColor.Gf(), fraction );
//     float b = Interpolate( startColor.Bf(), endColor.Bf(), fraction );
//     float a = Interpolate( startColor.Af(), endColor.Af(), fraction );
// 
//     return Rgba( r, g, b, a );
// }

#endif