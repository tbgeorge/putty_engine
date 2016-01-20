//================================================================================================================================
// Texture.hpp
//================================================================================================================================


#pragma once
#ifndef __included_Texture__
#define __included_Texture__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <string>
#include <map>
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Utilities/Rgba.hpp"
#include "Engine/Multi-Threading/JobManager.hpp"

class Texture;

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------
typedef std::map< std::string, Texture* >	TextureMap ;


///---------------------------------------------------------------------------------
/// Take Fullscreen Screenshot Job class
///---------------------------------------------------------------------------------
class TakeFullScreenshotJob : public Job
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    TakeFullScreenshotJob( void* pixelData );
    ~TakeFullScreenshotJob();

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    virtual void Run();

private:
    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    void* m_pixelData;
};


//================================================================================================================================
//
//================================================================================================================================
class Texture
{
public:
	///---------------------------------------------------------------------------------
	/// Destructor
	///---------------------------------------------------------------------------------
	virtual ~Texture() {}

	///-----------------------------------------------------------------------------------
	/// Accessors
	///-----------------------------------------------------------------------------------
	const std::string&		GetName() const				{ return m_pathName ; }
	unsigned int			GetPlatformHandle() const	{ return m_platformHandle ; }
    Vector2                 GetSize() const             { return m_size;  }
    bool                    IsValid() const             { return m_isValid; }

	///-----------------------------------------------------------------------------------
	/// 
	///-----------------------------------------------------------------------------------
    static void Shutdown();
	static Texture*	CreateOrGetTexture( const std::string& texturePath );
    static Texture* CreateTexture2D( const int& width, const int& height );
    static Texture* CreateOrGetTexture( const std::string& name, unsigned char* pixelData, const IntVector2& size, const unsigned int& numBytesPerPixel );


protected:
	///-----------------------------------------------------------------------------------
	///
	///-----------------------------------------------------------------------------------
	Texture( const std::string& pathName );
    Texture( const int& width, const int& height );
    Texture( const std::string& name, unsigned char* pixelData, const IntVector2& size, const unsigned int& numPixels );



	std::string				m_pathName;
	Vector2					m_size;
	unsigned int			m_platformHandle;
    bool                    m_isValid;
    static unsigned int     s_numGeneratedTextures;
	static TextureMap		s_globalTextures;
};

#endif // __included_Texture__


