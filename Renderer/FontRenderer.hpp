//=================================================================================
// FontRenderer.hpp
// Author: Tyler George
// Date  : June 11, 2015
//=================================================================================

#pragma once

#ifndef __included_FontRenderer__
#define __included_FontRenderer__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Utilities/Font.hpp"

class OpenGLRenderer;


///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class FontRenderer
{
public:
	///---------------------------------------------------------------------------------
	/// Public Constructors/Destructors
	///---------------------------------------------------------------------------------
    FontRenderer( OpenGLRenderer* renderer );
    ~FontRenderer();
	
    ///---------------------------------------------------------------------------------
    /// Initialization
    ///---------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------
	/// Public Accessors/Queries
	///---------------------------------------------------------------------------------
    float CalcTextWidth( const unsigned int& fontHeight, const Font& font, const std::string& text, const unsigned int& endPos );
    static float CalcTextWidth( const unsigned int& fontHeight, const Font& font, const std::string& text );

	///---------------------------------------------------------------------------------
	/// Render
	///---------------------------------------------------------------------------------
    void DrawFontText( const unsigned int& fontHeight, const Font& font, const std::string& text, Vector3& in_out_Position, const Rgba& color = Rgba() );
    void DrawFontTextOrtho( const unsigned int& fontHeight, const Font& font, const std::string& text, Vector3& in_out_Position, const Rgba& color = Rgba() );
    void DrawFontTextWH( const float& fontWidth, const float& fontHeight, const Font& font, const std::string& text, Vector3& in_out_Position, const Rgba& color = Rgba() );
    void DrawFontGlyphWH( const float& fontWidth, const float& fontHeight, const Font& font, const char& character, Vector3& int_out_Position, const Rgba& color = Rgba() );


private:
	///---------------------------------------------------------------------------------
	/// Private Accessors/Queries
	///---------------------------------------------------------------------------------
    void GetGlyphVertexes( const int& previuousGlyphID, const unsigned int& fontSize, const float& scale, const Rgba& color, const GlyphData& glyph, const KerningMap& kernings, Vector3& cursorPos, PUC_Vertexes& out_glyphVertexes );
    void GetGlyphVertexes( const int& previuousGlyphID, const unsigned int& fontSize, const float& fonstScaleX, const float& fontScaleY, const Rgba& color, const GlyphData& glyph, const KerningMap& kernings, Vector3& cursorPos, PUC_Vertexes& out_glyphVertexes );
    void GetGlyphVertexes( const unsigned int& fontSize, const float& fontWidth, const float& fontScaleY, const Rgba& color, const GlyphData& glyph, Vector3& cursorPos, PUC_Vertexes& out_glyphVertexes );

	///---------------------------------------------------------------------------------
	/// Private Member Variables
	///---------------------------------------------------------------------------------
    OpenGLRenderer* m_renderer;
    unsigned int m_samplerID;
    unsigned int m_vaoID;


};

#endif