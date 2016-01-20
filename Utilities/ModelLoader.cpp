//=================================================================================
// ModelLoader.cpp
// Author: Tyler George
// Date  : August 26, 2015
//=================================================================================


////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================

#include "Engine/Utilities/ModelLoader.hpp"


////===========================================================================================
///===========================================================================================
// Static Variable Initialization
///===========================================================================================
////===========================================================================================



////===========================================================================================
///===========================================================================================
// Functions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
/// 
///---------------------------------------------------------------------------------
bool LoadC23Model( const std::string& c23FilePath, PuttyMesh* mesh, Material& material )
{

    bool success = mesh->LoadMeshFromC23File( c23FilePath );
    if (!success)
        return success;

    std::string baseFilePath = "Data/Models/";
    size_t pos = c23FilePath.find_last_of( '/' );
    baseFilePath += c23FilePath.substr( pos );
    pos = baseFilePath.find_last_of( '.' );
    baseFilePath = baseFilePath.substr( 0, pos );

    std::string diffuseFilePath = baseFilePath + "_Diffuse.tga";
    std::string normalFilePath = baseFilePath + "_Normal.tga";
    std::string specGlossEmitFilePath = baseFilePath + "_SpecGlossEmit.tga";

    Texture* diffuseTex = Texture::CreateOrGetTexture( diffuseFilePath );
    if (!diffuseTex)
        diffuseTex = Texture::CreateOrGetTexture( "PlainWhite" );
    Texture* normalTex = Texture::CreateOrGetTexture( normalFilePath );
    if (!normalTex)
        normalTex = Texture::CreateOrGetTexture( "DefaultNormal" );
    Texture* specGlossEmitTex = Texture::CreateOrGetTexture( specGlossEmitFilePath );
    if (!specGlossEmitTex)
        specGlossEmitTex = Texture::CreateOrGetTexture( "DefaultSpecGlossEmit" );

    material.SetTextureUniform( "gTexDiffuse", diffuseTex );
    material.SetTextureUniform( "gTexNormal", normalTex );
    material.SetTextureUniform( "gTexSpecGlossEmit", specGlossEmitTex );

    return true;
}

