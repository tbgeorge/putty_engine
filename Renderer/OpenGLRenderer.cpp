//=================================================================================
// OpenGLRenderer.cpp
// Author: Tyler George
// Date  : January 15, 2015
//=================================================================================

#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Renderer/geometry.h"
#include "Engine/Utilities/Time.hpp"
#include "Engine/Utilities/Console.hpp"
#include "Engine/Math/IcoSphereCreator.hpp"
#include "Engine/Utilities/FileUtilities.hpp"
#include "Engine/Utilities/DeveloperConsole.hpp"
#include "Engine/Renderer/GraphicsDebugger.hpp"


// Globals "function pointer" variables
PFNGLGENBUFFERSPROC		            glGenBuffers		        = nullptr;
PFNGLDELETEBUFFERSPROC	            glDeleteBuffers		        = nullptr;
PFNGLBINDBUFFERPROC		            glBindBuffer		        = nullptr;
PFNGLBUFFERDATAPROC		            glBufferData		        = nullptr;
PFNGLBINDBUFFERBASEPROC             glBindBufferBase            = nullptr;
PFNGLGENERATEMIPMAPPROC	            glGenerateMipmap	        = nullptr;
PFNGLCREATESHADERPROC               glCreateShader              = nullptr;
PFNGLCOMPILESHADERPROC              glCompileShader             = nullptr;
PFNGLSHADERSOURCEPROC               glShaderSource              = nullptr;
PFNGLDELETESHADERPROC               glDeleteShader              = nullptr;
PFNGLGETSHADERIVPROC                glGetShaderiv               = nullptr;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog          = nullptr;
PFNGLCREATEPROGRAMPROC              glCreateProgram             = nullptr;
PFNGLATTACHSHADERPROC               glAttachShader              = nullptr;
PFNGLDETACHSHADERPROC               glDetachShader              = nullptr;
PFNGLLINKPROGRAMPROC                glLinkProgram               = nullptr;
PFNGLDELETEPROGRAMPROC              glDeleteProgram             = nullptr;
PFNGLPROGRAMPARAMETERIPROC          glProgramParameteri         = nullptr;
PFNGLGETPROGRAMIVPROC               glGetProgramiv              = nullptr;
PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog         = nullptr;
PFNGLUSEPROGRAMPROC                 glUseProgram                = nullptr;
PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation        = nullptr;
PFNGLGETUNIFORMINDICESPROC          glGetUniformIndices         = nullptr;
PFNGLGETUNIFORMBLOCKINDEXPROC       glGetUniformBlockIndex      = nullptr;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC    glGetActiveUniformBlockiv   = nullptr;
PFNGLGETACTIVEUNIFORMSIVPROC         glGetActiveUniformsiv      = nullptr;

PFNGLGETACTIVEUNIFORMPROC           glGetActiveUniform          = nullptr;
PFNGLBINDATTRIBLOCATIONPROC         glBindAttribLocation        = nullptr;
PFNGLUNIFORM1FVPROC                 glUniform1fv                = nullptr;
PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv          = nullptr;
PFNGLUNIFORM2FVPROC                 glUniform2fv                = nullptr;
PFNGLUNIFORM3FVPROC                 glUniform3fv                = nullptr;
PFNGLUNIFORM4FVPROC                 glUniform4fv                = nullptr;
PFNGLUNIFORM1IVPROC                 glUniform1iv                = nullptr;
PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays           = nullptr;
PFNGLDELETEVERTEXARRAYSPROC         glDeleteVertexArrays        = nullptr;
PFNGLBINDVERTEXARRAYPROC            glBindVertexArray           = nullptr;
PFNGLGETATTRIBLOCATIONPROC          glGetAttribLocation         = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray   = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer       = nullptr;
PFNGLGETPROGRAMBINARYPROC           glGetProgramBinary          = nullptr;
PFNGLPROGRAMBINARYPROC              glProgramBinary             = nullptr;
PFNGLGENSAMPLERSPROC                glGenSamplers               = nullptr;
PFNGLSAMPLERPARAMETERIPROC          glSamplerParameteri         = nullptr;
PFNGLACTIVETEXTUREPROC              glActiveTexture             = nullptr;
PFNGLBINDSAMPLERPROC                glBindSampler               = nullptr;
PFNGLDELETESAMPLERSPROC             glDeleteSamplers            = nullptr;

PFNGLBINDFRAMEBUFFERPROC            glBindFramebuffer           = nullptr;
PFNGLBLITFRAMEBUFFERPROC            glBlitFramebuffer           = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC     glCheckFramebufferStatus    = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC         glDeleteFramebuffers        = nullptr;
PFNGLFRAMEBUFFERTEXTUREPROC         glFramebufferTexture        = nullptr;
PFNGLGENFRAMEBUFFERSPROC            glGenFramebuffers           = nullptr;

PFNGLGENRENDERBUFFERSPROC           glGenRenderbuffers          = nullptr;
PFNGLBINDRENDERBUFFERPROC           glBindRenderbuffer          = nullptr;
PFNGLRENDERBUFFERSTORAGEPROC        glRenderbufferStorage       = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC       glFramebufferTexture2D      = nullptr;
PFNGLFRAMEBUFFERRENDERBUFFERPROC    glFramebufferRenderbuffer   = nullptr;

//DeveloperConsole* OpenGLRenderer::m_devConsole = nullptr;

////===========================================================================================
///===========================================================================================
// Static Variable Initialization
///===========================================================================================
////===========================================================================================

std::map< std::string, unsigned int > OpenGLRenderer::s_shaderMap;

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
OpenGLRenderer::OpenGLRenderer()
	: m_windowHandle( nullptr )
	, m_deviceContext( nullptr )
	, m_renderingContext( nullptr )
	, m_displayWidth( 0.0f )
	, m_displayHeight( 0.0f )
    , m_fontRenderer( nullptr )
    , m_graphicsDebugger( nullptr )
    , m_defaultShaderID( 0 )
    , m_currentlyBoundShaderID( 0 )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
OpenGLRenderer::~OpenGLRenderer()
{
    delete m_fontRenderer;
    delete m_graphicsDebugger;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void* OpenGLRenderer::ReadPixels()
{
    void* pixelData = malloc( 4 * (size_t)floorf(m_displayWidth) * (size_t)floorf(m_displayHeight) );
    glReadPixels( 0, 0, (size_t)floorf(m_displayWidth), (size_t)floorf(m_displayHeight), GL_RGBA, GL_UNSIGNED_BYTE, pixelData );
    return pixelData;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int OpenGLRenderer::CreateOrGetShader( const std::string& shaderName )
{
    std::map< std::string, unsigned int >::const_iterator shaderIter = s_shaderMap.find( shaderName );

    unsigned int shaderID = 0;

    if (shaderIter != s_shaderMap.end())
        shaderID = shaderIter->second;
    
    else
    {
        shaderID = LoadProgram( (shaderName + ".vert").c_str(), (shaderName + ".frag").c_str() );
        s_shaderMap.insert( std::pair< std::string, unsigned int >( shaderName, shaderID ) );
    }

    return shaderID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::Startup( HWND windowHandle ) 
{
	m_windowHandle	= windowHandle;
	m_deviceContext = GetDC( windowHandle );

	CreateOpenGLRenderingContext();
	InitializeAdvancedOpenGLFunctions();
    InitializeDefaultTextures();

    m_defaultShaderID = OpenGLRenderer::CreateOrGetShader( "Data/Shaders/basic" ); 
    m_fontRenderer = new FontRenderer( this );
    m_graphicsDebugger = new GraphicsDebugger( this );
    GLCheckError();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
// void OpenGLRenderer::SetDevConsole( DeveloperConsole* devConsole )
// {
//     m_devConsole = devConsole;
// }

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::Shutdown()
{
	DestroyOpenGLRenderingContext();
    Texture::Shutdown();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::InitializeAdvancedOpenGLFunctions()
{
	glGenBuffers		        = (PFNGLGENBUFFERSPROC)		            wglGetProcAddress( "glGenBuffers" );
	glDeleteBuffers		        = (PFNGLDELETEBUFFERSPROC)	            wglGetProcAddress( "glDeleteBuffers" );
	glBindBuffer		        = (PFNGLBINDBUFFERPROC)		            wglGetProcAddress( "glBindBuffer" );
	glBufferData		        = (PFNGLBUFFERDATAPROC)		            wglGetProcAddress( "glBufferData" );
    glBindBufferBase            = (PFNGLBINDBUFFERBASEPROC)             wglGetProcAddress( "glBindBufferBase" );
	glGenerateMipmap	        = (PFNGLGENERATEMIPMAPPROC)	            wglGetProcAddress( "glGenerateMipmap" );
    glCreateShader              = (PFNGLCREATESHADERPROC)               wglGetProcAddress( "glCreateShader" ); 
    glCompileShader             = (PFNGLCOMPILESHADERPROC)              wglGetProcAddress( "glCompileShader" );
    glShaderSource              = (PFNGLSHADERSOURCEPROC)               wglGetProcAddress( "glShaderSource" );
    glDeleteShader              = (PFNGLDELETESHADERPROC)               wglGetProcAddress( "glDeleteShader" );
    glGetShaderiv               = (PFNGLGETSHADERIVPROC)                wglGetProcAddress( "glGetShaderiv" );
    glGetShaderInfoLog          = (PFNGLGETSHADERINFOLOGPROC)           wglGetProcAddress( "glGetShaderInfoLog" );
    glCreateProgram             = (PFNGLCREATEPROGRAMPROC)              wglGetProcAddress( "glCreateProgram" );
    glAttachShader              = (PFNGLATTACHSHADERPROC)               wglGetProcAddress( "glAttachShader" );
    glDetachShader              = (PFNGLDETACHSHADERPROC)               wglGetProcAddress( "glDetachShader" );
    glLinkProgram               = (PFNGLLINKPROGRAMPROC)                wglGetProcAddress( "glLinkProgram" );
    glDeleteProgram             = (PFNGLDELETEPROGRAMPROC)              wglGetProcAddress( "glDeleteProgram" );
    glProgramParameteri         = (PFNGLPROGRAMPARAMETERIPROC)          wglGetProcAddress( "glProgramParameteri" );
    glGetProgramiv              = (PFNGLGETPROGRAMIVPROC)               wglGetProcAddress( "glGetProgramiv" );
    glGetProgramInfoLog         = (PFNGLGETPROGRAMINFOLOGPROC)          wglGetProcAddress( "glGetProgramInfoLog" );
    glUseProgram                = (PFNGLUSEPROGRAMPROC)                 wglGetProcAddress( "glUseProgram" );
    glGetUniformLocation        = (PFNGLGETUNIFORMLOCATIONPROC)         wglGetProcAddress( "glGetUniformLocation" );
    glGetUniformIndices         = (PFNGLGETUNIFORMINDICESPROC)          wglGetProcAddress( "glGetUniformIndices" );
    glGetUniformBlockIndex      = (PFNGLGETUNIFORMBLOCKINDEXPROC)       wglGetProcAddress( "glGetUniformBlockIndex" );
    glGetActiveUniformBlockiv   = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)    wglGetProcAddress( "glGetActiveUniformBlockiv" );
    glGetActiveUniformsiv       = (PFNGLGETACTIVEUNIFORMSIVPROC)        wglGetProcAddress( "glGetActiveUniformsiv" );

    glGetActiveUniform          = (PFNGLGETACTIVEUNIFORMPROC)           wglGetProcAddress( "glGetActiveUniform" );
    glBindAttribLocation        = (PFNGLBINDATTRIBLOCATIONPROC)         wglGetProcAddress( "glBindAttribLocation" );
    glUniform1fv                = (PFNGLUNIFORM1FVPROC)                 wglGetProcAddress( "glUniform1fv" );
    glUniformMatrix4fv          = (PFNGLUNIFORMMATRIX4FVPROC)           wglGetProcAddress( "glUniformMatrix4fv" );
    glUniform2fv                = (PFNGLUNIFORM2FVPROC)                 wglGetProcAddress( "glUniform2fv" );
    glUniform3fv                = (PFNGLUNIFORM3FVPROC)                 wglGetProcAddress( "glUniform3fv" );
    glUniform4fv                = (PFNGLUNIFORM4FVPROC)                 wglGetProcAddress( "glUniform4fv" );
    glUniform1iv                = (PFNGLUNIFORM1IVPROC)                 wglGetProcAddress( "glUniform1iv" );
    glGenVertexArrays           = (PFNGLGENVERTEXARRAYSPROC)            wglGetProcAddress( "glGenVertexArrays" );
    glDeleteVertexArrays        = (PFNGLDELETEVERTEXARRAYSPROC)         wglGetProcAddress( "glDeleteVertexArrays" );
    glBindVertexArray           = (PFNGLBINDVERTEXARRAYPROC)            wglGetProcAddress( "glBindVertexArray" );
    glGetAttribLocation         = (PFNGLGETATTRIBLOCATIONPROC)          wglGetProcAddress( "glGetAttribLocation" );
    glEnableVertexAttribArray   = (PFNGLENABLEVERTEXATTRIBARRAYPROC)    wglGetProcAddress( "glEnableVertexAttribArray" );
    glVertexAttribPointer       = (PFNGLVERTEXATTRIBPOINTERPROC)        wglGetProcAddress( "glVertexAttribPointer" );
    glGetProgramBinary          = (PFNGLGETPROGRAMBINARYPROC)           wglGetProcAddress( "glGetProgramBinary" );
    glProgramBinary             = (PFNGLPROGRAMBINARYPROC)              wglGetProcAddress( "glProgramBinary" );
    glGenSamplers               = (PFNGLGENSAMPLERSPROC)                wglGetProcAddress( "glGenSamplers" );
    glSamplerParameteri         = (PFNGLSAMPLERPARAMETERIPROC)          wglGetProcAddress( "glSamplerParameteri" );
    glActiveTexture             = (PFNGLACTIVETEXTUREPROC)              wglGetProcAddress( "glActiveTexture" );
    glBindSampler               = (PFNGLBINDSAMPLERPROC)                wglGetProcAddress( "glBindSampler" );
    glDeleteSamplers            = (PFNGLDELETESAMPLERSPROC)             wglGetProcAddress( "glDeleteSamplers" );

    glBindFramebuffer           = (PFNGLBINDFRAMEBUFFERPROC)            wglGetProcAddress( "glBindFramebuffer" );
    glBlitFramebuffer           = (PFNGLBLITFRAMEBUFFERPROC)            wglGetProcAddress( "glBlitFramebuffer" );
    glCheckFramebufferStatus    = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)     wglGetProcAddress( "glCheckFramebufferStatus" );
    glDeleteFramebuffers        = (PFNGLDELETEFRAMEBUFFERSPROC)         wglGetProcAddress( "glDeleteFramebuffers" );
    glFramebufferTexture        = (PFNGLFRAMEBUFFERTEXTUREPROC)         wglGetProcAddress( "glFramebufferTexture" );
    glGenFramebuffers           = (PFNGLGENFRAMEBUFFERSPROC)            wglGetProcAddress( "glGenFramebuffers" );
    glGenRenderbuffers          = (PFNGLGENRENDERBUFFERSPROC)           wglGetProcAddress( "glGenRenderbuffers" );
    glBindRenderbuffer          = (PFNGLBINDRENDERBUFFERPROC)           wglGetProcAddress( "glBindRenderbuffer" );
    glRenderbufferStorage       = (PFNGLRENDERBUFFERSTORAGEPROC)        wglGetProcAddress( "glRenderbufferStorage" );
    glFramebufferTexture2D      = (PFNGLFRAMEBUFFERTEXTURE2DPROC)       wglGetProcAddress( "glFramebufferTexture2D" );
    glFramebufferRenderbuffer   = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)    wglGetProcAddress( "glFramebufferRenderbuffer" );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::InitializeDefaultTextures()
{
    unsigned char plainWhiteTexel[3] = { 255, 255, 255 };
    Texture::CreateOrGetTexture( "PlainWhite", plainWhiteTexel, IntVector2( 1, 1 ), 3 );

    unsigned char lightBlueTexel[3] = { 127, 127, 255 };
    Texture::CreateOrGetTexture( "DefaultNormal", lightBlueTexel, IntVector2( 1, 1 ), 3 );

    unsigned char darkYellowTexel[3] = { 1, 1, 0 };
    Texture::CreateOrGetTexture( "DefaultSpecGlossEmit", darkYellowTexel, IntVector2( 1, 1 ), 3 );

    unsigned char blackMagentaCheckerboardTexels[4 * 4 * 3] = 
    {
        255, 0, 255,    0,   0, 0,     255, 0, 0,     0,   0, 0,
        0,   0, 0,      255, 0, 255,   0,   0, 0,     255, 0, 255,
        255, 0, 255,    0,   0, 0,     255, 0, 0,     0,   0, 0,
        0, 0, 0,        255, 0, 255,   0, 0, 0,       255, 0, 255,
    };
    Texture::CreateOrGetTexture( "MissingTexture", blackMagentaCheckerboardTexels, IntVector2( 4, 4 ), 3 );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int OpenGLRenderer::GetDefaultSampler()
{
    static unsigned int defaultSamplerID = CreateSampler( GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT );
    return defaultSamplerID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::BindRenderState( RenderState rs )
{
    if (rs.culling)
        Enable( GL_CULL_FACE );
    else
        Disable( GL_CULL_FACE );

    if (rs.depthTest)
        Enable( GL_DEPTH_TEST );
    else
        Disable( GL_DEPTH_TEST );

    if (rs.blend)
    {
        Enable( GL_BLEND );
        BlendFunct( rs.blendSrc, rs.blendDst );
    }
    else
        Disable( GL_BLEND );

    if (rs.filling)
        glPolygonMode( rs.fillFace, rs.fillMode );

  
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
GLuint OpenGLRenderer::CreateVAO()
{
    unsigned int vaoID;
    glGenVertexArrays( 1, &vaoID );
    GLCheckError();

    if (vaoID == NULL) {
        MessageBoxAf( MBT_OK, MBI_ERROR, "VAO ERROR", "Could not create VAO." );
        __debugbreak();
        return NULL;
    }

    return vaoID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::BindVAOBuffer( const GLuint& programID, const GLuint& vaoID, const GLuint& bufferID, const VertexLayout& layout )
{
    glBindVertexArray( vaoID );
    glBindBuffer( GL_ARRAY_BUFFER, bufferID );

    for (VertexLayout::const_iterator layoutIter = layout.begin(); layoutIter != layout.end(); ++layoutIter)
    {
        const VertexAttributeProperties& properites = *layoutIter;

        ProgramBindAttribute( programID, properites.name, properites.count, properites.type, properites.normalize, properites.stride, properites.offset );
    }

    GLCheckError();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::CheckCompatability()
{
    // Versions 
    char* gl_version = (char*) glGetString( GL_VERSION );
    ConsolePrintf( "GL_VERSION: %s \n", gl_version );
    DeveloperConsole::WriteLine( "GL_VERSION: " + std::string( gl_version ), INFO_TEXT_COLOR );

    GLint major;
    GLint minor;
    glGetIntegerv( GL_MAJOR_VERSION, &major );
    glGetIntegerv( GL_MINOR_VERSION, &minor );

    ConsolePrintf( "GL_VERSION (int): %i.%i \n", major, minor );
    DeveloperConsole::WriteLine( "GL_VERSION (int): " + std::to_string( major ) + "." + std::to_string( minor ), INFO_TEXT_COLOR );

    char* shader_version = (char*) glGetString( GL_SHADING_LANGUAGE_VERSION );
    ConsolePrintf( "GL_SHADING_LANGUAGE_VERSION: %s \n", shader_version );
    DeveloperConsole::WriteLine( "GL_SHADING_LANGUAGE_VERSION: " + std::string( shader_version ), INFO_TEXT_COLOR );
    GLCheckError();

    GLboolean shader_compiler_available = GL_FALSE;
    glGetBooleanv( GL_SHADER_COMPILER, &shader_compiler_available );
    if (shader_compiler_available != GL_TRUE)
    {
        MessageBoxAf( MBT_OK, MBI_ERROR, "GLSL ERROR", "OpenGL driver does not have a shader compiler available." );
        __debugbreak();
    }

    // For saving binaries
    GLint binary_formats;
    glGetIntegerv( GL_NUM_PROGRAM_BINARY_FORMATS, &binary_formats );
    if (binary_formats <= 0)
    {
        MessageBoxReturnValue userChoice = MessageBoxAf( MBT_ABORTRETRYIGNORE, MBI_INFORMATION, "GL BINARY FORMATS ERROR", "There are no available binary formats for OpenGL." );
        if( userChoice == MBR_RETRY || userChoice == MBR_ABORT )
            __debugbreak();
    }
    return (shader_compiler_available == GL_TRUE);
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int OpenGLRenderer::LoadProgram( const char* vertexFilePath, const char* fragmentFilePath )
{
    GLuint programID = glCreateProgram();
    if (programID == NULL) 
        return NULL;

    GLuint vertexShader = LoadShader( vertexFilePath, GL_VERTEX_SHADER );
    GLCheckError();

    GLuint fragmentShader = LoadShader( fragmentFilePath, GL_FRAGMENT_SHADER );
    GLCheckError();

    if (!LinkProgram( programID, vertexShader, fragmentShader )) 
    {
        glDeleteProgram( programID );
        programID = NULL;
    }

    // no longer needed no matter what
    FreeShader( vertexShader );
    GLCheckError();

    FreeShader( fragmentShader );
    GLCheckError();

    return programID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::CopyToBuffer( GLuint bufferID, void const *data, size_t size )
{
    glBindBuffer( GL_ARRAY_BUFFER, bufferID );
    GLCheckError();

    // talk about sub buffer binding, and usage hints here
    // most common ones are GL_STATIC_DRAW, and GL_DYNAMIC_DRAW
    glBufferData( GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW );
    GLCheckError();

    // There's also glBufferSubData, for when you know it's large enough and only
    // want to edit part of it.  Will not be going over that yet
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int OpenGLRenderer::CreateBuffer( void const *buffer, size_t size )
{
    unsigned int bufferID;
    glGenBuffers( 1, &bufferID );
    if (bufferID == NULL) {
        return NULL;
    }

    if (buffer != nullptr) {
        CopyToBuffer( bufferID, buffer, size );
    }

    return bufferID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int OpenGLRenderer::CreateBuffer()
{
    unsigned int bufferID;
    glGenBuffers( 1, &bufferID );
    GLCheckError();

    if (bufferID == NULL)
    {
        MessageBoxAf( MBT_OK, MBI_ERROR, "Buffer ERROR", "Could not create vertex buffer." );
        __debugbreak();
        return NULL;
    }

    return bufferID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::BindBufferData( GLenum target, const GLuint& bufferID, const void* data, const size_t& size, const GLenum& bufferUsage /* = GL_DYNAMIC_DRAW */ )
{
    glBindBuffer( target, bufferID );
    GLCheckError();

    // talk about sub buffer binding, and usage hints here
    // most common ones are GL_STATIC_DRAW, and GL_DYNAMIC_DRAW
    glBufferData( target, size, data, bufferUsage );
    GLCheckError();

    // There's also glBufferSubData, for when you know it's large enough and only
    // want to edit part of it.  Will not be going over that yet

    glBindBuffer( target, 0 );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DestroyBuffer( unsigned int buffer )
{
    glDeleteBuffers( 1, &buffer );
}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int OpenGLRenderer::CreateVAO( unsigned int programID, unsigned int bufferID, VertexLayout layouts )
{
    unsigned int vaoID = 0;
    glGenVertexArrays( 1, &vaoID );
    if (vaoID == NULL) {
        MessageBoxAf( MBT_OK, MBI_ERROR, "VAO ERROR", "Could not create VAO." );
        __debugbreak();
        return NULL;
    }

    // Some more debug information that you could do in an early check
#ifdef OPENGL_CHECK_ERRORS
    GLint max_attribs;
    glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &max_attribs );
//     ConsolePrintf( "Max Vertex Attributes: %i \n", max_attribs );
#endif

    glBindVertexArray( vaoID );
    glBindBuffer( GL_ARRAY_BUFFER, bufferID );

    for (VertexLayout::const_iterator layoutIter = layouts.begin(); layoutIter != layouts.end(); ++layoutIter)
    {
        VertexAttributeProperties properites = *layoutIter;

        ProgramBindAttribute( programID, properites.name, properites.count, properites.type, properites.normalize, properites.stride, properites.offset );
    }

    GLCheckError();

    glBindBuffer( GL_ARRAY_BUFFER, NULL );
    glBindVertexArray( NULL );

    return vaoID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DestroyVAO( unsigned int vaoID )
{
    if (vaoID != NULL) {
        glDeleteVertexArrays( 1, &vaoID );
    }
}
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int OpenGLRenderer::CreateSampler( GLenum min_filter, GLenum mag_filter, GLenum u_wrap, GLenum v_wrap )
{
    GLuint samplerID;
    glGenSamplers( 1, &samplerID );
    if ( samplerID == NULL ) {
        return NULL;
    }

    glSamplerParameteri( samplerID, GL_TEXTURE_MIN_FILTER, min_filter );			// Default: GL_NEAREST_MIPMAP_LINEAR
    glSamplerParameteri( samplerID, GL_TEXTURE_MAG_FILTER, mag_filter );			// Default: GL_LINEAR
    glSamplerParameteri( samplerID, GL_TEXTURE_WRAP_S, u_wrap );					// Default: GL_REPEAT
    glSamplerParameteri( samplerID, GL_TEXTURE_WRAP_T, v_wrap );					// Default: GL_REPEAT

    return samplerID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DestroySampler( unsigned int samplerID )
{
    glDeleteSamplers( 1, &samplerID );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWithTexture( unsigned int programID, unsigned int vaoID, unsigned int numVertexes, const Matrix4f& projection, const Matrix4f& model, unsigned int samplerID, Texture* texture )
{

    GLint texIndex = 0;

    BindProgram( programID );
    GLCheckError();

    glBindVertexArray( vaoID );

    ProgramBindMatrix( programID, "gView", m_view );
    ProgramBindMatrix( programID, "gProj", projection );
    ProgramBindMatrix( programID, "gModel", model );

    ProgramBindSamplerIndex( programID, "gTexture", texIndex );
    glActiveTexture( GL_TEXTURE0 + texIndex );
    glBindTexture( GL_TEXTURE_2D, texture->GetPlatformHandle() );
    glBindSampler( texIndex, samplerID );


    // non index buffer only for now
    // type, start, count of vertices
    glDrawArrays( GL_TRIANGLES, 0, numVertexes );
    GLCheckError();

    glUseProgram( NULL );
    glBindVertexArray( NULL );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWithTexture( unsigned int programID, VertexLayout layout, unsigned int numVertexes, const Matrix4f& projection, const Matrix4f& model, unsigned int samplerID, Texture* texture )
{
    GLint texIndex = 0;

    glBindBuffer( GL_ARRAY_BUFFER, NULL );
    glBindVertexArray( NULL );

    if (m_currentlyBoundShaderID != programID)
    {
        if (m_currentlyBoundShaderID != NULL)
            UnbindProgram();

        BindProgram( programID );
        GLCheckError();
    }

    ProgramBindMatrix( programID, "gView", m_view );
    ProgramBindMatrix( programID, "gProj", projection );
    ProgramBindMatrix( programID, "gModel", model );

    ProgramBindSamplerIndex( programID, "gTexture", texIndex );
    glActiveTexture( GL_TEXTURE0 + texIndex );
    glBindTexture( GL_TEXTURE_2D, texture->GetPlatformHandle() );
    glBindSampler( texIndex, samplerID );

    for (VertexLayout::const_iterator attribIter = layout.begin(); attribIter != layout.end(); ++attribIter)
    {
        VertexAttributeProperties attrib = *attribIter;
        ProgramBindAttribute( programID, attrib.name, attrib.count, attrib.type, attrib.normalize, attrib.stride, attrib.offset );
    }

    glDrawArrays( GL_TRIANGLES, 0, numVertexes );
    GLCheckError();

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWithTextureOrtho( unsigned int programID, VertexLayout layout, unsigned int numVertexes, unsigned int samplerID, Texture* texture )
{
    GLint texIndex = 0;

    glBindBuffer( GL_ARRAY_BUFFER, NULL );
    glBindVertexArray( NULL );

    if (m_currentlyBoundShaderID != programID)
    {
        if (m_currentlyBoundShaderID != NULL)
            UnbindProgram();

        BindProgram( programID );
        GLCheckError();
    }

    ProgramBindMatrix( programID, "gView", Matrix4f::CreateIdentity() );
    ProgramBindMatrix( programID, "gProj", m_orthoProj );
    ProgramBindMatrix( programID, "gModel", Matrix4f::CreateIdentity() );

    ProgramBindSamplerIndex( programID, "gTexture", texIndex );
    glActiveTexture( GL_TEXTURE0 + texIndex );
    glBindTexture( GL_TEXTURE_2D, texture->GetPlatformHandle() );
    glBindSampler( texIndex, samplerID );

    for (VertexLayout::const_iterator attribIter = layout.begin(); attribIter != layout.end(); ++attribIter)
    {
        VertexAttributeProperties attrib = *attribIter;
        ProgramBindAttribute( programID, attrib.name, attrib.count, attrib.type, attrib.normalize, attrib.stride, attrib.offset );
    }

    glDrawArrays( GL_TRIANGLES, 0, numVertexes );
    GLCheckError();

}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::Draw( unsigned int programID, unsigned int vaoID, unsigned int numVertexes, const Matrix4f& projection, const Matrix4f& model, const unsigned int& drawMode )
{

    BindProgram( programID );
    GLCheckError();

    glBindVertexArray( vaoID );

    ProgramBindMatrix( programID, "gView", m_view );
    ProgramBindMatrix( programID, "gProj", projection );
    ProgramBindMatrix( programID, "gModel", model );

    // non index buffer only for now
    // type, start, count of vertices
    glDrawArrays( drawMode, 0, numVertexes );
    GLCheckError();

    glUseProgram( NULL );
    glBindVertexArray( NULL );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawColoredQuad( unsigned int programID, const Vector2& mins, const Vector2& maxs, const float& z, const Rgba& faceColor )
{
    if (programID == 0)
        programID = m_defaultShaderID;

    glBindBuffer( GL_ARRAY_BUFFER, NULL );
    glBindVertexArray( NULL );

    if (m_currentlyBoundShaderID != programID)
    {
        if (m_currentlyBoundShaderID != NULL)
            UnbindProgram();

        BindProgram( programID );
        GLCheckError();
    }
    
    PC_Vertexes vertexes;
    vertexes.push_back( Vertex3D_PC( Vector3( mins.x, mins.y, z ), faceColor ) );
    vertexes.push_back( Vertex3D_PC( Vector3( maxs.x, mins.y, z ), faceColor ) );
    vertexes.push_back( Vertex3D_PC( Vector3( maxs.x, maxs.y, z ), faceColor ) );
    vertexes.push_back( Vertex3D_PC( Vector3( maxs.x, maxs.y, z ), faceColor ) );
    vertexes.push_back( Vertex3D_PC( Vector3( mins.x, maxs.y, z ), faceColor ) );
    vertexes.push_back( Vertex3D_PC( Vector3( mins.x, mins.y, z ), faceColor ) );
     
    ProgramBindMatrix( programID, "gView", Matrix4f::CreateIdentity() );
    ProgramBindMatrix( programID, "gProj", m_orthoProj );
    ProgramBindMatrix( programID, "gModel", Matrix4f::CreateIdentity() );
    
    VertexLayout layout = Vertex3D_PC::GetVertexLayout( vertexes );
    for (VertexLayout::const_iterator attribIter = layout.begin(); attribIter != layout.end(); ++attribIter)
    {
        VertexAttributeProperties attrib = *attribIter;
        ProgramBindAttribute( programID, attrib.name, attrib.count, attrib.type, attrib.normalize, attrib.stride, attrib.offset );
    }

    glDrawArrays( GL_TRIANGLES, 0, vertexes.size() );
    GLCheckError();

    //glUseProgram( NULL );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawVertexes( unsigned int programID, const PUC_Vertexes& vertexes, GLenum drawMode )
{
    if (vertexes.empty())
        return;

    if (programID == 0)
        programID = m_defaultShaderID;

    glBindBuffer( GL_ARRAY_BUFFER, NULL );
    glBindVertexArray( NULL );

    if (m_currentlyBoundShaderID != programID)
    {
        if (m_currentlyBoundShaderID != NULL)
            UnbindProgram();

        BindProgram( programID );
        GLCheckError();
    }

    ProgramBindMatrix( programID, "gView", m_view );
    ProgramBindMatrix( programID, "gProj", m_perspectiveProj );
    ProgramBindMatrix( programID, "gModel", Matrix4f::CreateIdentity() );

    VertexLayout layout = Vertex3D_PUC::GetVertexLayout( vertexes );
    for (VertexLayout::const_iterator attribIter = layout.begin(); attribIter != layout.end(); ++attribIter)
    {
        VertexAttributeProperties attrib = *attribIter;
        ProgramBindAttribute( programID, attrib.name, attrib.count, attrib.type, attrib.normalize, attrib.stride, attrib.offset );
    }

    glDrawArrays( drawMode, 0, vertexes.size() );
    GLCheckError();

    //glUseProgram( NULL );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawVertexesOrtho( unsigned int programID, const PUC_Vertexes& vertexes, GLenum drawMode )
{
    if (programID == 0)
        programID = m_defaultShaderID;

    glBindBuffer( GL_ARRAY_BUFFER, NULL );
    glBindVertexArray( NULL );

    if (m_currentlyBoundShaderID != programID)
    {
        if (m_currentlyBoundShaderID != NULL)
            UnbindProgram();

        BindProgram( programID );
        GLCheckError();
    }

    ProgramBindMatrix( programID, "gView", Matrix4f::CreateIdentity() );
    ProgramBindMatrix( programID, "gProj", m_orthoProj );
    ProgramBindMatrix( programID, "gModel", Matrix4f::CreateIdentity() );

    VertexLayout layout = Vertex3D_PUC::GetVertexLayout( vertexes );
    for (VertexLayout::const_iterator attribIter = layout.begin(); attribIter != layout.end(); ++attribIter)
    {
        VertexAttributeProperties attrib = *attribIter;
        ProgramBindAttribute( programID, attrib.name, attrib.count, attrib.type, attrib.normalize, attrib.stride, attrib.offset );
    }

    glDrawArrays( drawMode, 0, vertexes.size() );
    GLCheckError();

    //glUseProgram( NULL );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawTexturedSphere( unsigned int programID, unsigned int samplerID, int debugInt, const Matrix4f& model, float radius, unsigned int slices, unsigned int stacks, Texture* texture /* = nullptr */ )
{
    GLint texIndex = 0;

    BindProgram( programID );
    GLCheckError();

    PUC_Vertexes sphereVerts = GetSpherePUCVertexArray( radius, slices, stacks );

    unsigned int buffer = CreateBuffer( sphereVerts.data(), sizeof( Vertex3D_PUC ) * sphereVerts.size() );
    unsigned int vao = CreateVAO( programID, buffer, Vertex3D_PUC::GetVertexLayout() );

    glBindVertexArray( vao );

    ProgramBindMatrix( programID, "gView", m_view );
    ProgramBindMatrix( programID, "gProj", m_perspectiveProj );
    ProgramBindMatrix( programID, "gModel", model );
    ProgramBindFloat( programID, "gTime", (float) GetCurrentSeconds() );
    ProgramBindInt32( programID, "gDebugInt", debugInt );
    ProgramBindSamplerIndex( programID, "gTexture", texIndex );

    glActiveTexture( GL_TEXTURE0 + texIndex );
    glBindTexture( GL_TEXTURE_2D, texture->GetPlatformHandle() );
    glBindSampler( texIndex, samplerID );

    // non index buffer only for now
    // type, start, count of vertices
    glDrawArrays( GL_TRIANGLES, 0, sphereVerts.size() );
    GLCheckError();

    glUseProgram( NULL );
    glBindVertexArray( NULL );
    
     DestroyBuffer( buffer );
     DestroyVAO( vao );
    
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawTexturedCuboid( unsigned int programID, unsigned int samplerID, const Matrix4f& model, float xSize, float ySize, float zSize, Texture* texture )
{
    GLint texIndex = 0;

    BindProgram( programID );
    GLCheckError();

    PUC_Vertexes cuboidVerts = GetCuboidPUCVertexArray( xSize, ySize, zSize );

    unsigned int buffer = CreateBuffer( cuboidVerts.data(), sizeof( Vertex3D_PUC ) * cuboidVerts.size() );
    unsigned int vao = CreateVAO( programID, buffer, Vertex3D_PUC::GetVertexLayout() );

    glBindVertexArray( vao );

    ProgramBindFloat( programID, "gTime", 0.0f );
    ProgramBindMatrix( programID, "gView", m_view );
    ProgramBindMatrix( programID, "gProj", m_perspectiveProj );
    ProgramBindMatrix( programID, "gModel", model );
    ProgramBindSamplerIndex( programID, "gTexture", texIndex );

    glActiveTexture( GL_TEXTURE0 + texIndex );
    glBindTexture( GL_TEXTURE_2D, texture->GetPlatformHandle() );
    glBindSampler( texIndex, samplerID );

    // non index buffer only for now
    // type, start, count of vertices
    glDrawArrays( GL_TRIANGLES, 0, cuboidVerts.size() );
    GLCheckError();

    glUseProgram( NULL );
    glBindVertexArray( NULL );

    DestroyBuffer( buffer );
    DestroyVAO( vao );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
GLuint OpenGLRenderer::LoadShader( const char* fileName, GLenum shaderType )
{
    //size_t sourceSize = 0;

    char* source = LoadTextFileToNewBuffer( fileName );
    if( source == nullptr )
        return NULL;

    GLuint shaderID = glCreateShader( shaderType );
    if ( shaderID == NULL)
        return NULL;

    // can take multiple strings - internally these will all be 
    // concatenated end-to-end
    
    //GLint len = (GLint)(sourceSize);
    glShaderSource( shaderID, 1, &source, NULL );
    delete source;

    glCompileShader( shaderID );
    GLCheckError();

    MessageBoxReturnValue userChoice = MBR_NULL;
    bool shaderLoadedSuccess = CheckShaderStatus( shaderID, fileName, userChoice );

    while (!shaderLoadedSuccess)
    {
        glDeleteShader( shaderID );

        if (userChoice == MBR_CANCEL)
            return NULL;
        
        else if (userChoice == MBR_RETRY)
        {
            //sourceSize = 0;

            char* sourceRetry = LoadTextFileToNewBuffer( fileName );
            if (sourceRetry == nullptr)
                return NULL;

            shaderID = glCreateShader( shaderType );
            if (shaderID == NULL)
                return NULL;

            // can take multiple strings - internally these will all be 
            // concatenated end-to-end
            
            //len = (GLint)(sourceSize);
            glShaderSource( shaderID, 1, &sourceRetry, NULL );
            free( sourceRetry );

            glCompileShader( shaderID );
            GLCheckError();
        }

        userChoice = MBR_NULL;
        shaderLoadedSuccess = CheckShaderStatus( shaderID, fileName, userChoice );
    }

    if (shaderLoadedSuccess)
        return shaderID;

    else
        return NULL;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::FreeShader( GLuint shaderID )
{
    if (shaderID != NULL) 
        glDeleteShader( shaderID );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::CheckShaderStatus( GLuint shaderID, const char* fileName, MessageBoxReturnValue& out_userChoice )
{
    GLint status;
    glGetShaderiv( shaderID, GL_COMPILE_STATUS, &status );
    if (status == GL_FALSE) 
    {
        // failed to compile
        GLint logSize = 0;
        glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &logSize );

        if (logSize > 0)
        {
            char* log = new char[logSize];

            glGetShaderInfoLog( shaderID, logSize, &logSize, log );

            out_userChoice = FormatAndPrintLog( fileName, log );

            delete log;
        }
        else 
        {
            // Failed, unknown reason - shouldn't happen
            // make note of this method
            GLCheckError();
        }

        return false;
    }

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::LinkProgram( GLuint programID, GLuint vertexShader, GLuint fragmentShader )
{
    if ((vertexShader == NULL) || (fragmentShader == NULL) || (programID == NULL))
        return false;

    // At this point, you can still do operations on shader
    // and they'll take - just everything has to be set before
    // you do glLinkProgram or glProgramBinary
    glAttachShader( programID, vertexShader );
    glAttachShader( programID, fragmentShader );
    GLCheckError();



    // this is just for show - this has to be set before linking for this to work
    bool save_binary = false;
    if (true == save_binary) 
    {
        // if you want to save off this binary - make sure to set this before linking
        glProgramParameteri( programID, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE );
        GLCheckError();
    }

    // This is also where you'd set options if you want the program to be seperable
    // We won't be going over how to do that now, but it basically let's you have 
    // different programs bound to different stages (pipelining)


    glLinkProgram( programID );
    GLCheckError();

    // Only need to be attached if they're 
    glDetachShader( programID, vertexShader );
    glDetachShader( programID, fragmentShader );
    GLCheckError();

    if (!CheckProgramStatus( programID )) 
        return false;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DestroyProgram( GLuint programID )
{
    if (programID != NULL) {
        glDeleteProgram( programID );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::CheckProgramStatus( GLuint programID )
{
    GLint linkStatus;
    glGetProgramiv( programID, GL_LINK_STATUS, &linkStatus );

    if (linkStatus == GL_FALSE) 
    {
        GLint logSize = 0;
        glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &logSize );

        if (logSize > 0) 
        {
            char* buffer = new char[logSize];
            glGetProgramInfoLog( programID, sizeof( buffer ), &logSize, (GLchar*)buffer );

            ConsolePrintf( "Failed to link;\n %s", buffer );
            delete( buffer );
        }
        else
            GLCheckError();

        return false;
    }
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
int OpenGLRenderer::GetUniformLocation( unsigned int programID, const std::string& name )
{
    return glGetUniformLocation( programID, name.c_str() );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::ProgramBindMatrix( GLuint progID, const char* name, const Matrix4f& mat )
{
    GLint loc = glGetUniformLocation( progID, name );
    GLCheckError();
    if (loc < 0) {
        // didn't exist
        return false;
    }

    // If you want, you can make sure it actually is the type you want
    GLuint index;
    glGetUniformIndices( progID, 1, &name, &index );
    GLCheckError();
    if (index >= 0) {
        GLint count;
        GLenum type;
        glGetActiveUniform( progID, index, 0, NULL, &count, &type, NULL );
        
        if (type != GL_FLOAT_MAT4)
        {
            MessageBoxAf( MBT_OK, MBI_ERROR, "OpenGL Error", "OpenGL Bind Matrix Error: Data is not of type GL_FLOAT_MAT4."  );
            __debugbreak();
        }
    }

    // there is just glUniformMatrix4f, but 
    // this one is more general purpose
    glUniformMatrix4fv( loc, 1, GL_FALSE, (GLfloat*)&mat );
    GLCheckError();
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::ProgramBindAttribute( GLuint programID, const char* name, GLint count, GLenum type, GLboolean normalize, GLsizei stride, GLsizei offset )
{
    GLint loc = glGetAttribLocation( programID, name );
    GLCheckError();
    if (loc < 0) {
        // does not exist
        return false;
    }

    // there is no glGetAttribIndices
    // so if you want to error check, you can go 
    // use glGetProgramiv( id, GL_ACTIVE_ATTRIBUTES, &max_attributes );
    // and glGetActiveAttrib(...) to enumerator all the attributes, and compare when you find it.

    // test to see if this even matters - that is, having more enabled than are used.
    glEnableVertexAttribArray( loc );
    GLCheckError();

    glVertexAttribPointer( loc, count, type, normalize, stride, (GLvoid*)offset );
    GLCheckError();

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::BindProgram( GLuint progID )
{
    glUseProgram( progID );
    m_currentlyBoundShaderID = progID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::UnbindProgram()
{
    glUseProgram( NULL );
    m_currentlyBoundShaderID = NULL;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::ProgramBindSamplerIndex( GLuint programID, const char* name, GLint idx )
{
    GLint loc = ProgramFindAndCheck( programID, name, GL_SAMPLER_2D );
    if (loc < 0) {
        // didn't exist
        return false;
    }


    glUniform1iv( loc, 1, (GLint*)&idx );
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::ProgramBindFloat( GLuint programID, const char* name, const float& val )
{
    GLint loc = glGetUniformLocation( programID, name );
    if (loc < 0) {
        // didn't exist
        return false;
    }

    // If you want, you can make sure it actually is the type you want
    GLuint index;
    glGetUniformIndices( programID, 1, &name, &index );
    if (index >= 0) {
        GLint count;
        GLenum type;
        glGetActiveUniform( programID, index, 0, NULL, &count, &type, NULL );
        
        if (type != GL_FLOAT)
        {
            MessageBoxAf( MBT_OK, MBI_ERROR, "Program Bind Float Error", "Type in program is not float" );
            __debugbreak();
        }
    }

    // ditto of what matrix said
    glUniform1fv( loc, 1, &val );
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::ProgramBindInt32( GLuint programID, const char* name, const int& val )
{
    GLint loc = glGetUniformLocation( programID, name );
    if (loc < 0) {
        // didn't exist
        return false;
    }

    // If you want, you can make sure it actually is the type you want
    GLuint index;
    glGetUniformIndices( programID, 1, &name, &index );
    if (index >= 0) {
        GLint count;
        GLenum type;
        glGetActiveUniform( programID, index, 0, NULL, &count, &type, NULL );

        if (type != GL_INT )
        {
            MessageBoxAf( MBT_OK, MBI_ERROR, "Program Bind Int Error", "Type in program is not int" );
            __debugbreak();
        }
    }

    // ditto of what matrix said
    glUniform1iv( loc, 1, &val );
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::ProgramBindVec2( GLuint programID, const char* name, const Vector2& val )
{
    GLint loc = glGetUniformLocation( programID, name );
    if (loc < 0) {
        // didn't exist
        return false;
    }

    // If you want, you can make sure it actually is the type you want
    GLuint index;
    glGetUniformIndices( programID, 1, &name, &index );
    if (index >= 0) {
        GLint count;
        GLenum type;
        glGetActiveUniform( programID, index, 0, NULL, &count, &type, NULL );

        if (type != GL_FLOAT_VEC2)
        {
            MessageBoxAf( MBT_OK, MBI_ERROR, "Program Bind Vector2 Error", "Type in program is not vec2" );
            __debugbreak();
        }
    }

    // ditto of what matrix said
    glUniform2fv( loc, 1, val.GetAsFloatArray() );
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::ProgramBindVec3( GLuint programID, const char* name, const Vector3& val )
{
    GLint loc = glGetUniformLocation( programID, name );
    if (loc < 0) {
        // didn't exist
        return false;
    }

    // If you want, you can make sure it actually is the type you want
    GLuint index;
    glGetUniformIndices( programID, 1, &name, &index );
    if (index >= 0) {
        GLint count;
        GLenum type;
        glGetActiveUniform( programID, index, 0, NULL, &count, &type, NULL );

        if (type != GL_FLOAT_VEC3 )
        {
            MessageBoxAf( MBT_OK, MBI_ERROR, "Program Bind Vector3 Error", "Type in program is not vec3" );
            __debugbreak();
        }
    }

    // ditto of what matrix said
    glUniform3fv( loc, 1, val.GetAsFloatArray() );
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::ProgramBindVec4f( GLuint programID, const char* name, const Vector4f& val )
{
    GLint loc = glGetUniformLocation( programID, name );
    if (loc < 0) {
        // didn't exist
        return false;
    }

    // If you want, you can make sure it actually is the type you want
    GLuint index;
    glGetUniformIndices( programID, 1, &name, &index );
    if (index >= 0) {
        GLint count;
        GLenum type;
        glGetActiveUniform( programID, index, 0, NULL, &count, &type, NULL );

        if (type != GL_FLOAT_VEC4 )
        {
            MessageBoxAf( MBT_OK, MBI_ERROR, "Program Bind Vector4f Error", "Type in program is not vec4" );
            __debugbreak();
        }
    }

    // ditto of what matrix said
    glUniform4fv( loc, 1, val.GetAsPrimitiveTypeArray() );
    return true;
}

bool OpenGLRenderer::ProgramBindLights( GLuint programID, const char* name, const Lights& lights )
{
//     GLint loc = glGetUniformLocation( programID, name );
//     if (loc < 0)
//     {
//         //didn't exist
//         return false;
//     }

    if (lights.size() > MAX_NUM_LIGHTS)
    {
        MessageBoxAf( MBT_OK, MBI_STOP, "Too Many Lights in Scene", "There are more lights in the scene than can be handled by the shader." );
        __debugbreak();
    }

    unsigned int numLights = lights.size();

    GLuint lightBlockIndex = glGetUniformBlockIndex( programID, name );
    GLCheckError();
    if (lightBlockIndex < 0)
        return false;

    GLint lightBlockSize = 0;

    glGetActiveUniformBlockiv( programID, lightBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &lightBlockSize );
    GLCheckError();
    //DeveloperConsole::WriteLine( "Light block size (in bytes):" + std::to_string( lightBlockSize ), INFO_TEXT_COLOR );

    if (lightBlockSize == 0)
        return false;

    const GLchar* names[] = { "gLightCount", "gLights[0].lightPosition", "gLights[0].lightDirection", "gLights[0].lightColor", 
                              "gLights[0].lightVectorWeight", "gLights[0].lightPower", "gLights[0].minSpotFactor", 
                              "gLights[0].minIntensity", "gLights[0].maxIntensity", "gLights[0].intensityOutsideInnerPenumbra", "gLights[0].penumbraInner", 
                              "gLights[0].penumbraOuter", "gLights[1].lightPosition" };
    const int numAttributes = 13;

    GLuint indicies[numAttributes];
    glGetUniformIndices( programID, numAttributes, names, indicies );
    GLCheckError();
    GLint offset[numAttributes];
    glGetActiveUniformsiv( programID, numAttributes, indicies, GL_UNIFORM_OFFSET, offset );
    GLCheckError();
    GLubyte* lightBlockBuffer;
    lightBlockBuffer = new GLubyte[lightBlockSize]();

    int stride = offset[numAttributes - 1] - offset[1];

    memcpy( lightBlockBuffer + offset[0], &numLights, sizeof( numLights ) );

    int lightNum = 0;
    for (Lights::const_iterator lightIter = lights.begin(); lightIter != lights.end(); ++lightIter)
    {
        PuttyLight* light = *lightIter;

        memcpy( lightBlockBuffer + (offset[1] + (lightNum * stride)), light->GetLightPosition().GetAsFloatArray(), 3 * sizeof( GLfloat ) );
        memcpy( lightBlockBuffer + (offset[2] + (lightNum * stride)), light->GetLightDirection().GetAsFloatArray(), 3 * sizeof( GLfloat ) );

        Rgba lightColor = light->GetLightColor();
        Vector4f lightColorVec4f( lightColor.Rf(), lightColor.Gf(), lightColor.Bf(), lightColor.Af() );
        memcpy( lightBlockBuffer + (offset[3] + (lightNum * stride)), lightColorVec4f.GetAsPrimitiveTypeArray(), 4 * sizeof( GLfloat ) );

        int lightVectorWeight = light->GetLightVectorWeight();
        memcpy( lightBlockBuffer + (offset[4] + (lightNum * stride)), &lightVectorWeight, sizeof( lightVectorWeight ) );

        float lightPower = light->GetLightPower();
        memcpy( lightBlockBuffer + (offset[5] + (lightNum * stride)), &lightPower, sizeof( lightPower ) );

        float minSpotFactor = light->GetMinSpotFactor();
        memcpy( lightBlockBuffer + (offset[6] + (lightNum * stride)), &minSpotFactor, sizeof( minSpotFactor ) );

        float minIntensity = light->GetMinIntensity();
        memcpy( lightBlockBuffer + (offset[7] + (lightNum * stride)), &minIntensity, sizeof( minIntensity ) );

        float maxIntensity = light->GetMaxIntensity();
        memcpy( lightBlockBuffer + (offset[8] + (lightNum * stride)), &maxIntensity, sizeof( maxIntensity ) );

        float intensityOutsideInnerPenubra = light->GetIntensityOutsideInnerPenumbra();
        memcpy( lightBlockBuffer + (offset[9] + (lightNum * stride)), &intensityOutsideInnerPenubra, sizeof( intensityOutsideInnerPenubra ) );

        float penumbraInner = light->GetPenumbraInner();
        memcpy( lightBlockBuffer + (offset[10] + (lightNum * stride)), &penumbraInner, sizeof( penumbraInner ) );

        float penumbraOuter = light->GetPenumbraOuter();
        memcpy( lightBlockBuffer + (offset[11] + (lightNum * stride)), &penumbraOuter, sizeof( penumbraOuter ) );

        lightNum++;
    }

    static GLuint uboHandle = 0;
    if (uboHandle == 0)
    {
        glGenBuffers( 1, &uboHandle );
        GLCheckError();
    }

    glBindBuffer( GL_UNIFORM_BUFFER, uboHandle );
    GLCheckError();
    glBufferData( GL_UNIFORM_BUFFER, lightBlockSize, lightBlockBuffer, GL_DYNAMIC_DRAW );
    GLCheckError();
    glBindBufferBase( GL_UNIFORM_BUFFER, 0, uboHandle );
    GLCheckError();
    glBindBuffer( GL_UNIFORM_BUFFER, NULL );
    GLCheckError();
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
GLint OpenGLRenderer::ProgramFindAndCheck( GLuint programID, const char* name, GLenum type )
{
    GLint loc = glGetUniformLocation( programID, name );
    if (loc < 0) {
        // didn't exist
        return loc;
    }

    // If you want, you can make sure it actually is the type you want
    GLuint index;
    glGetUniformIndices( programID, 1, &name, &index );
    if (index >= 0) {
        GLint count;
        GLenum check_type;
        glGetActiveUniform( programID, index, 0, NULL, &count, &check_type, NULL );
        
        if (type != check_type)
        {
            MessageBoxAf( MBT_OK, MBI_ERROR, "Program Bind Error", "The type you are trying to bind does not exist in the program" );
            __debugbreak();
        }

        if (type == check_type) {
            return loc;
        }
    }

    return -1;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::BindVertexArray( GLuint vaoID )
{
    glBindVertexArray( vaoID );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::BindIndexArray( GLuint iboID )
{
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iboID );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::SetActiveTexture( GLenum textureIndex )
{
    glActiveTexture( textureIndex );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
// void OpenGLRenderer::BindTexture( GLenum target, GLuint texture )
// {
//     glBindTexture( target, texture );
// }

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::BindSampler( GLuint unit, GLuint sampler )
{
    glBindSampler( unit, sampler );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawArrays( GLenum mode, GLint first, GLsizei count )
{
    glDrawArrays( mode, first, count );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawElements( GLenum mode, GLsizei count )
{
    glDrawElements( mode, count, GL_UNSIGNED_INT, (GLvoid*)0 );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int OpenGLRenderer::CreateFrameBuffer()
{
    GLuint fboID;
    glGenFramebuffers( 1, &fboID );

    GLCheckError();

    return fboID;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int OpenGLRenderer::CreateFrameBuffer( int width, int height, GLuint colorTexture, GLuint depthTexture /* = NULL */ )
{
    GLuint fboID = CreateFrameBuffer();
    if (fboID == NULL) {
        return fboID;
    }

    FramebufferBind( fboID, width, height );
    FrameBufferAttachTexture2D( GL_COLOR_ATTACHMENT0, colorTexture );
    FrameBufferAttachTexture2D( GL_DEPTH_ATTACHMENT, depthTexture );

    CheckFramebufferStatus();

//     GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
//     if (status != GL_FRAMEBUFFER_COMPLETE) {
//         glDeleteFramebuffers( 1, &fboID );
//         return NULL;
//     }

    FramebufferUnbind();

    return fboID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::FrameBufferAttachTexture2D( GLenum attachment, GLuint texture )
{
    if (texture != NULL) {
        glFramebufferTexture2D( GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0 /*mip index*/ );
        GLCheckError();
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::FrameBufferAttachRenderBuffer( GLenum attachment, GLuint renderbufferID )
{
    if (renderbufferID != NULL) {
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbufferID );
        GLCheckError();
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool OpenGLRenderer::CheckFramebufferStatus()
{
    // check FBO status
    GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
    switch (status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        ConsolePrintf( "Framebuffer complete. \n" ); 
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        ConsolePrintf( "[ERROR] Framebuffer incomplete: Attachment is NOT complete. \n" );
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        ConsolePrintf( "[ERROR] Framebuffer incomplete: No image is attached to FBO. \n" );
        return false;
        /*
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
        ConsolePrintf( "[ERROR] Framebuffer incomplete: Attached images have different dimensions."); 
        return false;

        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
        ConsolePrintf( "[ERROR] Framebuffer incomplete: Color attached images have different internal formats."); 
        return false;
        */
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        ConsolePrintf( "[ERROR] Framebuffer incomplete: Draw buffer. \n" );
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        ConsolePrintf( "[ERROR] Framebuffer incomplete: Read buffer. \n" );
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        ConsolePrintf( "[ERROR] Framebuffer incomplete: Unsupported by FBO implementation. \n" );
        return false;

    default:
        ConsolePrintf( "[ERROR] Framebuffer incomplete: Unknown error. \n" );
        return false;
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::FramebufferBind( GLuint fboID, GLuint width, GLuint height )
{
    glBindFramebuffer( GL_FRAMEBUFFER, fboID );
    glViewport( 0, 0, width, height );
    GLCheckError();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::FramebufferUnbind()
{
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glViewport( 0, 0, (GLsizei) m_displayWidth, (GLsizei) m_displayHeight );
    GLCheckError();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::FramebufferCopyToBack( GLuint fboID, GLuint fboTexWidth, GLuint fboTexHeight )
{
    // Copy my Framebuffer wholesale [no fx] to the front buffer
    glBindFramebuffer( GL_READ_FRAMEBUFFER, fboID );
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

    glBlitFramebuffer( 0, 0, fboTexWidth, fboTexHeight, 0, 0, (GLuint) m_displayWidth, (GLuint) m_displayHeight, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST );

    glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

    GLCheckError();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int OpenGLRenderer::CreateRenderBuffer( GLenum internalFormat, GLsizei width, GLsizei height )
{
    GLuint rboID;

    glGenRenderbuffers( 1, &rboID );
    glBindRenderbuffer( GL_RENDERBUFFER, rboID );
    glRenderbufferStorage( GL_RENDERBUFFER, internalFormat, width, height );
    glBindRenderbuffer( GL_RENDERBUFFER, 0 );

    GLCheckError();

    return rboID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::GLCheckErrorFL( const char* fileName, int lineNum )
{
    #ifdef OPENGL_CHECK_ERRORS
        GLenum error = glGetError();

//         const GLubyte* errorStr = gluErrorString( error );

        if (error != GL_NO_ERROR)
        {
            ConsolePrintf( "%s(%d): OpenGL error code %i \n", fileName, lineNum - 1, error );
            DeveloperConsole::WriteLine( ( std::string( fileName ) + "(" + std::to_string( lineNum - 1 ) + "):" + "OpenGL error code " + std::to_string( error ) ), ERROR_TEXT_COLOR );
        }
    #endif
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
MessageBoxReturnValue OpenGLRenderer::FormatAndPrintLog( const char* fileName, const char* log )
{
    char *fileExt;
    char fullPath[256]; //dummy buffer
    GetFullPathNameA( fileName, 256, fullPath, &fileExt );


    std::string logStr( log );
    std::string line = "";

    std::string errorList;

    while (logStr.find( '\n' ) != std::string::npos)
    {
        line = logStr.substr( 1, logStr.find( '\n' ) );

        ConsolePrintf( "%s", fullPath );

        std::string lineNum = line.substr( 0, line.find( ')' ) + 1 );
        ConsolePrintf( "%s", lineNum.c_str() );

        line.erase( 0, line.find( ')' ) + 1 );

        ConsolePrintf( "%s", line.c_str() );

        errorList += fileName + lineNum + line + "\n";

        logStr.erase( 0, logStr.find( '\n' ) + 1);
    }

    MessageBoxReturnValue returnVal =  MessageBoxAf( MBT_RETRYCANCEL, MBI_EXCLAMATION, "GLSL errors", "%s", errorList.c_str() );
    return returnVal;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PUC_Vertexes OpenGLRenderer::GetSpherePUCVertexArray( float radius, unsigned int slices, unsigned int stacks )
{
    Vertex3D_PUC vertex1;
    Vertex3D_PUC vertex2;
    Vertex3D_PUC vertex3;
    Vertex3D_PUC vertex4;

    PUC_Vertexes vertexes;

    for (float t = 0.0f; t < stacks; t += 1.0f) // stacks are ELEVATION so they count theta
    {
        float theta1 = PI * (t / stacks);
        float theta2 = PI * ((t + 1) / stacks);

        for (float p = 0.0f; p < slices; p += 1.0f) // slices are ORANGE SLICES so the count azimuth
        {
            float phi1 = TWO_PI * (p / slices); // azimuth goes around 0 .. 2*PI
            float phi2 = TWO_PI * ((p + 1) / slices);

            //phi2   phi1
            // |      |
            // 2------1 -- theta1
            // |\ _   |
            // |    \ |
            // 3------4 -- theta2
            //

            float sinTheta1 = sin( theta1 );
            float cosTheta1 = cos( theta1 );
            float sinTheta2 = sin( theta2 );
            float cosTheta2 = cos( theta2 );

            float sinPhi1 = sin( phi1 );
            float cosPhi1 = cos( phi1 );
            float sinPhi2 = sin( phi2 );
            float cosPhi2 = cos( phi2 );

            vertex1.position = Vector3(
                (radius * sinPhi1 * sinTheta1),
                (radius * cosTheta1),
                (radius * cosPhi1 * sinTheta1)
                );
            vertex1.uv = Vector2( phi1 / TWO_PI, theta1 / PI );

            vertex2.position = Vector3(
                (radius * sinPhi2 * sinTheta1),
                (radius * cosTheta1),
                (radius * cosPhi2 * sinTheta1)
                );
            vertex2.uv = Vector2( phi2 / TWO_PI, theta1 / PI );

            vertex3.position = Vector3(
                (radius * sinPhi2 * sinTheta2),
                (radius * cosTheta2),
                (radius * cosPhi2 * sinTheta2)
                );
            vertex3.uv = Vector2( phi2 / TWO_PI, theta2 / PI );

            vertex4.position = Vector3(
                (radius * sinPhi1 * sinTheta2),
                (radius * cosTheta2),
                (radius * cosPhi1 * sinTheta2)
                );
            vertex4.uv = Vector2( phi1 / TWO_PI, theta2 / PI );

            // facing out
            if (t == 0) // top cap
            {
                vertexes.push_back( vertex1 );
                vertexes.push_back( vertex3 );
                vertexes.push_back( vertex4 ); //t1p1, t2p2, t2p1
            }
            else if (t + 1 == stacks) //end cap
            {
                vertexes.push_back( vertex3 );
                vertexes.push_back( vertex1 );
                vertexes.push_back( vertex2 ); //t2p2, t1p1, t1p2
            }
            else
            {
                // body, facing OUT:
                vertexes.push_back( vertex1 );
                vertexes.push_back( vertex2 );
                vertexes.push_back( vertex4 );

                vertexes.push_back( vertex2 );
                vertexes.push_back( vertex3 );
                vertexes.push_back( vertex4 );
            }
        }
    }

    return vertexes;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PUC_Vertexes OpenGLRenderer::GetCuboidPUCVertexArray( float xSize, float ySize, float zSize )
{
    PUC_Vertexes vertexes;

    Vector3 mins( -(xSize / 2.0f), -(ySize / 2.0f), -(zSize / 2.0f) );
    Vector3 maxs( (xSize / 2.0f), (ySize / 2.0f), (zSize / 2.0f) );

    // +x face
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, mins.z ), Vector2( 1.0f, 1.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, mins.z ), Vector2( 1.0f, 0.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs ), Vector2( 0.0f, 0.0f ), Rgba() ) );

    vertexes.push_back( Vertex3D_PUC( Vector3( maxs ), Vector2( 0.0f, 0.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, maxs.z ), Vector2( 0.0f, 1.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, mins.z ), Vector2( 1.0f, 1.0f ), Rgba() ) );

    // +y face
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs ), Vector2( 1.0f, 1.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, mins.z ), Vector2( 1.0f, 0.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, mins.z ), Vector2( 0.0f, 0.0f ), Rgba() ) );

    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, mins.z ), Vector2( 0.0f, 0.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, maxs.z ), Vector2( 0.0f, 1.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs ), Vector2( 1.0f, 1.0f ), Rgba() ) );

    // +z face
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, maxs.z ), Vector2( 1.0f, 1.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs ), Vector2( 1.0f, 0.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, maxs.z ), Vector2( 0.0f, 0.0f ), Rgba() ) );
    
    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, maxs.z ), Vector2( 0.0f, 0.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, maxs.z ), Vector2( 0.0f, 1.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, maxs.z ), Vector2( 1.0f, 1.0f ), Rgba() ) );

    // -x face
    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, maxs.z ), Vector2( 1.0f, 1.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, maxs.z ), Vector2( 1.0f, 0.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, mins.z ), Vector2( 0.0f, 0.0f ), Rgba() ) );

    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, mins.z ), Vector2( 0.0f, 0.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( mins ), Vector2( 0.0f, 1.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, maxs.z ), Vector2( 1.0f, 1.0f ), Rgba() ) );

    // -y face
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, mins.z ), Vector2( 1.0f, 1.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, maxs.z ), Vector2( 1.0f, 0.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, maxs.z ), Vector2( 0.0f, 0.0f ), Rgba() ) );

    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, maxs.z ), Vector2( 0.0f, 0.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( mins ), Vector2( 0.0f, 1.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, mins.z ), Vector2( 1.0f, 1.0f ), Rgba() ) );

    // -z face
    vertexes.push_back( Vertex3D_PUC( Vector3( mins ), Vector2( 1.0f, 1.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, mins.z ), Vector2( 1.0f, 0.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, mins.z ), Vector2( 0.0f, 0.0f ), Rgba() ) );

    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, mins.z ), Vector2( 0.0f, 0.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, mins.z ), Vector2( 0.0f, 1.0f ), Rgba() ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( mins ), Vector2( 1.0f, 1.0f ), Rgba() ) );


    return vertexes;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::SwapBuffers()
{
	::SwapBuffers( m_deviceContext );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::Clear( const Rgba& color )
{
	glClearColor( color.Rf(), color.Gf(), color.Bf(), 1.0f );
	glClearDepth( 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::PushMatrix()
{
	glPushMatrix();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::PopMatrix()
{
	glPopMatrix();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::Begin( GLenum drawMode )
{
	glBegin( drawMode );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::End()
{
	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::Enable( GLenum mode )
{
	glEnable( mode );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::Disable( GLenum mode )
{
	glDisable( mode );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::Vertex2f( const Vector2& vertex )
{
	glVertex2f( vertex.x, vertex.y );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::Vertex3f( const Vector3& vertex )
{
	glVertex3f( vertex.x, vertex.y, vertex.z );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::TexCoord2f( const Vector2& texCoord )
{
	glTexCoord2f( texCoord.x, texCoord.y );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::LoadIdentity()
{
	glLoadIdentity();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::LoadMatrixf( const GLfloat* matrixPtr )
{
	glLoadMatrixf( matrixPtr );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::BlendFunct( GLenum sfactor, GLenum dfactor )
{
	glBlendFunc( sfactor, dfactor );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::BindTexture( GLenum target, GLuint texture )
{
	glBindTexture( target, texture );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::SetColor( const Rgba& color )
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::SetLineSize( float lineSize )
{
	glLineWidth( lineSize );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::SetPointSize( float pointSize )
{
	glPointSize( pointSize );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::SetModelViewMatrixMode()
{
	glMatrixMode( GL_MODELVIEW );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::SetProjectionMatrixMode()
{
	glMatrixMode( GL_PROJECTION );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::SetModelViewRotation( float angleDegrees, float amountAroundX, float amountAroundY, float amountAroundZ )
{
	glRotatef( angleDegrees, amountAroundX, amountAroundY, amountAroundZ );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::SetOrthographicView(float width, float height )
{
	glLoadIdentity();

	if( width != 0.0f && height != 0.0f )
		glOrtho( 0.0f, width, 0.0f, height, 0.0f, 1.0f );

	else
		glOrtho( 0.0f, m_displayWidth, 0.0f, m_displayHeight, 0.0f, 0.1f );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::SetPerspective( float fieldOfViewDegreesVertical, float aspectRatio, float nearDepth, float farDepth )
{
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glLoadIdentity();

	gluPerspective( fieldOfViewDegreesVertical, aspectRatio, nearDepth, farDepth );
}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::SetModelViewTranslation(float x, float y, float z)
{
	glTranslatef( x, y, z );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::SetModelViewScale( float xScale, float yScale, float zScale )
{
	glScalef( xScale, yScale, zScale );
}
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::CreateOpenGLRenderingContext()
{
	PIXELFORMATDESCRIPTOR pixelFormatDescription;

	memset( &pixelFormatDescription, 0, sizeof( PIXELFORMATDESCRIPTOR ) );

	pixelFormatDescription.nSize = sizeof( PIXELFORMATDESCRIPTOR );
	pixelFormatDescription.nVersion	= 1;
	pixelFormatDescription.cColorBits = 32;
	pixelFormatDescription.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	int	pixelFormat	= ChoosePixelFormat( m_deviceContext, &pixelFormatDescription );
	BOOL didSetFormat = SetPixelFormat( m_deviceContext, pixelFormat, &pixelFormatDescription );

	if( didSetFormat )
	{
		m_renderingContext = wglCreateContext( m_deviceContext );
		wglMakeCurrent( m_deviceContext, m_renderingContext );

		float dimensions[ 4 ];
		glGetFloatv( GL_VIEWPORT, dimensions );

		m_displayWidth = dimensions[ 2 ];
		m_displayHeight = dimensions[ 3 ];
	}

	glLineWidth( 1.0f );
	glEnable( GL_LINE_SMOOTH );
	glPointSize( 3.0f );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DestroyOpenGLRenderingContext()
{
	wglDeleteContext( m_renderingContext );
}


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::ApplyCameraTransformZUp( const Camera3D& camera )
{
	// Put us in our preferred coordinate system: +X is east (Forward), +Y is north, +Z is up
	glRotatef( -90.0f, 1.0f, 0.0f, 0.0f ); // lean "forward" 90 degrees to put +Z up ( was +Y )
	glRotatef( 90.0f, 0.0f, 0.0f, 1.0f ); // spin "left" 90 degrees, to put +X forward ( was +Y)

	// Orient the view per the camera's orientation
	glRotatef( -camera.m_orientation.rollDegrees, 1.0f, 0.0f, 0.0f );
	glRotatef( -camera.m_orientation.pitchDegrees, 0.0f, 1.0f, 0.0f );
	glRotatef( -camera.m_orientation.yawDegrees, 0.0f, 0.0f, 1.0f );

	// Position the view per the camera's position
	glTranslatef( -camera.m_position.x, -camera.m_position.y, -camera.m_position.z );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Matrix4f OpenGLRenderer::CalculateProjectionPerspectiveMatrix( float fovDegrees, float ratio, float nearZ, float farZ, bool setMatrix )
{
    float fovRadians = ConvertDegreesToRadians( fovDegrees );
    float size = atan2( fovRadians, 2.0f );

    float w = size;
    float h = size;

    if (ratio > 1.0f)
        w *= ratio;
    else
        h /= ratio;

    float q = farZ / (farZ - nearZ);

    Matrix4f perspectiveMatrix;
    perspectiveMatrix.m_iBasis =      Vector4f( 1.0f / w, 0.0f,     0.0f,              0.0f  );
    perspectiveMatrix.m_jBasis =      Vector4f( 0.0f,     1.0f / h, 0.0f,              0.0f  );
    perspectiveMatrix.m_kBasis =      Vector4f( 0.0f,     0.0f,     -q,                -1.0f );
    perspectiveMatrix.m_translation = Vector4f( 0.0f,     0.0f,     -1.0f * nearZ * q, 0.0f  );

    if ( setMatrix )
        m_perspectiveProj = perspectiveMatrix;

    return perspectiveMatrix;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Matrix4f OpenGLRenderer::CalculateOrthographicMatrix( float width, float height, float farZ, float nearZ, bool setMatrix )
{
    float cX = 2.0f / width;
    float cY = 2.0f / height;
    float cZ = -2.0f / (farZ - nearZ);

    float tX = -1.0f;
    float tY = -1.0f;
    float tZ = (farZ + nearZ) / (farZ - nearZ);

    Matrix4f ortho;
    ortho.m_iBasis = Vector4f( cX, 0.0f, 0.0f, 0.0f );
    ortho.m_jBasis = Vector4f( 0.0f, cY, 0.0f, 0.0f );
    ortho.m_kBasis = Vector4f( 0.0f, 0.0f, cZ, 0.0f );
    ortho.m_translation = Vector4f( tX, tY, tZ, 1.0f );

    if (setMatrix)
        m_orthoProj = ortho;

    return ortho;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Matrix4f OpenGLRenderer::CalculateViewMatrix( const Vector3& cameraPos, const Vector3& lookAtTarget, bool setMatrix )
{
	Matrix4f cameraPosMatrix = Matrix4f::CreateTranslation( cameraPos );
	cameraPosMatrix.m_translation.x = -cameraPosMatrix.m_translation.x;
	cameraPosMatrix.m_translation.y = -cameraPosMatrix.m_translation.y;
	cameraPosMatrix.m_translation.z = -cameraPosMatrix.m_translation.z;

	Vector3 cameraK = cameraPos - lookAtTarget;
	cameraK.Normalize();
	Vector3 cameraI = CrossProduct( Vector3( 0.0f, 1.0f, 0.0f ), cameraK );
	cameraI.Normalize();
	Vector3 cameraJ = CrossProduct( cameraK, cameraI );

	Matrix4f orientationMatrix;
	orientationMatrix.m_iBasis = Vector4f( cameraI.x, cameraI.y, cameraI.z, 0 );
	orientationMatrix.m_jBasis = Vector4f( cameraJ.x, cameraJ.y, cameraJ.z, 0 );
	orientationMatrix.m_kBasis = Vector4f( cameraK.x, cameraK.y, cameraK.z, 0 );
	orientationMatrix.m_translation = Vector4f( 0.0f, 0.0f, 0.0f, 1.0f );

	orientationMatrix.Transpose();

	Matrix4f view =  MatrixMultiply( cameraPosMatrix, orientationMatrix );
    
    if (setMatrix)
        m_view = view;

    return view;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Matrix4f OpenGLRenderer::CalculateViewMatrix( const Camera3D& camera, bool setMatrix )
{
	Matrix4f cameraPosMatrix = Matrix4f::CreateTranslation( camera.m_position );
	cameraPosMatrix.m_translation.x = -cameraPosMatrix.m_translation.x;
	cameraPosMatrix.m_translation.y = -cameraPosMatrix.m_translation.y;
	cameraPosMatrix.m_translation.z = -cameraPosMatrix.m_translation.z;

	Vector3 cameraK = camera.GetForwardVector() * -1.0f;
	cameraK.Normalize();
	Vector3 cameraI = CrossProduct( Vector3( 0.0f, 1.0f, 0.0f ), cameraK );
	cameraI.Normalize();
	Vector3 cameraJ = CrossProduct( cameraK, cameraI );

	Matrix4f orientationMatrix;
	orientationMatrix.m_iBasis = Vector4f( cameraI.x, cameraI.y, cameraI.z, 0 );
	orientationMatrix.m_jBasis = Vector4f( cameraJ.x, cameraJ.y, cameraJ.z, 0 );
	orientationMatrix.m_kBasis = Vector4f( cameraK.x, cameraK.y, cameraK.z, 0 );
	orientationMatrix.m_translation = Vector4f( 0.0f, 0.0f, 0.0f, 1.0f );

	orientationMatrix.Transpose();

	Matrix4f view = MatrixMultiply( cameraPosMatrix, orientationMatrix );
    
    if ( setMatrix )
        m_view = view;

    return view;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawTriangle2D( const Vector2& v0, const Vector2& v1, const Vector2& v2, const Rgba& color ) 
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );
	
	glBegin( GL_LINE_LOOP );
	glVertex2f( v0.x, v0.y );
	glVertex2f( v1.x, v1.y );
	glVertex2f( v2.x, v2.y );
	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawPolygon2D( const std::vector<Vector2>& vertices, const Rgba& color /* = Rgba() */, float lineSize /* = 1.0f */ )
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );
	glLineWidth( lineSize );
	if( vertices.size() <  3 )
		return;

	glBegin( GL_LINE_LOOP );

	for( std::vector<Vector2>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter )
	{
		const Vector2& vertex = ( *vertexIter );
		glVertex2f( vertex.x, vertex.y );
	}

	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawFilledPolygon2D( const std::vector<Vector2>& vertices, const Rgba& color /* = Rgba() */ )
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );

	if( vertices.size() <  3 )
		return;

	glBegin( GL_POLYGON );

	for( std::vector<Vector2>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter )
	{
		const Vector2& vertex = ( *vertexIter );
		glVertex2f( vertex.x, vertex.y );
	}

	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawPoints2D( const std::vector<Vector2>& points, const Rgba& color /* = Rgba() */, float pointSize )
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );
	glPointSize( pointSize );
	if( points.empty() )
		return;

	glBegin( GL_POINTS );

	for( std::vector<Vector2>::const_iterator pointIter = points.begin(); pointIter != points.end(); ++pointIter )
	{
		const Vector2& point = ( *pointIter );
		glVertex2f( point.x, point.y );
	}

	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawPoint2D( const Vector2& point, const Rgba& color /* = Rgba() */, float pointSize )
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );
	glPointSize( pointSize );
	glBegin( GL_POINTS );

	glVertex2f( point.x, point.y );

	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawQuad2D( const Vector2s& vertices, const Rgba& color /* = Rgba() */ )
{
	if( vertices.size() != 4 )
	{
		// assert error here
		return;
	}

	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );

	glBegin( GL_QUADS );

	for( std::vector<Vector2>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter )
	{
		const Vector2& vertex = ( *vertexIter );
		glVertex2f( vertex.x, vertex.y );
	}

	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawLineSegment2D( const Vector2& startPoint, const Vector2& endPoint, const Rgba& color /* = Rgba() */ )
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );
	
	glBegin( GL_LINES );

	glVertex2f( startPoint.x, startPoint.y );
	glVertex2f( endPoint.x, endPoint.y );

	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawTexturedQuad2D( const Texture& texture, const Vector2s& vertices, const Vector2s& textureCoordinates, const Rgba& tint /* = Rgba() */ )
{
	if( vertices.size() != 4 )
		return;

	if( vertices.size() != textureCoordinates.size() )
		return;

	glEnable( GL_TEXTURE_2D );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture( GL_TEXTURE_2D, texture.GetPlatformHandle() );
	glColor4f( tint.Rf(), tint.Gf(), tint.Bf(), tint.Af() );

	glBegin( GL_QUADS );

	Vector2s::const_iterator vertexIter = vertices.begin();
	Vector2s::const_iterator textureCoordIter = textureCoordinates.begin();
	for( ; vertexIter != vertices.end(); ++vertexIter, ++textureCoordIter )
	{
		const Vector2& vertex = ( *vertexIter );
		const Vector2& textureCoordinate = ( *textureCoordIter );
		glTexCoord2f( textureCoordinate.x, textureCoordinate.y );
		glVertex2f( vertex.x, vertex.y );

	}

	glEnd();
	glDisable( GL_TEXTURE_2D );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawTriangle3D( const Vector3& v0, const Vector3& v1, const Vector3& v2, const Rgba& color /* = Rgba() */ )
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );

	glBegin( GL_TRIANGLES );
	glVertex3f( v0.x, v0.y, v0.z );
	glVertex3f( v1.x, v1.y, v1.z );
	glVertex3f( v2.x, v2.y, v2.z );
	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawPolygon3D( const Vector3s& vertices, const Rgba& color /* = Rgba() */, float lineSize /* = 1.0f */ )
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );
	glLineWidth( lineSize );
	if( vertices.size() <  3 )
		return;

	glBegin( GL_LINE_LOOP );

	for( std::vector<Vector3>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter )
	{
		const Vector3& vertex = ( *vertexIter );
		glVertex3f( vertex.x, vertex.y, vertex.z );
	}

	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawFilledPolygon3D( const Vector3s& vertices, const Rgba& color )
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );

	if( vertices.size() <  3 )
		return;

	glBegin( GL_POLYGON );

	for( std::vector<Vector3>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter )
	{
		const Vector3& vertex = ( *vertexIter );
		glVertex3f( vertex.x, vertex.y, vertex.z );
	}

	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawPoints3D( const Vector3s& points, const Rgba& color, float pointSize )
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );
	glPointSize( pointSize );
	if( points.empty() )
		return;

	glBegin( GL_POINTS );

	for( std::vector<Vector3>::const_iterator pointIter = points.begin(); pointIter != points.end(); ++pointIter )
	{
		const Vector3& point = ( *pointIter );
		glVertex3f( point.x, point.y, point.z );
	}

	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawPoint3D( const Vector3& point, const Rgba& color, float pointSize )
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );
	glPointSize( pointSize );
	glBegin( GL_POINTS );

	glVertex3f( point.x, point.y, point.z );

	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawQuad3D( const Vector3s& vertices, const Rgba& color )
{
	if( vertices.size() != 4 )
	{
		// assert error here
		return;
	}

	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );

	glBegin( GL_QUADS );

	for( std::vector<Vector3>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter )
	{
		const Vector3& vertex = ( *vertexIter );
		glVertex3f( vertex.x, vertex.y, vertex.z );
	}

	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawLineSegment3D( const Vector3& startPoint, const Vector3& endPoint, const Rgba& color )
{
	glColor4f( color.Rf(), color.Gf(), color.Bf(), color.Af() );

	glBegin( GL_LINES );

	glVertex3f( startPoint.x, startPoint.y, startPoint.z );
	glVertex3f( endPoint.x, endPoint.y, endPoint.z );

	glEnd();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawTexturedQuad3D( const Texture& texture, const Vector3s& vertices, const Vector2s& textureCoordinates, const Rgba& tint )
{
	if( vertices.size() != 4 )
		return;

	if( vertices.size() != textureCoordinates.size() )
		return;

	glEnable( GL_TEXTURE_2D );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture( GL_TEXTURE_2D, texture.GetPlatformHandle() );
	glColor4f( tint.Rf(), tint.Gf(), tint.Bf(), tint.Af() );

	glBegin( GL_QUADS );

	Vector3s::const_iterator vertexIter = vertices.begin();
	Vector2s::const_iterator textureCoordIter = textureCoordinates.begin();
	for( ; vertexIter != vertices.end(); ++vertexIter, ++textureCoordIter )
	{
		const Vector3& vertex = ( *vertexIter );
		const Vector2& textureCoordinate = ( *textureCoordIter );
		glTexCoord2f( textureCoordinate.x, textureCoordinate.y );
		glVertex3f( vertex.x, vertex.y, vertex.z );

	}
	glDisable( GL_TEXTURE_2D );
	glEnd();
	
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawOriginAxes( const float& size )
{
	glDisable( GL_TEXTURE_2D );

	Vector3 origin( 0.0f, 0.0f, 0.0f );
	Vector3 x( size, 0.0f, 0.0f );
	Vector3 y( 0.0f, size, 0.0f );
	Vector3 z( 0.0f, 0.0f, size );

	DrawLineSegment3D( origin, x, Rgba::RED );
	DrawLineSegment3D( origin, y, Rgba::GREEN );
	DrawLineSegment3D( origin, z, Rgba::BLUE );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawAxes( unsigned int programID, float length, Matrix4f proj, Matrix4f view )
{
//     static PUC_Vertexes vertexes;
//     if (vertexes.size() == 0)
//     {
//         vertexes.push_back( Vertex3D_PUC( Vector3( 0.0f, 0.0f, 0.0f ), Vector2( 0.0f, 0.0f ), Rgba::RED ) );
//         vertexes.push_back( Vertex3D_PUC( Vector3( length, 0.0f, 0.0f ), Vector2( 0.0f, 0.0f ), Rgba::RED ) );
//         vertexes.push_back( Vertex3D_PUC( Vector3( 0.0f, 0.0f, 0.0f ), Vector2( 0.0f, 0.0f ), Rgba::GREEN ) );
//         vertexes.push_back( Vertex3D_PUC( Vector3( 0.0f, length, 0.0f ), Vector2( 0.0f, 0.0f ), Rgba::GREEN ) );
//         vertexes.push_back( Vertex3D_PUC( Vector3( 0.0f, 0.0f, 0.0f ), Vector2( 0.0f, 0.0f ), Rgba::BLUE ) );
//         vertexes.push_back( Vertex3D_PUC( Vector3( 0.0f, 0.0f, length ), Vector2( 0.0f, 0.0f ), Rgba::BLUE ) );
//     }
// 
//     static unsigned int buffer = CreateBuffer( vertexes.data(), sizeof( Vertex3D_PUC ) * vertexes.size() );
//     static unsigned int vao = CreateVAO( programID, buffer, Vertex3D_PUC::GetVertexLayout() );
// 
//     BindProgram( programID );
//     GLCheckError();
// 
//     glBindVertexArray( vao );
// 
//     ProgramBindMatrix( programID, "gModel", Matrix4f::CreateIdentity() );
//     ProgramBindMatrix( programID, "gView", view );
//     ProgramBindMatrix( programID, "gProj", proj );
// 
//     // non index buffer only for now
//     // type, start, count of vertices
//     glDrawArrays( GL_LINES, 0, vertexes.size() );
//     GLCheckError();
// 
//     glUseProgram( NULL );
//     glBindVertexArray( NULL );


    if (programID == 0)
        programID = m_defaultShaderID;

    glBindBuffer( GL_ARRAY_BUFFER, NULL );
    glBindVertexArray( NULL );

    BindProgram( programID );
    GLCheckError();

    PUC_Vertexes vertexes;
    vertexes.push_back( Vertex3D_PUC( Vector3( 0.0f, 0.0f, 0.0f ), Vector2( 0.0f, 0.0f ), Rgba::RED ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( length, 0.0f, 0.0f ), Vector2( 0.0f, 0.0f ), Rgba::RED ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( 0.0f, 0.0f, 0.0f ), Vector2( 0.0f, 0.0f ), Rgba::GREEN ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( 0.0f, length, 0.0f ), Vector2( 0.0f, 0.0f ), Rgba::GREEN ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( 0.0f, 0.0f, 0.0f ), Vector2( 0.0f, 0.0f ), Rgba::BLUE ) );
    vertexes.push_back( Vertex3D_PUC( Vector3( 0.0f, 0.0f, length ), Vector2( 0.0f, 0.0f ), Rgba::BLUE ) );

    ProgramBindMatrix( programID, "gView", m_view );
    ProgramBindMatrix( programID, "gProj", proj );
    ProgramBindMatrix( programID, "gModel", Matrix4f::CreateIdentity() );

    VertexLayout layout = Vertex3D_PUC::GetVertexLayout( vertexes );
    for (VertexLayout::const_iterator attribIter = layout.begin(); attribIter != layout.end(); ++attribIter)
    {
        VertexAttributeProperties attrib = *attribIter;
        ProgramBindAttribute( programID, attrib.name, attrib.count, attrib.type, attrib.normalize, attrib.stride, attrib.offset );
    }

    glDrawArrays( GL_LINES, 0, vertexes.size() );


    glUseProgram( NULL );



// 	//Use Red, Green, Blue for X Y Z respectively
// 	glLineWidth(3.0f);
// 	glBegin(GL_LINES);
// 	//X axis
// 	glColor3f(1.0f,0.0f,0.0f);
// 	glVertex3f(0.0f,0.0f,0.0f);
// 	glVertex3f(length,0.0f,0.0f);
// 	//Y axis
// 	glColor3f(0.0f,1.0f,0.0f);
// 	glVertex3f(0.0f,0.0f,0.0f);
// 	glVertex3f(0.0f,length,0.0f);
// 	//Z axis
// 	glColor3f(0.0f,0.0f,1.0f);
// 	glVertex3f(0.0f,0.0f,0.0f);
// 	glVertex3f(0.0f,0.0f,length);
// 	glEnd();

}

///---------------------------------------------------------------------------------
/// Geometries
///---------------------------------------------------------------------------------
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWireTeapot( GLdouble size )
{
	renderWireTeapot( size );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawSolidTeapot( GLdouble size )
{
	renderSolidTeapot( size );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWireCube( GLdouble size )
{
	renderWireCube( size );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawSolidCube( GLdouble size )
{
	renderSolidCube( size );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWireSphere( GLdouble radius, GLint slices, GLint stacks )
{
	renderWireSphere( radius, slices, stacks );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawSolidSphere( GLdouble radius, GLint slices, GLint stacks )
{
	renderSolidSphere( radius, slices, stacks );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWireCone( GLdouble base, GLdouble height, GLint slices, GLint stacks )
{
	renderWireCone( base, height, slices, stacks );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawSolidCone( GLdouble base, GLdouble height, GLint slices, GLint stacks )
{
	renderSolidCone( base, height, slices, stacks );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWireTorus( GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings )
{
	renderWireTorus( innerRadius, outerRadius, sides, rings );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawSolidTorus( GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings )
{
	renderSolidTorus( innerRadius, outerRadius, sides, rings );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWireDodecahedron()
{
	renderWireDodecahedron();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawSolidDodecahedron()
{
	renderSolidDodecahedron();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWireOctahedron()
{
	renderWireOctahedron();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawSolidOctahedron()
{
	renderSolidOctahedron();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWireTetrahedron()
{
	renderWireTetrahedron();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawSolidTetrahedron()
{
	renderSolidTetrahedron();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWireIcosahedron()
{
	renderWireIcosahedron();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawSolidIcosahedron()
{
	renderSolidIcosahedron();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawWireSierpinskiSponge( int num_levels, GLdouble offset[3], GLdouble scale )
{
	renderWireSierpinskiSponge( num_levels, offset, scale );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void OpenGLRenderer::DrawSolidSierpinskiSponge( int num_levels, GLdouble offset[3], GLdouble scale )
{
	renderSolidSierpinskiSponge( num_levels, offset, scale );
}