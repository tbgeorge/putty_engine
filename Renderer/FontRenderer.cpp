//=================================================================================
// FontRenderer.cpp
// Author: Tyler George
// Date  : June 11, 2015
//=================================================================================

//===========================================================================================
//===========================================================================================
// Includes
//===========================================================================================
//===========================================================================================
#include "Engine/Renderer/FontRenderer.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"

//===========================================================================================
//===========================================================================================
// Public Constructors/Destructors
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
/// 
///---------------------------------------------------------------------------------
FontRenderer::FontRenderer( OpenGLRenderer* renderer )
    : m_renderer( renderer )
{
    m_samplerID = m_renderer->GetDefaultSampler();
//     m_vaoID = m_renderer->CreateVAO();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
FontRenderer::~FontRenderer()
{
//     m_renderer->DestroyVAO( m_vaoID );
}

//===========================================================================================
//===========================================================================================
// Public Accessors/Queries
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float FontRenderer::CalcTextWidth( const unsigned int& fontHeight, const Font& font, const std::string& text, const unsigned int& endPos )
{
    float width = 0.0f;

    unsigned int fontSize = font.fontData.size;
    float fontScale = (float) fontHeight / (float) fontSize;

    const GlyphMap glyphs = font.glyphs;
    const KerningMap kernings = font.kernings;

    int lastGlyphID = 32;
    for (std::string::const_iterator textIter = text.begin(); textIter != text.begin() + endPos; ++textIter)
    {
        const char character = *textIter;
        int asciiVal = (int)character;

        const GlyphData glyph = glyphs.at( asciiVal );
        width += fontScale * (float)glyph.advance;

        const KerningData* kerning = nullptr;

        if (kernings.find( IntVector2( lastGlyphID, glyph.glyphID ) ) != kernings.end())
        {
            kerning = &kernings.at( IntVector2( lastGlyphID, glyph.glyphID ) );
        }

        if (kerning != nullptr)
            width += fontScale * kerning->amount;

        lastGlyphID = glyph.glyphID;
    }

    return width;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float FontRenderer::CalcTextWidth( const unsigned int& fontHeight, const Font& font, const std::string& text )
{
    float width = 0.0f;

    unsigned int fontSize = font.fontData.size;
    float fontScale = (float)fontHeight / (float)fontSize;

    const GlyphMap glyphs = font.glyphs;
    const KerningMap kernings = font.kernings;

    int lastGlyphID = 32;
    for (std::string::const_iterator textIter = text.begin(); textIter != text.end(); ++textIter)
    {
        const char character = *textIter;
        int asciiVal = (int)character;

        const GlyphData glyph = glyphs.at( asciiVal );
        width += fontScale * (float)glyph.advance;

        const KerningData* kerning = nullptr;

        if (kernings.find( IntVector2( lastGlyphID, glyph.glyphID ) ) != kernings.end())
        {
            kerning = &kernings.at( IntVector2( lastGlyphID, glyph.glyphID ) );
        }

        if (kerning != nullptr)
            width += fontScale * kerning->amount;

        lastGlyphID = glyph.glyphID;
    }

    return width;
}

//===========================================================================================
//===========================================================================================
// Private Accessors/Queries
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
/// Maintains ratio
///---------------------------------------------------------------------------------
void FontRenderer::GetGlyphVertexes( const int& previuousGlyphID, const unsigned int& fontSize, const float& scale, const Rgba& color, const GlyphData& glyph, const KerningMap& kernings, Vector3& cursorPos, PUC_Vertexes& out_glyphVertexes )
{
    const Vector2& uvMins = glyph.mappedUVMins;
    const Vector2& uvMaxs = glyph.mappedUVMaxs;

    const IntVector2& offset = glyph.offset;
    const IntVector2& size = glyph.unmappedSize;

    float advance = (float)glyph.advance;

    Vector2 maxs( cursorPos.x + scale * (float)(offset.x + size.x), cursorPos.y + scale * (float)(fontSize - offset.y) );
    Vector2 mins( cursorPos.x + scale * (float)offset.x, cursorPos.y + scale * (float)(fontSize - offset.y - size.y) );

    const KerningData* kerning = nullptr;

    if (kernings.find( IntVector2( previuousGlyphID, glyph.glyphID ) ) != kernings.end())
    {
        kerning = &kernings.at( IntVector2( previuousGlyphID, glyph.glyphID ) );
    }

    if (kerning != nullptr)
    {
        mins.x += kerning->amount;
        maxs.x += kerning->amount;
    }

    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, cursorPos.z ), Vector2( uvMins.x, uvMaxs.y ), color ) );
    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, cursorPos.z ), uvMaxs, color ) );
    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, cursorPos.z ), Vector2( uvMaxs.x, uvMins.y ), color ) );

    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, cursorPos.z ), Vector2( uvMaxs.x, uvMins.y ), color ) );
    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, cursorPos.z ), uvMins, color ) );
    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, cursorPos.z ), Vector2( uvMins.x, uvMaxs.y ), color ) );

    cursorPos.x += scale * (float)advance;
}

///---------------------------------------------------------------------------------
/// Ratio passed in
///---------------------------------------------------------------------------------
void FontRenderer::GetGlyphVertexes( const int& previuousGlyphID, const unsigned int& fontSize, const float& fontWidth, const float& fontScaleY, const Rgba& color, const GlyphData& glyph, const KerningMap& kernings, Vector3& cursorPos, PUC_Vertexes& out_glyphVertexes )
{
    const Vector2& uvMins = glyph.mappedUVMins;
    const Vector2& uvMaxs = glyph.mappedUVMaxs;

    const IntVector2& offset = glyph.offset;
    const IntVector2& size = glyph.unmappedSize;

    float advance = (float)glyph.advance;

    float fontScaleX = fontWidth / advance;

    Vector2 maxs( cursorPos.x + fontScaleX * (float)(offset.x + size.x), cursorPos.y + fontScaleY * (float)(fontSize - offset.y) );
    Vector2 mins( cursorPos.x + fontScaleX * (float)offset.x, cursorPos.y + fontScaleY * (float)(fontSize - offset.y - size.y) );

    const KerningData* kerning = nullptr;

    if (kernings.find( IntVector2( previuousGlyphID, glyph.glyphID ) ) != kernings.end())
    {
        kerning = &kernings.at( IntVector2( previuousGlyphID, glyph.glyphID ) );
    }

    if (kerning != nullptr)
    {
        mins.x += kerning->amount;
        maxs.x += kerning->amount;
    }

    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, cursorPos.z ), Vector2( uvMins.x, uvMaxs.y ), color ) );
    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, cursorPos.z ), uvMaxs, color ) );
    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, cursorPos.z ), Vector2( uvMaxs.x, uvMins.y ), color ) );

    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, cursorPos.z ), Vector2( uvMaxs.x, uvMins.y ), color ) );
    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, cursorPos.z ), uvMins, color ) );
    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, cursorPos.z ), Vector2( uvMins.x, uvMaxs.y ), color ) );

    cursorPos.x += fontScaleX * (float)advance;
}

///---------------------------------------------------------------------------------
/// Ratio passed in - ignore kerning, optimize for speed
///---------------------------------------------------------------------------------
void FontRenderer::GetGlyphVertexes( const unsigned int& fontSize, const float& fontWidth, const float& fontScaleY, const Rgba& color, const GlyphData& glyph, Vector3& cursorPos, PUC_Vertexes& out_glyphVertexes )
{
    const Vector2& uvMins = glyph.mappedUVMins;
    const Vector2& uvMaxs = glyph.mappedUVMaxs;

    const IntVector2& offset = glyph.offset;
    const IntVector2& size = glyph.unmappedSize;

    float advance = (float)glyph.advance;

    float fontScaleX = fontWidth / advance;

    Vector2 maxs( cursorPos.x + fontScaleX * (float)(offset.x + size.x), cursorPos.y + fontScaleY * (float)(fontSize - offset.y) );
    Vector2 mins( cursorPos.x + fontScaleX * (float)offset.x, cursorPos.y + fontScaleY * (float)(fontSize - offset.y - size.y) );


    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, cursorPos.z ), Vector2( uvMins.x, uvMaxs.y ), color ) );
    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, cursorPos.z ), uvMaxs, color ) );
    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, cursorPos.z ), Vector2( uvMaxs.x, uvMins.y ), color ) );

    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, cursorPos.z ), Vector2( uvMaxs.x, uvMins.y ), color ) );
    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, cursorPos.z ), uvMins, color ) );
    out_glyphVertexes.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, cursorPos.z ), Vector2( uvMins.x, uvMaxs.y ), color ) );

    cursorPos.x += fontScaleX * (float)advance;
}

//===========================================================================================
//===========================================================================================
// Render
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
/// 
///---------------------------------------------------------------------------------
void FontRenderer::DrawFontText( const unsigned int& fontHeight, const Font& font, const std::string& text, Vector3& int_out_Position, const Rgba& color /* = Rgba() */ )
{
    unsigned int programID = OpenGLRenderer::CreateOrGetShader( "Data/Shaders/textured" );

    const FontData& fontData = font.fontData;
    unsigned int fontSize = fontData.size;
    float fontScale = (float)fontHeight / (float)fontSize;

    const PageFiles& pageFiles = font.pageTextures;
    const GlyphMap& glyphs = font.glyphs;
    const KerningMap& kernings = font.kernings;

    std::map< int, PUC_Vertexes > pageVertexes;


    int lastGlyphID = 32;
    for (std::string::const_iterator textIter = text.begin(); textIter != text.end(); ++textIter)
    {
        const char character = *textIter;
        int asciiVal = (int)character;

        const GlyphData glyph = glyphs.at( asciiVal );
        unsigned int pageID = glyph.pageID;

        GetGlyphVertexes( lastGlyphID, fontSize, fontScale, color, glyph, kernings, int_out_Position, pageVertexes[pageID] );
        lastGlyphID = glyph.glyphID;
    }

    for (std::map< int, PUC_Vertexes >::iterator pgVertsIter = pageVertexes.begin(); pgVertsIter != pageVertexes.end(); ++pgVertsIter)
    {
        PUC_Vertexes vertexes = pgVertsIter->second;
        VertexLayout layout = Vertex3D_PUC::GetVertexLayout( vertexes );

        m_renderer->DrawWithTexture( programID, layout, vertexes.size(), m_renderer->GetOrthoMatrix(), Matrix4f::CreateIdentity(), m_samplerID, pageFiles.at( pgVertsIter->first ) );
    }

}

///---------------------------------------------------------------------------------
/// 
///---------------------------------------------------------------------------------
void FontRenderer::DrawFontTextOrtho( const unsigned int& fontHeight, const Font& font, const std::string& text, Vector3& int_out_Position, const Rgba& color /* = Rgba() */ )
{
    m_renderer->Enable( GL_BLEND );
    m_renderer->Enable( GL_TEXTURE_2D );
    m_renderer->BlendFunct( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    unsigned int programID = OpenGLRenderer::CreateOrGetShader( "Data/Shaders/textured" );
    const FontData& fontData = font.fontData;
    unsigned int fontSize = fontData.size;
    float fontScale = (float)fontHeight / (float)fontSize;

    const PageFiles& pageFiles = font.pageTextures;
    const GlyphMap& glyphs = font.glyphs;
    const KerningMap& kernings = font.kernings;

    std::map< int, PUC_Vertexes > pageVertexes;


    int lastGlyphID = 32;
    for (std::string::const_iterator textIter = text.begin(); textIter != text.end(); ++textIter)
    {
        const char character = *textIter;
        int asciiVal = (int)character;

        const GlyphData glyph = glyphs.at( asciiVal );
        unsigned int pageID = glyph.pageID;

        GetGlyphVertexes( lastGlyphID, fontSize, fontScale, color, glyph, kernings, int_out_Position, pageVertexes[pageID] );
        lastGlyphID = glyph.glyphID;
    }

    for (std::map< int, PUC_Vertexes >::iterator pgVertsIter = pageVertexes.begin(); pgVertsIter != pageVertexes.end(); ++pgVertsIter)
    {
        PUC_Vertexes vertexes = pgVertsIter->second;
        VertexLayout layout = Vertex3D_PUC::GetVertexLayout( vertexes );

        m_renderer->DrawWithTextureOrtho( programID, layout, vertexes.size(), m_samplerID, pageFiles.at( pgVertsIter->first ) );
    }

    m_renderer->Disable( GL_BLEND );
    m_renderer->Disable( GL_TEXTURE_2D );
}

///---------------------------------------------------------------------------------
/// 
///---------------------------------------------------------------------------------
void FontRenderer::DrawFontTextWH( const float& fontWidth, const float& fontHeight, const Font& font, const std::string& text, Vector3& int_out_Position, const Rgba& color /* = Rgba() */ )
{
    unsigned int programID = OpenGLRenderer::CreateOrGetShader( "Data/Shaders/textured" );

    const FontData& fontData = font.fontData;
    unsigned int fontSize = fontData.size;
    float fontScaleY = fontHeight / (float)fontSize;

    const PageFiles& pageFiles = font.pageTextures;
    const GlyphMap& glyphs = font.glyphs;
    const KerningMap& kernings = font.kernings;

    std::map< int, PUC_Vertexes > pageVertexes;


    int lastGlyphID = 32;
    for (std::string::const_iterator textIter = text.begin(); textIter != text.end(); ++textIter)
    {
        const char character = *textIter;
        int asciiVal = (int)character;

        const GlyphData glyph = glyphs.at( asciiVal );
        unsigned int pageID = glyph.pageID;

        GetGlyphVertexes( lastGlyphID, fontSize, fontWidth, fontScaleY, color, glyph, kernings, int_out_Position, pageVertexes[pageID] );
        lastGlyphID = glyph.glyphID;
    }

    for (std::map< int, PUC_Vertexes >::iterator pgVertsIter = pageVertexes.begin(); pgVertsIter != pageVertexes.end(); ++pgVertsIter)
    {
        PUC_Vertexes vertexes = pgVertsIter->second;
        VertexLayout layout = Vertex3D_PUC::GetVertexLayout( vertexes );

        m_renderer->DrawWithTexture( programID, layout, vertexes.size(), m_renderer->GetOrthoMatrix(), Matrix4f::CreateIdentity(), m_samplerID, pageFiles.at( pgVertsIter->first ) );
    }

}

///---------------------------------------------------------------------------------
/// Optimized for speed
///---------------------------------------------------------------------------------
void FontRenderer::DrawFontGlyphWH( const float& fontWidth, const float& fontHeight, const Font& font, const char& character, Vector3& int_out_Position, const Rgba& color /* = Rgba() */ )
{
    unsigned int programID = OpenGLRenderer::CreateOrGetShader( "Data/Shaders/textured" );

    const FontData& fontData = font.fontData;
    float fontScaleY = fontHeight / (float)fontData.size;

    const PageFiles& pageFiles = font.pageTextures;
    const GlyphMap& glyphs = font.glyphs;

    PUC_Vertexes verts;
    unsigned int pageID = glyphs.at( (int)character ).pageID;

    GetGlyphVertexes( fontData.size, fontWidth, fontScaleY, color, glyphs.at( (int)character ), int_out_Position, verts );    

    VertexLayout layout = Vertex3D_PUC::GetVertexLayout( verts );
    m_renderer->DrawWithTexture( programID, layout, verts.size(), m_renderer->GetOrthoMatrix(), Matrix4f::CreateIdentity(), m_samplerID, pageFiles.at( pageID ) );
}