//=================================================================================
// AnimatedTexture.hpp
// Author: Tyler George
// Date  : February 5, 2015
//=================================================================================

#pragma once
#ifndef __included_AnimatedTexture__
#define __included_AnimatedTexture__

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Renderer/Texture.hpp"

class AnimatedTexture ;
typedef std::map< std::string, AnimatedTexture* >	AnimatedTextureMap ;

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class AnimatedTexture : public Texture
{
public:
	///---------------------------------------------------------------------------------
	/// Destructor
	///---------------------------------------------------------------------------------
	~AnimatedTexture() {}

	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
	static AnimatedTexture* CreateOrGetAnimatedTexture( const std::string& path, unsigned int numberOfSprites, const Vector2& cellSize );
	static AnimatedTexture* CreateOrGetAnimatedTexture( const std::string& path, unsigned int numSpritesWide, unsigned int numSpritesHigh );

	
	void					CalcTextureCoordinatesAtPercentComplete( double percentComplete, Vector2& out_minCoordinates, Vector2& out_maxCoordinates ) const;
	void					CalcTextureCoordinatesAtSpriteIndex( int spriteNumber, Vector2& out_minCoordinates, Vector2& out_maxCoordinates ) const;
	const Vector2			CalcTextureCoordMinsAtSpriteCoordinates( const IntVector2& spriteCoords ) const;
	const Vector2			GetCellSize() const { return m_cellSize; }

	const Vector2				GetTexCoordSizePerCell() const;

protected:
	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
	AnimatedTexture( const std::string& path );

private:
	Vector2			m_cellSize;
	unsigned int	m_totalNumberOfSprites;

	Vector2			m_spriteSheetSize;
	unsigned int	m_spritesWide;
	unsigned int	m_spritesHigh;

	///---------------------------------------------------------------------------------
	/// Static private member variables
	///---------------------------------------------------------------------------------
	static AnimatedTextureMap s_globalAnimatedTextures;

};

#endif