//=================================================================================
// TextRenderer.cpp
// Author: Tyler George
// Date  : June 10, 2015
//=================================================================================

#include <fstream>
#include <iostream>
#include "Engine/Renderer/TextRenderer.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
TextRenderer::TextRenderer( const std::string& fontInfoFilePath, const std::string& fontFile, OpenGLRenderer* renderer )
    : m_renderer( renderer )
    , m_font( NULL )
{
    if (!m_font)
        m_font = Texture::CreateOrGetTexture( fontFile );

    LoadFontInfo( fontInfoFilePath );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void TextRenderer::LoadFontInfo( const std::string& fontInfoFilePath )
{
    std::ifstream fontInfoFile( fontInfoFilePath );

    std::string line = "";

    while (fontInfoFile.good())
    {
        getline( fontInfoFile, line );
        ConsolePrintf( "%s", line.c_str() );
    }
}