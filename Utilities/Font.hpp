//=================================================================================
// Font.hpp
// Author: Tyler George
// Date  : June 10, 2015
//=================================================================================

#pragma once

#ifndef __included_Font__
#define __included_Font__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <string>
#include <map>
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Math/IntVector2.hpp"
class Font;
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
struct FontData
{
    std::string fontFace;
    int size;
    bool isBold;
    bool isItalic;
    std::string charset;
    bool isUnicode;
    int stretch;
    bool isSmoothed;
    int antiAliasLevels;
    Vector4< int > padding;
    IntVector2 spacing;
    int outline;

    int lineHeight;
    int baseLine;
    IntVector2 scale;
    int numPages;
    bool isPacked;
    int alphaChannel;
    int redChannel;
    int greenChannel;
    int blueChannel;

    int numGlyphs;
    int numKernings;
};

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
struct PageData
{
    int pageID;
    std::string pageFile;
};
typedef std::vector< PageData > Pages;

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
struct GlyphData
{
    int glyphID;
    IntVector2 unmappedUV;
    Vector2 mappedUVMins;
    IntVector2 unmappedSize;
    Vector2 mappedUVMaxs;
    IntVector2 offset;
    int advance;
    int pageID;
    int channel;
};
typedef std::map< int, GlyphData > GlyphMap;

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
struct KerningData
{
    IntVector2 glyphs;
    int amount;
};
typedef std::map< IntVector2, KerningData > KerningMap;

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------
typedef std::map< int, Texture* > PageFiles;
typedef std::map< std::string, Font* > FontMap;
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class Font
{
public:
	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
    Font( const std::string& fontInfoFilePath );
    ~Font() {}

	///---------------------------------------------------------------------------------
	/// Initialization
	///---------------------------------------------------------------------------------
    static void Shutdown();

	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
    static Font* CreateOrGetFont( const std::string fontName );

	///---------------------------------------------------------------------------------
	/// Update
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Render
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Public Member Variables
	///---------------------------------------------------------------------------------
    FontData fontData;

    IntVector2 scale;
    int lineHeight;
    int baseLine;

    int numPages;
    int numChars;
    int numKernings;

    Pages pages;
    PageFiles pageTextures;

    GlyphMap glyphs;
    KerningMap kernings;

	///---------------------------------------------------------------------------------
	/// Static Variables
	///---------------------------------------------------------------------------------


private:
	///---------------------------------------------------------------------------------
	/// Private Functions
	///---------------------------------------------------------------------------------
    void LoadFont( const std::string& fontInfoFilePath );
    void ParseFontData( std::string& infoLine, std::string& commonLine );
    void ParsePageData( std::string& pageLine, const std::string& fontDirectory );
    void ParseGlyphData( std::string& glyphLine );
    void ParseKerningData( std::string& kerningLine );
    void PrintFontData();
    void PrintPageData();
    void PrintGlyphData();
    void PrintKerningData();


	///---------------------------------------------------------------------------------
	/// Private Member Variables
	///---------------------------------------------------------------------------------
    static FontMap s_fonts;
};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

#endif