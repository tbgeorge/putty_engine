//=================================================================================
// Font.cpp
// Author: Tyler George
// Date  : June 10, 2015
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include <fstream>
#include <iostream>
#include <vector>
#include "Engine/Utilities/Font.hpp"
#include "Engine/Utilities/Utilities.hpp"

////===========================================================================================
///===========================================================================================
// Static Variable initialization
///===========================================================================================
////===========================================================================================

FontMap Font::s_fonts;


////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Font* Font::CreateOrGetFont( const std::string fontName )
{
    FontMap::const_iterator fontIter = s_fonts.find( fontName );

    if (fontIter == s_fonts.end())
    {
        Font* newFont = new Font( fontName + ".fnt" );
        s_fonts.insert( std::pair< std::string, Font* >( fontName, newFont ) );
        return newFont;
    }

    else
        return fontIter->second;
}

////===========================================================================================
///===========================================================================================
// Constructors/Destructors
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Font::Font( const std::string& fontInfoFilePath )
    : lineHeight( 0 )
    , baseLine( 0 )
    , numPages( 0 )
    , numChars( 0 )
    , numKernings( 0 )
{
    LoadFont( fontInfoFilePath );
}

////===========================================================================================
///===========================================================================================
// Initialization
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Font::Shutdown()
{
    for (FontMap::iterator fontIter = s_fonts.begin(); fontIter != s_fonts.end();)
    {
        Font* font = fontIter->second;

        delete font;
        fontIter = s_fonts.erase( fontIter );
    }
}

////===========================================================================================
///===========================================================================================
// Private Functions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Font::LoadFont( const std::string& fontInfoFilePath )
{
    std::ifstream fontInfoFile( fontInfoFilePath );

    std::string fontInfoFilePathFixed = ConvertBackSlashesToForwardSlashes( fontInfoFilePath );
    std::string fontDirectory = fontInfoFilePath.substr( 0, fontInfoFilePathFixed.find_last_of( "/" ) + 1 );
    //ConsolePrintf( "%s\n", fontDirectory.c_str() );
    std::string line = "";

    std::vector< std::string > lines;

    while (fontInfoFile.good())
    {
        getline( fontInfoFile, line );
        lines.push_back( line );
    }

    int currentlineIndex = 0;

    /// Line Definitions
    // 0                - info
    // 1                - common
    std::string info = lines[currentlineIndex++];
    std::string common = lines[currentlineIndex++];

    // parse info
    ParseFontData( info, common );

    // parse pages
    std::string pageLine = "";
    for (int pageNum = 0; pageNum < numPages; ++pageNum)
    {
        pageLine = lines[currentlineIndex++];
        ParsePageData( pageLine, fontDirectory );
    }

    //parse chars
    std::string charCount = lines[currentlineIndex++];
    charCount.erase( 0, charCount.find( "count=" ) + 6 );
    fontData.numGlyphs = stoi( charCount );
    numChars = fontData.numGlyphs;

    std::string glyphLine = "";
    for (int glyphNum = 0; glyphNum < numChars; ++glyphNum)
    {
        glyphLine = lines[currentlineIndex++];
        ParseGlyphData( glyphLine );
    }

    // parse kernings
    std::string kerningCount = lines[currentlineIndex++];
    if (kerningCount.find( "count=" ) != std::string::npos)
    {
        kerningCount.erase( 0, kerningCount.find( "count=" ) + 6 );
        fontData.numKernings = stoi( kerningCount );
        numKernings = fontData.numKernings;

        std::string kerningLine = "";
        for (int kerningNum = 0; kerningNum < numKernings; ++kerningNum)
        {
            kerningLine = lines[currentlineIndex++];
            ParseKerningData( kerningLine );
        }
    }
//      PrintFontData();
//      PrintPageData();
//      PrintGlyphData();
//      PrintKerningData();

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Font::ParseFontData( std::string& infoLine, std::string& commonLine )
{
    // parse info
    infoLine.erase( 0, infoLine.find( "\"" ) + 1 );
    fontData.fontFace = infoLine.substr( 0, infoLine.find( "\"" ) );
    infoLine.erase( 0, infoLine.find( "size=" ) + 5 );
    fontData.size = stoi( infoLine.substr( 0, infoLine.find( " bold=" ) ) );
    infoLine.erase( 0, infoLine.find( "bold=" ) + 5 );
    fontData.isBold = stoi( infoLine.substr( 0, infoLine.find( " italic=" ) ) ) == 1 ? true : false;
    infoLine.erase( 0, infoLine.find( "italic=" ) + 7 );
    fontData.isItalic = stoi( infoLine.substr( 0, infoLine.find( " charset=\"" ) ) ) == 1 ? true : false;
    infoLine.erase( 0, infoLine.find( "charset=" ) + 9 );
    fontData.charset = infoLine.substr( 0, infoLine.find( "\"" ) );
    infoLine.erase( 0, infoLine.find( "unicode=" ) + 8 );
    fontData.isUnicode = stoi( infoLine.substr( 0, infoLine.find( " stretchH=" ) ) ) == 1 ? true : false;
    infoLine.erase( 0, infoLine.find( "stretchH=" ) + 9 );
    fontData.stretch = stoi( infoLine.substr( 0, infoLine.find( " smooth=" ) ) );
    infoLine.erase( 0, infoLine.find( "smooth=" ) + 7 );
    fontData.isSmoothed = stoi( infoLine.substr( 0, infoLine.find( " aa=" ) ) ) == 1 ? true : false;
    infoLine.erase( 0, infoLine.find( "aa=" ) + 3 );
    fontData.antiAliasLevels = stoi( infoLine.substr( 0, infoLine.find( " padding=" ) ) );
    infoLine.erase( 0, infoLine.find( "padding=" ) + 8 );
    fontData.padding.x = stoi( infoLine.substr( 0, infoLine.find( "," ) ) );
    infoLine.erase( 0, infoLine.find( "," ) + 1 );
    fontData.padding.y = stoi( infoLine.substr( 0, infoLine.find( "," ) ) );
    infoLine.erase( 0, infoLine.find( "," ) + 1 );
    fontData.padding.z = stoi( infoLine.substr( 0, infoLine.find( "," ) ) );
    infoLine.erase( 0, infoLine.find( "," ) + 1 );
    fontData.padding.w = stoi( infoLine.substr( 0, infoLine.find( " spacing=" ) ) );
    infoLine.erase( 0, infoLine.find( "spacing=" ) + 8 );
    fontData.spacing.x = stoi( infoLine.substr( 0, infoLine.find( "," ) ) );
    infoLine.erase( 0, infoLine.find( "," ) + 1 );
    fontData.spacing.y = stoi( infoLine.substr( 0, infoLine.find( " outline=" ) ) );
    infoLine.erase( 0, infoLine.find( "outline=" ) + 8 );
    fontData.outline = stoi( infoLine );

    // parse common
    commonLine.erase( 0, commonLine.find( "lineHeight=" ) + 11 );
    fontData.lineHeight = stoi( commonLine.substr( 0, commonLine.find( " base=" ) ) );
    commonLine.erase( 0, commonLine.find( "base=" ) + 5 );
    fontData.baseLine = stoi( commonLine.substr( 0, commonLine.find( " scaleW=" ) ) );
    commonLine.erase( 0, commonLine.find( "scaleW=" ) + 7 );
    fontData.scale.x = stoi( commonLine.substr( 0, commonLine.find( " scaleH=" ) ) );
    commonLine.erase( 0, commonLine.find( "scaleH=" ) + 7 );
    fontData.scale.y = stoi( commonLine.substr( 0, commonLine.find( " pages=" ) ) );
    commonLine.erase( 0, commonLine.find( "pages=" ) + 6 );
    fontData.numPages = stoi( commonLine.substr( 0, commonLine.find( " packed=" ) ) );
    commonLine.erase( 0, commonLine.find( "packed=" ) + 7 );
    fontData.isPacked = stoi( commonLine.substr( 0, commonLine.find( " alphaChnl=" ) ) ) == 1 ? true : false;
    commonLine.erase( 0, commonLine.find( "alphaChnl=" ) + 10 );
    fontData.alphaChannel = stoi( commonLine.substr( 0, commonLine.find( " redChnl=" ) ) );
    commonLine.erase( 0, commonLine.find( "redChnl=" ) + 8 );
    fontData.redChannel = stoi( commonLine.substr( 0, commonLine.find( " greenChnl=" ) ) );
    commonLine.erase( 0, commonLine.find( "greenChnl=" ) + 10 );
    fontData.greenChannel = stoi( commonLine.substr( 0, commonLine.find( " blueChnl=" ) ) );
    commonLine.erase( 0, commonLine.find( "blueChnl=" ) + 9 );
    fontData.blueChannel = stoi( commonLine );


    scale = fontData.scale;
    lineHeight = fontData.lineHeight;
    baseLine = fontData.baseLine;

    numPages = fontData.numPages;
    
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Font::ParsePageData( std::string& pageLine, const std::string& fontDirectory )
{
    PageData pd;

    pageLine.erase( 0, pageLine.find( "id=" ) + 3 );
    pd.pageID = stoi( pageLine.substr( 0, pageLine.find( " file=" ) ) );
    pageLine.erase( 0, pageLine.find( "file=\"" ) + 6 );
    pd.pageFile = fontDirectory + pageLine.substr( 0, pageLine.find( "\"" ) );

    pages.push_back( pd );

    if (pageTextures.find( pd.pageID ) == pageTextures.end())
        pageTextures[ pd.pageID ] = Texture::CreateOrGetTexture( pd.pageFile );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Font::ParseGlyphData( std::string& glyphLine )
{
    GlyphData gd;

    glyphLine.erase( 0, glyphLine.find( "id=" ) + 3 );
    gd.glyphID = stoi( glyphLine.substr( 0, glyphLine.find_first_of( " " ) ) );
    glyphLine.erase( 0, glyphLine.find( "x=" ) + 2 );
    gd.unmappedUV.x = stoi( glyphLine.substr( 0, glyphLine.find_first_of( " " ) ) );
    glyphLine.erase( 0, glyphLine.find( "y=" ) + 2 );
    gd.unmappedUV.y = stoi( glyphLine.substr( 0, glyphLine.find_first_of( " " ) ) );
    glyphLine.erase( 0, glyphLine.find( "width=" ) + 6 );
    gd.unmappedSize.x = stoi( glyphLine.substr( 0, glyphLine.find_first_of( " " ) ) );
    glyphLine.erase( 0, glyphLine.find( "height=" ) + 7 );
    gd.unmappedSize.y = stoi( glyphLine.substr( 0, glyphLine.find_first_of( " " ) ) );
    glyphLine.erase( 0, glyphLine.find( "xoffset=" ) + 8 );
    gd.offset.x = stoi( glyphLine.substr( 0, glyphLine.find_first_of( " " ) ) );
    glyphLine.erase( 0, glyphLine.find( "yoffset=" ) + 8 );
    gd.offset.y = stoi( glyphLine.substr( 0, glyphLine.find_first_of( " " ) ) );
    glyphLine.erase( 0, glyphLine.find( "xadvance=" ) + 9 );
    gd.advance = stoi( glyphLine.substr( 0, glyphLine.find_first_of( " " ) ) );
    glyphLine.erase( 0, glyphLine.find( "page=" ) + 5 );
    gd.pageID = stoi( glyphLine.substr( 0, glyphLine.find_first_of( " " ) ) );
    glyphLine.erase( 0, glyphLine.find( "chnl=" ) + 5 );
    gd.channel = stoi( glyphLine.substr( 0, glyphLine.find_first_of( " " ) ) );

    gd.mappedUVMins.x = ( (float) gd.unmappedUV.x ) / (float) scale.x;
    gd.mappedUVMins.y = ( (float) gd.unmappedUV.y ) / (float) scale.y;

    Vector2 mappedUVSize;
    mappedUVSize.x = (float) ( gd.unmappedSize.x ) / (float) scale.x;
    mappedUVSize.y = (float) ( gd.unmappedSize.y ) / (float) scale.y;

    gd.mappedUVMaxs = gd.mappedUVMins + mappedUVSize;


    glyphs[gd.glyphID] = gd;
    
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Font::ParseKerningData( std::string& kerningLine )
{
    KerningData kd;

    kerningLine.erase( 0, kerningLine.find( "first=" ) + 6 );

    kd.glyphs.x = stoi( kerningLine.substr( 0, kerningLine.find_first_of( " " ) ) );
    kerningLine.erase( 0, kerningLine.find( "second=" ) + 7 );
    kd.glyphs.y = stoi( kerningLine.substr( 0, kerningLine.find_first_of( " " ) ) );
    kerningLine.erase( 0, kerningLine.find( "amount=" ) + 7 );
    kd.amount = stoi( kerningLine );

    kernings[IntVector2( kd.glyphs.x, kd.glyphs.y )] = kd;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Font::PrintFontData()
{
    ConsolePrintf( "Font face: %s\n", fontData.fontFace.c_str() );
    ConsolePrintf( "Font size: %i\n", fontData.size );
    ConsolePrintf( "Font is bold: %s\n", fontData.isBold ? "true" : "false" );
    ConsolePrintf( "Font is italic: %s\n", fontData.isItalic ? "true" : "false" );
    ConsolePrintf( "Font charset: %s\n", fontData.charset.c_str() );
    ConsolePrintf( "Font is unicode: %s\n", fontData.isUnicode ? "true" : "false" );
    ConsolePrintf( "Font stretch: %i\n", fontData.stretch );
    ConsolePrintf( "Font is smoothed: %s\n", fontData.isSmoothed ? "true" : "false" );
    ConsolePrintf( "Font antialias levels: %i\n", fontData.antiAliasLevels );
    ConsolePrintf( "Font padding: x(%i), y(%i), z(%i), w(%i)\n", fontData.padding.x, fontData.padding.y, fontData.padding.z, fontData.padding.w );
    ConsolePrintf( "Font spacing: x(%i), y(%i)\n", fontData.spacing.x, fontData.spacing.y );
    ConsolePrintf( "Font outline thickness: %i\n", fontData.outline );
    ConsolePrintf( "Font line height: %i\n", fontData.lineHeight );
    ConsolePrintf( "Font base line: %i\n", fontData.baseLine );
    ConsolePrintf( "Font scale: x(%i), y(%i)\n", fontData.scale.x, fontData.scale.y );
    ConsolePrintf( "Font num pages: %i\n", fontData.numPages );
    ConsolePrintf( "Font is packed: %s\n", fontData.isPacked ? "true" : "false" );
    ConsolePrintf( "Font Alpha Channel: %i\n", fontData.alphaChannel );
    ConsolePrintf( "Font Red Channel: %i\n", fontData.redChannel );
    ConsolePrintf( "Font Green Channel: %i\n", fontData.greenChannel );
    ConsolePrintf( "Font Blue Channel: %i\n", fontData.blueChannel );
    ConsolePrintf( "Font Num Glyphs: %i\n", fontData.numGlyphs );
    ConsolePrintf( "Font Num Kernings: %i\n", fontData.numKernings );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Font::PrintPageData()
{
    for (Pages::iterator pageIter = pages.begin(); pageIter != pages.end(); ++pageIter)
    {
        PageData page = *pageIter;

        ConsolePrintf( "Page ID: %i         file: %s\n", page.pageID, page.pageFile.c_str() );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Font::PrintGlyphData()
{
    for (GlyphMap::iterator glyphIter = glyphs.begin(); glyphIter != glyphs.end(); ++glyphIter)
    {
        GlyphData glyph = glyphIter->second;

        ConsolePrintf( "Glyph ID: %i\n    Unmapped UV: x(%i), y(%i)  | Mapped UV Mins: x(%f), y(%f)\n", glyph.glyphID, glyph.unmappedUV.x, glyph.unmappedUV.y, glyph.mappedUVMins.x, glyph.mappedUVMins.y );
        ConsolePrintf( "    Unmapped Size: x(%i), y(%i)  |  Mapped UV Maxs: x(%f), y(%f)\n", glyph.unmappedSize.x, glyph.unmappedSize.y, glyph.mappedUVMaxs.x, glyph.mappedUVMaxs.y );
        ConsolePrintf( "    Offset: x(%i), y(%i)  |  Advance: %i  |  PageID: %i  |  Channel: %i\n", glyph.offset.x, glyph.offset.y, glyph.advance, glyph.pageID, glyph.channel );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void Font::PrintKerningData()
{
    for (KerningMap::iterator kerningIter = kernings.begin(); kerningIter != kernings.end(); ++kerningIter)
    {
        KerningData kerning = kerningIter->second;

        ConsolePrintf( "Kerning glyphID pair: first(%i), second(%i)  |  Amount: (%i)\n", kerning.glyphs.x, kerning.glyphs.y, kerning.amount );
    }
}