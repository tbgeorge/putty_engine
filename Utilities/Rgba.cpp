//=================================================================================
// Rgba.cpp
// Author: Tyler George
// Date  : January 29, 2015
//=================================================================================

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Utilities/Rgba.hpp"

///---------------------------------------------------------------------------------
/// Static Variables
///---------------------------------------------------------------------------------
const Rgba Rgba::WHITE( 0xFFFFFF, 1.0f );
const Rgba Rgba::BLACK( 0x000000, 1.0f );
const Rgba Rgba::TRANSPARENT_WHITE( 0xFFFFFF, 0.0f );
const Rgba Rgba::TRANSPARENT_BLACK( 0x000000, 0.0f );
const Rgba Rgba::AQUA( 0x00FFFF, 1.0f );
const Rgba Rgba::BLUE( 0x0000FF, 1.0f );
const Rgba Rgba::RED( 0xFF0000, 1.0f );
const Rgba Rgba::GREEN( 0x00FF00, 1.0f );
const Rgba Rgba::YELLOW( 0xFFFF00, 1.0f );
const Rgba Rgba::ORANGE( 0xFFA500, 1.0f );
const Rgba Rgba::GREY( 0x696969, 1.0f );
const Rgba Rgba::MAGENTA( 0xFF00FF, 1.0f );


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Rgba::Rgba( float fR, float fG, float fB, float fA )
{
    Clamp( fR, 0.0f, 1.0f );
    Clamp( fG, 0.0f, 1.0f );
    Clamp( fB, 0.0f, 1.0f );
    Clamp( fA, 0.0f, 1.0f );

	float rf = RangeMap( fR, 0.0f, 1.0f, 0.0f, 255.0f );
	float gf = RangeMap( fG, 0.0f, 1.0f, 0.0f, 255.0f );
	float bf = RangeMap( fB, 0.0f, 1.0f, 0.0f, 255.0f );
	float af = RangeMap( fA, 0.0f, 1.0f, 0.0f, 255.0f );

	r = (unsigned char) rf;
	g = (unsigned char) gf;
	b = (unsigned char) bf;
	a = (unsigned char) af;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Rgba::Rgba( int hexColor, float alpha )
{
	int red = hexColor & 0xFF0000;
	r = (unsigned char) ( red >> 16 );

	int green = hexColor & 0x00FF00;
	g = (unsigned char) ( green >> 8 );

	int blue = hexColor & 0x0000FF;
	b = (unsigned char) ( blue );

	float af = RangeMap( alpha, 0.0f, 1.0f, 0.0f, 255.0f );
	a = (unsigned char) af;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Rgba::Rgba( unsigned int hexColor )
{
    int red = hexColor & 0xFF000000;
    r = (unsigned char)(red >> 24);

    int green = hexColor & 0x00FF0000;
    g = (unsigned char)(green >> 16);

    int blue = hexColor & 0x0000FF00;
    b = (unsigned char)(blue >> 8);

    int alpha = hexColor & 0x000000FF;
    a = (unsigned char)(alpha);
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float Rgba::Rf() const
{
	float rf = RangeMap( (float) r, 0.0f, 255.0f, 0.0f, 1.0f );
	return rf;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float Rgba::Gf() const
{
	float gf = RangeMap( (float) g, 0.0f, 255.0f, 0.0f, 1.0f );
	return gf;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float Rgba::Bf() const
{
	float bf = RangeMap( (float) b, 0.0f, 255.0f, 0.0f, 1.0f );
	return bf;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float Rgba::Af() const
{
	float af = RangeMap( (float) a, 0.0f, 255.0f, 0.0f, 1.0f );
	return af;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Vector4f Rgba::GetAsVector4f() const
{
    return Vector4f( Rf(), Gf(), Bf(), Af() );
}

///---------------------------------------------------------------------------------
/// DOES NOT CHANGE ALPHA
///---------------------------------------------------------------------------------
const Rgba Rgba::operator*( float scale ) const 
{
	float newR = Rf() * scale;
	float newG = Gf() * scale;
	float newB = Bf() * scale;
	//float newA = Af() * scale;

	Rgba newRgba( newR, newG, newB, Af() );

	return newRgba;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const Rgba Rgba::operator+( const Rgba& colorToAdd ) const
{
	float newR = Rf() + colorToAdd.Rf();
	if( newR > 1.0f )
		newR = 1.0f;
	float newG = Gf() + colorToAdd.Gf();
	if( newG > 1.0f )
		newG = 1.0f;
	float newB = Bf() + colorToAdd.Bf();
	if( newB > 1.0f )
		newB = 1.0f;
	float newA = Af() + colorToAdd.Af();
	if( newA > 1.0f )
		newA = 1.0f;

	return Rgba( newR, newG, newB, newA );


}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const Rgba Rgba::operator-( const Rgba& colorToSubtract ) const
{
	float newR = Rf() - colorToSubtract.Rf();
	if( newR < 0.0f )
		newR = 0.0f;
	float newG = Gf() - colorToSubtract.Gf();
	if( newG < 0.0f )
		newG = 0.0f;
	float newB = Bf() - colorToSubtract.Bf();
	if( newB < 0.0f )
		newB = 0.0f;
	float newA = Af() - colorToSubtract.Af();
	if( newA < 0.0f )
		newA = 0.0f;

	return Rgba( newR, newG, newB, newA );


}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool Rgba::operator==(const Rgba& colorToCompare) const
{
    if (r == colorToCompare.r && g == colorToCompare.g && b == colorToCompare.b && a == colorToCompare.a)
        return true;
    return false;
}