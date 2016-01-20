//=================================================================================
// AnimatedTexture.cpp
// Author: Tyler George
// Date  : February 5, 2015
//=================================================================================

#include "Engine/Renderer/AnimatedTexture.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::map< std::string, AnimatedTexture* >	AnimatedTexture::s_globalAnimatedTextures ;


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
AnimatedTexture::AnimatedTexture( const std::string& path )
	: Texture( path )
{
}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
AnimatedTexture* AnimatedTexture::CreateOrGetAnimatedTexture( const std::string& path, unsigned int numberOfSprites, const Vector2& cellSize )
{
	
	AnimatedTextureMap::iterator animatedTextureIter = s_globalAnimatedTextures.find( path );
	if ( animatedTextureIter != s_globalAnimatedTextures.end() )
		return animatedTextureIter->second ;

	AnimatedTexture*	newAnimatedTexture			= new AnimatedTexture( path );
	newAnimatedTexture->m_cellSize = cellSize;
	newAnimatedTexture->m_totalNumberOfSprites = numberOfSprites;
	
	//Set cells/row and cells/column

	s_globalAnimatedTextures[ path ]	= newAnimatedTexture ;
	return newAnimatedTexture ;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
AnimatedTexture* AnimatedTexture::CreateOrGetAnimatedTexture( const std::string& path, unsigned int numSpritesWide, unsigned int numSpritesHigh )
{
	AnimatedTextureMap::iterator animatedTextureIter = s_globalAnimatedTextures.find( path );
	if ( animatedTextureIter != s_globalAnimatedTextures.end() )
		return animatedTextureIter->second ;

	AnimatedTexture*	newAnimatedTexture			= new AnimatedTexture( path );
	newAnimatedTexture->m_spritesWide = numSpritesWide;
	newAnimatedTexture->m_spritesHigh = numSpritesHigh;
	newAnimatedTexture->m_spriteSheetSize = newAnimatedTexture->m_size;

	newAnimatedTexture->m_totalNumberOfSprites = numSpritesHigh * numSpritesWide;
	newAnimatedTexture->m_cellSize = Vector2( newAnimatedTexture->m_size.x / numSpritesWide, newAnimatedTexture->m_size.y / numSpritesHigh );

	s_globalAnimatedTextures[ path ]	= newAnimatedTexture ;
	return newAnimatedTexture ;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void AnimatedTexture::CalcTextureCoordinatesAtPercentComplete( double percentComplete, Vector2& out_minCoordinates, Vector2& out_maxCoordinates ) const
{
	double percentBetweenSprites = 100.0 / m_totalNumberOfSprites;

	int requestedSprite = (int) floor( percentComplete / percentBetweenSprites );

	CalcTextureCoordinatesAtSpriteIndex( requestedSprite, out_minCoordinates, out_maxCoordinates );
	
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void AnimatedTexture::CalcTextureCoordinatesAtSpriteIndex( int spriteNumber, Vector2& out_minCoordinates, Vector2& out_maxCoordinates ) const
{
	float xMin = 0.0f;
	float yMax = 0.0f;

	for( int spriteIndex = 0; spriteIndex < spriteNumber; ++spriteIndex )
	{
		xMin += m_cellSize.x;
		if( xMin >= m_size.x )
		{
			xMin = 0.0f;
			yMax += m_cellSize.y;
		}
	}

	float xMax = RangeMap( xMin + m_cellSize.x, 0.0f, m_size.x, 0.0f, 1.0f );
	float yMin = RangeMap( yMax + m_cellSize.y, 0.0f, m_size.y, 0.0f, 1.0f );
	xMin = RangeMap( xMin, 0.0f, m_size.x, 0.0f, 1.0f );
	yMax = RangeMap( yMax, 0.0f, m_size.y, 0.0f, 1.0f );


	out_minCoordinates.SetXY( xMin, yMin );
	out_maxCoordinates.SetXY( xMax, yMax );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const Vector2 AnimatedTexture::CalcTextureCoordMinsAtSpriteCoordinates( const IntVector2& spriteCoords ) const
{
	float xMin = m_cellSize.x * (float) spriteCoords.x;
	float yMin = m_cellSize.y * (float) spriteCoords.y ;

	xMin = RangeMap( xMin, 0.0f, m_size.x, 0.0f, 1.0f );
	yMin = RangeMap( yMin, 0.0f, m_size.y, 0.0f, 1.0f );

	return Vector2( xMin, yMin );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const Vector2 AnimatedTexture::GetTexCoordSizePerCell() const 
{
	return Vector2( m_cellSize.x / m_size.x, m_cellSize.y / m_size.y );
}