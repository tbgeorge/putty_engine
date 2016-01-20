//================================================================================================================================
// Texture.cpp
//================================================================================================================================


#include "Engine/Renderer/Texture.hpp"
#define STBI_HEADER_FILE_ONLY
#include "Engine/Renderer/stb_image.c"
#undef STBI_HEADER_FILE_ONLY

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Engine/Utilities/Console.hpp"
#include "../Math/IntVector2.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Engine/Renderer/stb_image_write.h"

#define GL_CLAMP_TO_EDGE 0x812F

//================================================================================================================================
//
//================================================================================================================================
std::map< std::string, Texture* >	Texture::s_globalTextures ;
unsigned int Texture::s_numGeneratedTextures = 0;


////===========================================================================================
///===========================================================================================
// TakeFullScreenshotJob class
///===========================================================================================
////===========================================================================================

////===========================================================================================
///===========================================================================================
// Constructors/Destructors
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
TakeFullScreenshotJob::TakeFullScreenshotJob( void* pixelData )
    : m_pixelData( pixelData )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
TakeFullScreenshotJob::~TakeFullScreenshotJob()
{

}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void TakeFullScreenshotJob::Run()
{
    int width = 1600;
    int height = 900;

    int stride = width;
    
    Rgba* pos = (Rgba*) m_pixelData;
    Rgba* flippedPos = new Rgba[width * height];

    for (int y = height - 1; y >= 0 ; --y )
    {
        int offset = y * stride;

        for (int x = 0; x < width; ++x)
        {
            *(flippedPos + offset) = *pos;
            offset += 1;
            pos += 1;
        }
    }

    delete m_pixelData;

    time_t nowTime = time( 0 );   // get time now
    struct tm * now = localtime( &nowTime );

    std::string fileName = "Data/Images/Screenshot_" + std::to_string( now->tm_mon + 1 ) + "-" + std::to_string( now->tm_mday ) + "-" + std::to_string( now->tm_year + 1900 ) + "_" + std::to_string( now->tm_hour ) + "-" + std::to_string( now->tm_min ) + ".png";
    stbi_write_png( fileName.c_str() , 1600, 900, 4, flippedPos, 4 * 1600);
    delete flippedPos;

}


///-----------------------------------------------------------------------------------
///
///-----------------------------------------------------------------------------------
Texture::Texture( const std::string& texturePath )
{
	int				x ;
	int				y ; 
	int				numberOfColorComponents ;
// 	int				requiredNumberOfColorComponents ;
	unsigned char*	pixelData	= stbi_load( texturePath.c_str(), &x, &y, &numberOfColorComponents, 0 );
    if (pixelData == NULL)
    {
        // MessageBoxAf( MBT_OK, "Error loading image", "There was an error loading the image at %s \n", texturePath.c_str() );
        m_isValid = false;
        return;
    }

	m_size.x		= (float)x ;
	m_size.y		= (float)y ;
	m_pathName		= texturePath ;

	// Enable texturing
	glEnable( GL_TEXTURE_2D );

	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	// Ask OpenGL for an unused texName (ID number) to use for this texture
	glGenTextures( 1, (GLuint*) &m_platformHandle );

	// Tell OpenGL to bind (set) this as the currently active texture
	glBindTexture( GL_TEXTURE_2D, m_platformHandle );

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...

	// Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); // one of: GL_NEAREST, GL_LINEAR
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ); // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR

	GLenum bufferFormat = GL_RGBA ; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	if( numberOfColorComponents == 3 )
		bufferFormat = GL_RGB ;

	// Todo: What happens if numComponents is neither 3 nor 4?

	GLenum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	glTexImage2D(				// Upload this pixel data to our new OpenGL texture
		GL_TEXTURE_2D,			// Creating this as a 2d texture
		0,						// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
		internalFormat,			// Type of texel format we want OpenGL to use for this texture internally on the video card
		(int)m_size.x,			// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
		(int)m_size.y,			// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
		0,						// Border size, in texels (must be 0 or 1)
		bufferFormat,			// Pixel format describing the composition of the pixel data in buffer
		GL_UNSIGNED_BYTE,		// Pixel color components are unsigned bytes (one byte per color/alpha channel)
		pixelData );	// Location of the actual pixel data bytes/buffer

	stbi_image_free( pixelData );
    m_isValid = true;
}

///-----------------------------------------------------------------------------------
///
///-----------------------------------------------------------------------------------
Texture::Texture( const int& width, const int& height )
{
    int				x = width;
    int				y = height;

    m_size.x = (float)x;
    m_size.y = (float)y;
    m_pathName = "generated";

    // Enable texturing
    glEnable( GL_TEXTURE_2D );

    // Tell OpenGL that our pixel data is single-byte aligned
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    // Ask OpenGL for an unused texName (ID number) to use for this texture
    glGenTextures( 1, (GLuint*)&m_platformHandle );

    // Tell OpenGL to bind (set) this as the currently active texture
    glBindTexture( GL_TEXTURE_2D, m_platformHandle );

    glTexImage2D(				// Upload this pixel data to our new OpenGL texture
        GL_TEXTURE_2D,			// Creating this as a 2d texture
        0,						// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
        GL_RGBA8,			    // Type of texel format we want OpenGL to use for this texture internally on the video card
        (int)m_size.x,			// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
        (int)m_size.y,			// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
        0,						// Border size, in texels (must be 0 or 1)
        GL_RGBA,			    // Pixel format describing the composition of the pixel data in buffer
        GL_UNSIGNED_BYTE,		// Pixel color components are unsigned bytes (one byte per color/alpha channel)
        0 );	                // Location of the actual pixel data bytes/buffer

    glBindTexture( GL_TEXTURE_2D, 0 );

    m_isValid = true;
}

///-----------------------------------------------------------------------------------
///
///-----------------------------------------------------------------------------------
Texture::Texture( const std::string& name, unsigned char* pixelData, const IntVector2& size, const unsigned int& numBytesPerPixel )
{
    int				x = size.x; 
    int				y = size.y;

    m_size.x = (float)x;
    m_size.y = (float)y;
    m_pathName = name;

    // Enable texturing
    glEnable( GL_TEXTURE_2D );

    // Tell OpenGL that our pixel data is single-byte aligned
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    // Ask OpenGL for an unused texName (ID number) to use for this texture
    glGenTextures( 1, (GLuint*)&m_platformHandle );

    // Tell OpenGL to bind (set) this as the currently active texture
    glBindTexture( GL_TEXTURE_2D, m_platformHandle );

    glTexImage2D(				// Upload pixel data to our new OpenGL texture
        GL_TEXTURE_2D,			// Creating this as a 2d texture
        0,						// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
        GL_RGBA8,			    // Type of texel format we want OpenGL to use for this texture internally on the video card
        (int)m_size.x,			// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
        (int)m_size.y,			// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
        0,						// Border size, in texels (must be 0 or 1)
        GL_RGB,			    // Pixel format describing the composition of the pixel data in buffer
        GL_UNSIGNED_BYTE,		// Pixel color components are unsigned bytes (one byte per color/alpha channel)
        pixelData );	        // Location of the actual pixel data bytes/buffer

    glBindTexture( GL_TEXTURE_2D, 0 );

    m_isValid = true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Texture::Shutdown()
{
    for (TextureMap::iterator texIter = s_globalTextures.begin(); texIter != s_globalTextures.end();)
    {
        Texture* tex = texIter->second;

        delete tex;
        texIter = s_globalTextures.erase( texIter );
    }
}

///-----------------------------------------------------------------------------------
///
///-----------------------------------------------------------------------------------
Texture* Texture::CreateOrGetTexture( const std::string& texturePath )
{
	TextureMap::iterator textureIter = s_globalTextures.find( texturePath );
	if ( textureIter != s_globalTextures.end() )
		return textureIter->second ;

	Texture*	newTexture			= new Texture( texturePath );
    if (newTexture->IsValid())
    {
        s_globalTextures[texturePath] = newTexture;
        return newTexture;
    }
    else
    {
        delete newTexture;
        return nullptr;
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Texture* Texture::CreateTexture2D( const int& width, const int& height )
{
    Texture* newTexture = new Texture( width, height );

    return newTexture;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Texture* Texture::CreateOrGetTexture( const std::string& name, unsigned char* pixelData, const IntVector2& size, const unsigned int& numPixels )
{
    TextureMap::iterator textureIter = s_globalTextures.find( name );
    if (textureIter != s_globalTextures.end())
        return textureIter->second;

    Texture*	newTexture = new Texture( name, pixelData, size, numPixels );
    s_globalTextures[name] = newTexture;
    return newTexture;
}