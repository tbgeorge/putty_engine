// //=================================================================================
// // OGLRenderer.cpp
// // Author: Tyler George
// // Date  : June 24, 2015
// //=================================================================================
// 
// ////===========================================================================================
// ///===========================================================================================
// // Includes
// ///===========================================================================================
// ////===========================================================================================
// #include "Engine/Renderer/OGLRenderer.hpp"
// #include "Engine/Renderer/OpenGL/glext.h"
// #include "Engine/Renderer/OpenGL/glxext.h"
// #include "Engine/Renderer/OpenGL/wglext.h"
// #include "Engine/Utilities/Console.hpp"
// #include "Engine/Utilities/DeveloperConsole.hpp"
// #include "Engine/Utilities/FileUtilities.hpp"
// 
// ////===========================================================================================
// ///===========================================================================================
// // Function Pointer Initialization
// ///===========================================================================================
// ////===========================================================================================
// 
// // PFNGLGENBUFFERSPROC		            glGenBuffers                = nullptr;
// // PFNGLDELETEBUFFERSPROC	            glDeleteBuffers             = nullptr;
// // PFNGLBINDBUFFERPROC		            glBindBuffer                = nullptr;
// // PFNGLBUFFERDATAPROC		            glBufferData                = nullptr;
// // PFNGLGENERATEMIPMAPPROC	            glGenerateMipmap            = nullptr;
// // PFNGLCREATESHADERPROC               glCreateShader              = nullptr;
// // PFNGLCOMPILESHADERPROC              glCompileShader             = nullptr;
// // PFNGLSHADERSOURCEPROC               glShaderSource              = nullptr;
// // PFNGLDELETESHADERPROC               glDeleteShader              = nullptr;
// // PFNGLGETSHADERIVPROC                glGetShaderiv               = nullptr;
// // PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog          = nullptr;
// // PFNGLCREATEPROGRAMPROC              glCreateProgram             = nullptr;
// // PFNGLATTACHSHADERPROC               glAttachShader              = nullptr;
// // PFNGLDETACHSHADERPROC               glDetachShader              = nullptr;
// // PFNGLLINKPROGRAMPROC                glLinkProgram               = nullptr;
// // PFNGLDELETEPROGRAMPROC              glDeleteProgram             = nullptr;
// // PFNGLPROGRAMPARAMETERIPROC          glProgramParameteri         = nullptr;
// // PFNGLGETPROGRAMIVPROC               glGetProgramiv              = nullptr;
// // PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog         = nullptr;
// // PFNGLUSEPROGRAMPROC                 glUseProgram                = nullptr;
// // PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation        = nullptr;
// // PFNGLGETUNIFORMINDICESPROC          glGetUniformIndices         = nullptr;
// // PFNGLGETACTIVEUNIFORMPROC           glGetActiveUniform          = nullptr;
// // PFNGLBINDATTRIBLOCATIONPROC         glBindAttribLocation        = nullptr;
// // PFNGLUNIFORM1FVPROC                 glUniform1fv                = nullptr;
// // PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv          = nullptr;
// // PFNGLUNIFORM3FVPROC                 glUniform3fv                = nullptr;
// // PFNGLUNIFORM4FVPROC                 glUniform4fv                = nullptr;
// // PFNGLUNIFORM1IVPROC                 glUniform1iv                = nullptr;
// // PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays           = nullptr;
// // PFNGLDELETEVERTEXARRAYSPROC         glDeleteVertexArrays        = nullptr;
// // PFNGLBINDVERTEXARRAYPROC            glBindVertexArray           = nullptr;
// // PFNGLGETATTRIBLOCATIONPROC          glGetAttribLocation         = nullptr;
// // PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray   = nullptr;
// // PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer       = nullptr;
// // PFNGLGETPROGRAMBINARYPROC           glGetProgramBinary          = nullptr;
// // PFNGLPROGRAMBINARYPROC              glProgramBinary             = nullptr;
// // PFNGLGENSAMPLERSPROC                glGenSamplers               = nullptr;
// // PFNGLSAMPLERPARAMETERIPROC          glSamplerParameteri         = nullptr;
// // PFNGLACTIVETEXTUREPROC              glActiveTexture             = nullptr;
// // PFNGLBINDSAMPLERPROC                glBindSampler               = nullptr;
// // PFNGLDELETESAMPLERSPROC             glDeleteSamplers            = nullptr;
// 
// ////===========================================================================================
// ///===========================================================================================
// //  Constructors/Destructors
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// OGLRenderer::OGLRenderer()
//     : m_windowHandle( nullptr )
//     , m_deviceContext( nullptr )
//     , m_renderingContext( nullptr )
//     , m_displaySize( 0.0f, 0.0f )
//     , m_perspectiveProj( Matrix4f::CreateIdentity() )
//     , m_orthographicProj( Matrix4f::CreateIdentity() )
//     , m_currentView( Matrix4f::CreateIdentity() )
// {
// 
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// OGLRenderer::~OGLRenderer()
// {
//     delete m_windowHandle;
//     m_windowHandle = nullptr;
//     delete m_deviceContext;
//     m_deviceContext = nullptr;
//     delete m_renderingContext;
//     m_renderingContext = nullptr;
// }
// 
// 
// ////===========================================================================================
// ///===========================================================================================
// // Public Initialization
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::Startup( HWND windowHandle )
// {
//     m_windowHandle = windowHandle;
//     m_deviceContext = GetDC( windowHandle );
// 
//     CreateOpenGLRenderingContext();
//     InitializeAdvancedOpenGLFunctions();
// }
// 
// ////===========================================================================================
// ///===========================================================================================
// // Protected Initialization
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::CreateOpenGLRenderingContext()
// {
//     PIXELFORMATDESCRIPTOR pixelFormatDescription;
// 
//     memset( &pixelFormatDescription, 0, sizeof( PIXELFORMATDESCRIPTOR ) );
// 
//     pixelFormatDescription.nSize = sizeof( PIXELFORMATDESCRIPTOR );
//     pixelFormatDescription.nVersion = 1;
//     pixelFormatDescription.cColorBits = 32;
//     pixelFormatDescription.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
// 
//     int	pixelFormat = ChoosePixelFormat( m_deviceContext, &pixelFormatDescription );
//     BOOL didSetFormat = SetPixelFormat( m_deviceContext, pixelFormat, &pixelFormatDescription );
// 
//     if (didSetFormat)
//     {
//         m_renderingContext = wglCreateContext( m_deviceContext );
//         wglMakeCurrent( m_deviceContext, m_renderingContext );
// 
//         float dimensions[4];
//         glGetFloatv( GL_VIEWPORT, dimensions );
// 
//         m_displaySize.x = dimensions[2];
//         m_displaySize.y = dimensions[3];
//     }
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::InitializeAdvancedOpenGLFunctions()
// {
// //     glGenBuffers                = (PFNGLGENBUFFERSPROC)                 wglGetProcAddress( "glGenBuffers" );
// //     glDeleteBuffers             = (PFNGLDELETEBUFFERSPROC)              wglGetProcAddress( "glDeleteBuffers" );
// //     glBindBuffer                = (PFNGLBINDBUFFERPROC)                 wglGetProcAddress( "glBindBuffer" );
// //     glBufferData                = (PFNGLBUFFERDATAPROC)                 wglGetProcAddress( "glBufferData" );
// //     glGenerateMipmap            = (PFNGLGENERATEMIPMAPPROC)             wglGetProcAddress( "glGenerateMipmap" );
// //     glCreateShader              = (PFNGLCREATESHADERPROC)               wglGetProcAddress( "glCreateShader" );
// //     glCompileShader             = (PFNGLCOMPILESHADERPROC)              wglGetProcAddress( "glCompileShader" );
// //     glShaderSource              = (PFNGLSHADERSOURCEPROC)               wglGetProcAddress( "glShaderSource" );
// //     glDeleteShader              = (PFNGLDELETESHADERPROC)               wglGetProcAddress( "glDeleteShader" );
// //     glGetShaderiv               = (PFNGLGETSHADERIVPROC)                wglGetProcAddress( "glGetShaderiv" );
// //     glGetShaderInfoLog          = (PFNGLGETSHADERINFOLOGPROC)           wglGetProcAddress( "glGetShaderInfoLog" );
// //     glCreateProgram             = (PFNGLCREATEPROGRAMPROC)              wglGetProcAddress( "glCreateProgram" );
// //     glAttachShader              = (PFNGLATTACHSHADERPROC)               wglGetProcAddress( "glAttachShader" );
// //     glDetachShader              = (PFNGLDETACHSHADERPROC)               wglGetProcAddress( "glDetachShader" );
// //     glLinkProgram               = (PFNGLLINKPROGRAMPROC)                wglGetProcAddress( "glLinkProgram" );
// //     glDeleteProgram             = (PFNGLDELETEPROGRAMPROC)              wglGetProcAddress( "glDeleteProgram" );
// //     glProgramParameteri         = (PFNGLPROGRAMPARAMETERIPROC)          wglGetProcAddress( "glProgramParameteri" );
// //     glGetProgramiv              = (PFNGLGETPROGRAMIVPROC)               wglGetProcAddress( "glGetProgramiv" );
// //     glGetProgramInfoLog         = (PFNGLGETPROGRAMINFOLOGPROC)          wglGetProcAddress( "glGetProgramInfoLog" );
// //     glUseProgram                = (PFNGLUSEPROGRAMPROC)                 wglGetProcAddress( "glUseProgram" );
// //     glGetUniformLocation        = (PFNGLGETUNIFORMLOCATIONPROC)         wglGetProcAddress( "glGetUniformLocation" );
// //     glGetUniformIndices         = (PFNGLGETUNIFORMINDICESPROC)          wglGetProcAddress( "glGetUniformIndices" );
// //     glGetActiveUniform          = (PFNGLGETACTIVEUNIFORMPROC)           wglGetProcAddress( "glGetActiveUniform" );
// //     glBindAttribLocation        = (PFNGLBINDATTRIBLOCATIONPROC)         wglGetProcAddress( "glBindAttribLocation" );
// //     glUniform1fv                = (PFNGLUNIFORM1FVPROC)                 wglGetProcAddress( "glUniform1fv" );
// //     glUniformMatrix4fv          = (PFNGLUNIFORMMATRIX4FVPROC)           wglGetProcAddress( "glUniformMatrix4fv" );
// //     glUniform3fv                = (PFNGLUNIFORM3FVPROC)                 wglGetProcAddress( "glUniform3fv" );
// //     glUniform4fv                = (PFNGLUNIFORM4FVPROC)                 wglGetProcAddress( "glUniform4fv" );
// //     glUniform1iv                = (PFNGLUNIFORM1IVPROC)                 wglGetProcAddress( "glUniform1iv" );
// //     glGenVertexArrays           = (PFNGLGENVERTEXARRAYSPROC)            wglGetProcAddress( "glGenVertexArrays" );
// //     glDeleteVertexArrays        = (PFNGLDELETEVERTEXARRAYSPROC)         wglGetProcAddress( "glDeleteVertexArrays" );
// //     glBindVertexArray           = (PFNGLBINDVERTEXARRAYPROC)            wglGetProcAddress( "glBindVertexArray" );
// //     glGetAttribLocation         = (PFNGLGETATTRIBLOCATIONPROC)          wglGetProcAddress( "glGetAttribLocation" );
// //     glEnableVertexAttribArray   = (PFNGLENABLEVERTEXATTRIBARRAYPROC)    wglGetProcAddress( "glEnableVertexAttribArray" );
// //     glVertexAttribPointer       = (PFNGLVERTEXATTRIBPOINTERPROC)        wglGetProcAddress( "glVertexAttribPointer" );
// //     glGetProgramBinary          = (PFNGLGETPROGRAMBINARYPROC)           wglGetProcAddress( "glGetProgramBinary" );
// //     glProgramBinary             = (PFNGLPROGRAMBINARYPROC)              wglGetProcAddress( "glProgramBinary" );
// //     glGenSamplers               = (PFNGLGENSAMPLERSPROC)                wglGetProcAddress( "glGenSamplers" );
// //     glSamplerParameteri         = (PFNGLSAMPLERPARAMETERIPROC)          wglGetProcAddress( "glSamplerParameteri" );
// //     glActiveTexture             = (PFNGLACTIVETEXTUREPROC)              wglGetProcAddress( "glActiveTexture" );
// //     glBindSampler               = (PFNGLBINDSAMPLERPROC)                wglGetProcAddress( "glBindSampler" );
// //     glDeleteSamplers            = (PFNGLDELETESAMPLERSPROC)             wglGetProcAddress( "glDeleteSamplers" );
// }
// 
// ////===========================================================================================
// ///===========================================================================================
// // Public Destruction
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::Shutdown()
// {
//     DestroyOpenGLRenderingContext();
// }
// 
// ////===========================================================================================
// ///===========================================================================================
// // Protected Destruction
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::DestroyOpenGLRenderingContext()
// {
//     wglDeleteContext( m_renderingContext );
// }
// 
// ////===========================================================================================
// ///===========================================================================================
// // Non-Static Accessors/Queries
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool OGLRenderer::CheckCompatibility()
// {
//     // Versions 
//     char* glVersion = (char*)glGetString( GL_VERSION );
//     ConsolePrintf( "GL_VERSION: %s \n", glVersion );
//     DeveloperConsole::WriteLine( "GL_VERSION: " + std::string( glVersion ), INFO_TEXT_COLOR );
// 
//     GLint major;
//     GLint minor;
//     glGetIntegerv( GL_MAJOR_VERSION, &major );
//     glGetIntegerv( GL_MINOR_VERSION, &minor );
// 
//     ConsolePrintf( "GL_VERSION (int): %i.%i \n", major, minor );
//     DeveloperConsole::WriteLine( "GL_VERSION (int): " + std::to_string( major ) + "." + std::to_string( minor ), INFO_TEXT_COLOR );
// 
//     char* shaderVersion = (char*)glGetString( GL_SHADING_LANGUAGE_VERSION );
//     ConsolePrintf( "GL_SHADING_LANGUAGE_VERSION: %s \n", shaderVersion );
//     DeveloperConsole::WriteLine( "GL_SHADING_LANGUAGE_VERSION: " + std::string( shaderVersion ), INFO_TEXT_COLOR );
//     GLCheckError();
// 
//     // Some more debug information that you could do in an early check
//     GLint max_attribs;
//     glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &max_attribs );
//     ConsolePrintf( "Max Vertex Attributes: %i \n", max_attribs );
// 
// 
//     GLboolean shaderCompilerAvailable = GL_FALSE;
//     glGetBooleanv( GL_SHADER_COMPILER, &shaderCompilerAvailable );
//     if (shaderCompilerAvailable != GL_TRUE)
//     {
//         MessageBoxAf( MBT_OK, MBI_ERROR, "GLSL ERROR", "OpenGL driver does not have a shader compiler available." );
//         __debugbreak();
//     }
// 
//     // For saving binaries
//     GLint binaryFormats;
//     glGetIntegerv( GL_NUM_PROGRAM_BINARY_FORMATS, &binaryFormats );
//     if (binaryFormats <= 0)
//     {
//         MessageBoxReturnValue userChoice = MessageBoxAf( MBT_ABORTRETRYIGNORE, MBI_EXCLAMATION, "GL BINARY FORMATS ERROR", "There are no available binary formats for OpenGL." );
//         if (userChoice == MBR_RETRY || userChoice == MBR_ABORT)
//             __debugbreak();
//     }
//     return (shaderCompilerAvailable == GL_TRUE);
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// float OGLRenderer::GetDisplayWidth() const
// {
//     return m_displaySize.x;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// float OGLRenderer::GetDisplayHeight() const
// {
//     return m_displaySize.y;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// const Vector2 OGLRenderer::GetDisplaySize() const
// {
//     return m_displaySize;
// }
// 
// ////===========================================================================================
// ///===========================================================================================
// // Static Accessors/Queries
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::GLCheckErrorFL( const char* fileName, const int& lineNum )
// {
// #ifdef OPENGL_CHECK_ERRORS
//     GLenum error = glGetError();
//     if (error != GL_NO_ERROR)
//     {
//         ConsolePrintf( "%s(%d): OpenGL error code %i \n", fileName, lineNum - 1, error );
//         DeveloperConsole::WriteLine( (std::string( fileName ) + "(" + std::to_string( lineNum - 1 ) + "):" + "OpenGL error code " + std::to_string( error )), ERROR_TEXT_COLOR );
//     }
// #endif
// }
// 
// ////===========================================================================================
// ///===========================================================================================
// // Public GPU Program Functions
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// GLuint OGLRenderer::LoadProgram( const char* vertexFilePath, const char* fragmentFilePath )
// {
//     GLuint programID = glCreateProgram();
//     if (programID == NULL)
//         return NULL;
// 
//     GLuint vertexShader = LoadShader( vertexFilePath, GL_VERTEX_SHADER );
//     GLCheckError();
// 
//     GLuint fragmentShader = LoadShader( fragmentFilePath, GL_FRAGMENT_SHADER );
//     GLCheckError();
// 
//     if (!LinkProgram( programID, vertexShader, fragmentShader ))
//     {
//         glDeleteProgram( programID );
//         programID = NULL;
//     }
// 
//     // no longer needed no matter what
//     FreeShader( vertexShader );
//     GLCheckError();
// 
//     FreeShader( fragmentShader );
//     GLCheckError();
// 
//     return programID;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::DestroyProgram( const unsigned int& programID )
// {
//     if (programID != NULL) {
//         glDeleteProgram( programID );
//     }
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::BindProgram( const unsigned int& programID )
// {
//     glUseProgram( programID );
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool OGLRenderer::ProgramBindAttribute( const GLuint& programID, char const *name, const GLint& count, const GLenum& type, const GLboolean& normalize, const GLsizei& stride, const GLsizei& offset )
// {
//     GLint loc = glGetAttribLocation( programID, name );
//     GLCheckError();
//     if (loc < 0) {
//         // does not exist
//         return false;
//     }
// 
//     // there is no glGetAttribIndices
//     // so if you want to error check, you can go 
//     // use glGetProgramiv( id, GL_ACTIVE_ATTRIBUTES, &max_attributes );
//     // and glGetActiveAttrib(...) to enumerator all the attributes, and compare when you find it.
// 
//     // test to see if this even matters - that is, having more enabled than are used.
//     glEnableVertexAttribArray( loc );
//     GLCheckError();
// 
//     glVertexAttribPointer( loc, count, type, normalize, stride, (GLvoid*)offset );
//     GLCheckError();
// 
//     return true;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool OGLRenderer::ProgramBindInt32( const GLuint& programID, const char* name, const int& val )
// {
//     GLint loc = ProgramFindAndCheck( programID, name, GL_INT );
//     if (loc < 0) {
//         // didn't exist
//         return false;
//     }
// 
//     glUniform1iv( loc, 1, (GLint*)&val );
//     return true;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool OGLRenderer::ProgramBindFloat( const GLuint& programID, const char* name, const float& val )
// {
//     GLint loc = ProgramFindAndCheck( programID, name, GL_FLOAT );
//     if (loc < 0) {
//         // didn't exist
//         return false;
//     }
// 
//     glUniform1fv( loc, 1, (GLfloat*)&val );
//     return true;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool OGLRenderer::ProgramBindVec3( const GLuint& programID, const char* name, const Vector3& vec3 )
// {
//     GLint loc = ProgramFindAndCheck( programID, name, GL_FLOAT_VEC3 );
//     if (loc < 0) {
//         // didn't exist
//         return false;
//     }
// 
// 
//     glUniform3fv( loc, 1, (GLfloat*)vec3.GetAsFloatArray() );
//     return true;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool OGLRenderer::ProgramBindVec4f( const GLuint& programID, const char* name, const Vector4f& vec4 )
// {
//     GLint loc = ProgramFindAndCheck( programID, name, GL_FLOAT_VEC4 );
//     if (loc < 0) {
//         // didn't exist
//         return false;
//     }
// 
//     glUniform4fv( loc, 1, (GLfloat*)vec4.GetAsPrimitiveTypeArray() );
//     return true;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool OGLRenderer::ProgramBindMatrixf( const GLuint& programID, const char* name, const Matrix4f& mat )
// {
//     GLint loc = ProgramFindAndCheck( programID, name, GL_FLOAT_MAT4 );
//     if (loc < 0) {
//         // didn't exist
//         return false;
//     }
// 
//     glUniform4fv( loc, 1, (GLfloat*)mat.GetAsPrimitiveTypeArray());
//     return true;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool OGLRenderer::ProgramBindSamplerIndex( const GLuint& programID, const char* name, GLint idx )
// {
//     GLint loc = ProgramFindAndCheck( programID, name, GL_SAMPLER_2D );
//     if (loc < 0) {
//         // didn't exist
//         return false;
//     }
// 
//     glUniform1iv( loc, 1, (GLint*)&idx );
//     return true;
// }
// 
// ////===========================================================================================
// ///===========================================================================================
// // Protected GPU Program Functions
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool OGLRenderer::LinkProgram( const GLuint& programID, const GLuint& vertexShader, const GLuint& fragmentShader )
// {
//     if ((vertexShader == NULL) || (fragmentShader == NULL) || (programID == NULL))
//         return false;
// 
//     // At this point, you can still do operations on shader
//     // and they'll take - just everything has to be set before
//     // you do glLinkProgram or glProgramBinary
//     glAttachShader( programID, vertexShader );
//     glAttachShader( programID, fragmentShader );
//     GLCheckError();
// 
// 
//     // this is just for show - this has to be set before linking for this to work
//     bool save_binary = false;
//     if (true == save_binary)
//     {
//         // if you want to save off this binary - make sure to set this before linking
//         glProgramParameteri( programID, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE );
//         GLCheckError();
//     }
// 
//     // This is also where you'd set options if you want the program to be seperable
//     // We won't be going over how to do that now, but it basically let's you have 
//     // different programs bound to different stages (pipelining)
// 
//     glLinkProgram( programID );
//     GLCheckError();
// 
//     // Only need to be attached if they're 
//     glDetachShader( programID, vertexShader );
//     glDetachShader( programID, fragmentShader );
//     GLCheckError();
// 
//     if (!CheckProgramStatus( programID ))
//         return false;
// 
//     return true;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool OGLRenderer::CheckProgramStatus( const GLuint& programID )
// {
//     GLint linkStatus;
//     glGetProgramiv( programID, GL_LINK_STATUS, &linkStatus );
// 
//     if (linkStatus == GL_FALSE)
//     {
//         GLint logSize = 0;
//         glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &logSize );
// 
//         if (logSize > 0)
//         {
//             char* buffer = new char[logSize];
//             glGetProgramInfoLog( programID, sizeof( buffer ), &logSize, (GLchar*)buffer );
// 
//             ConsolePrintf( "Failed to link program: \n %s", buffer );
//             delete buffer;
//         }
//         else
//             GLCheckError();
// 
//         return false;
//     }
//     return true;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// GLint OGLRenderer::ProgramFindAndCheck( const GLuint & programID, const char* name, const GLenum& type )
// {
//     GLint loc = glGetUniformLocation( programID, name );
//     if (loc < 0) {
//         // didn't exist
//         MessageBoxAf( MBT_OK, MBI_ERROR, "Program Bind Error", "The uniform \"%s\" does not exist in the program.", name );
//         __debugbreak();
//         return loc;
//     }
// 
//     // If you want, you can make sure it actually is the type you want
//     GLuint index;
//     glGetUniformIndices( programID, 1, &name, &index );
//     if (index >= 0) {
//         GLint count;
//         GLenum check_type;
//         glGetActiveUniform( programID, index, 0, NULL, &count, &check_type, NULL );
// 
//         if (type != check_type)
//         {
//             MessageBoxAf( MBT_OK, MBI_ERROR, "Program Bind Error", "The type of \"%s\" is not of the type you are trying to bind.", name );
//             __debugbreak();
//         }
// 
//         else if (type == check_type) {
//             return loc;
//         }
//     }
// 
//     return -1;
// }
// 
// ////===========================================================================================
// ///===========================================================================================
// // Protected Shader File Functions
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// GLuint OGLRenderer::LoadShader( const char* fileName, const GLenum& shaderType )
// {
//     //size_t sourceSize = 0;
//     //GLint len = (GLint)(sourceSize);
//     char* source = nullptr;
// 
//     GLuint shaderID = glCreateShader( shaderType );
//     if (shaderID == NULL)
//         return NULL;
// 
//     MessageBoxReturnValue userChoice = MBR_NULL;
//     bool shaderLoadedSuccess = false;
// 
//     while (!shaderLoadedSuccess)
//     {
//         if (userChoice == MBR_CANCEL)
//             return NULL;
// 
//         else if (userChoice == MBR_RETRY || userChoice == MBR_NULL)
//         {
//             //sourceSize = 0;
// 
//             source = LoadTextFileToNewBuffer( fileName );
//             if (source == nullptr)
//                 return NULL;
// 
//             // can take multiple strings - internally these will all be 
//             // concatenated end-to-end
//             
//             //len = (GLint)(sourceSize);
//             glShaderSource( shaderID, 1, &source, NULL );
//             delete source;
// 
//             glCompileShader( shaderID );
//             GLCheckError();
//         }
// 
//         userChoice = MBR_NULL;
//         shaderLoadedSuccess = CheckShaderStatus( shaderID, fileName, userChoice );
//     }
// 
//     if (shaderLoadedSuccess)
//         return shaderID;
// 
//     else
//         return NULL;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::FreeShader( const GLuint& shaderID )
// {
//     if (shaderID != NULL)
//         glDeleteShader( shaderID );
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool OGLRenderer::CheckShaderStatus( const GLuint& shaderID, const char* fileName, MessageBoxReturnValue& out_userChoice )
// {
//     GLint status;
//     glGetShaderiv( shaderID, GL_COMPILE_STATUS, &status );
//     if (status == GL_FALSE)
//     {
//         // failed to compile
//         GLint logSize = 0;
//         glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &logSize );
// 
//         if (logSize > 0)
//         {
//             char* log = new char[logSize];
// 
//             glGetShaderInfoLog( shaderID, logSize, &logSize, log );
// 
//             out_userChoice = FormatAndPrintShaderErrorLog( fileName, log );
// 
//             delete(log);
//         }
//         else
//         {
//             // Failed, unknown reason - shouldn't happen
//             // make note of this method
//             GLCheckError();
//         }
// 
//         return false;
//     }
// 
//     return true;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// MessageBoxReturnValue OGLRenderer::FormatAndPrintShaderErrorLog( const char* fileName, const char* log )
// {
//     // filename(lineno):<text>\n...
//     // will let you double click
// 
//     char *fileExt;
//     char fullPath[256]; //dummy buffer
//     GetFullPathNameA( fileName, 256, fullPath, &fileExt );
// 
// 
//     std::string logStr( log );
//     std::string line = "";
// 
//     std::string errorList;
// 
//     while (logStr.find( '\n' ) != std::string::npos)
//     {
//         line = logStr.substr( 1, logStr.find( '\n' ) );
// 
//         ConsolePrintf( "%s", fullPath );
// 
//         std::string lineNum = line.substr( 0, line.find( ')' ) + 1 );
//         ConsolePrintf( "%s", lineNum.c_str() );
// 
//         line.erase( 0, line.find( ')' ) + 1 );
// 
//         ConsolePrintf( "%s", line.c_str() );
// 
//         errorList += fileName + lineNum + line + "\n";
// 
//         logStr.erase( 0, logStr.find( '\n' ) + 1 );
//     }
// 
//     MessageBoxReturnValue returnVal = MessageBoxAf( MBT_RETRYCANCEL, MBI_EXCLAMATION, "GLSL errors", "%s", errorList.c_str() );
//     return returnVal;
// 
// }
// 
// ////===========================================================================================
// ///===========================================================================================
// // Public Vertex Buffer Functions
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// GLuint OGLRenderer::CreateBuffer()
// {
//     unsigned int bufferID;
//     glGenBuffers( 1, &bufferID );
//     GLCheckError();
// 
//     if (bufferID == NULL)
//     {
//         MessageBoxAf( MBT_OK, MBI_ERROR, "Buffer ERROR", "Could not create vertex buffer." );
//         __debugbreak();
//         return NULL;
//     }
// 
//     return bufferID;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// GLuint OGLRenderer::CreateBuffer( const void* buffer, const size_t& size, const GLenum& bufferUsage /* = GL_DYNAMIC_DRAW */ )
// {
//     GLuint bufferID = CreateBuffer();
// 
//     if (buffer != nullptr) {
//         BindBufferData( bufferID, buffer, size, bufferUsage );
//     }
// 
//     return bufferID;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::BindBufferData( const GLuint& bufferID, const void* data, const size_t& size, const GLenum& bufferUsage /* = GL_DYNAMIC_DRAW */ )
// {
//     glBindBuffer( GL_ARRAY_BUFFER, bufferID );
//     GLCheckError();
// 
//     // talk about sub buffer binding, and usage hints here
//     // most common ones are GL_STATIC_DRAW, and GL_DYNAMIC_DRAW
//     glBufferData( GL_ARRAY_BUFFER, size, data, bufferUsage );
//     GLCheckError();
// 
//     // There's also glBufferSubData, for when you know it's large enough and only
//     // want to edit part of it.  Will not be going over that yet
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::DestroyBuffer( const GLuint& bufferID )
// {
//     glDeleteBuffers( 1, &bufferID );
// }
// 
// ////===========================================================================================
// ///===========================================================================================
// // Public Vertex Array Functions
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// GLuint OGLRenderer::CreateVAO()
// {
//     unsigned int vaoID;
//     glGenVertexArrays( 1, &vaoID );
//     GLCheckError();
// 
//     if (vaoID == NULL) {
//         MessageBoxAf( MBT_OK, MBI_ERROR, "VAO ERROR", "Could not create VAO." );
//         __debugbreak();
//         return NULL;
//     }
// 
//     return vaoID;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// unsigned int OGLRenderer::CreateVAO( const GLuint& programID, const GLuint& bufferID, const VertexLayout& layout )
// {
//     GLuint vaoID = CreateVAO();
// 
//     BindVAOBuffer( programID, vaoID, bufferID, layout );
// 
//     return vaoID;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::BindVAO( const GLuint& vaoID )
// {
//     glBindVertexArray( vaoID );
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::BindVAOBuffer( const GLuint& programID, const GLuint& vaoID, const GLuint& bufferID, const VertexLayout& layout )
// {
//     glBindVertexArray( vaoID );
//     glBindBuffer( GL_ARRAY_BUFFER, bufferID );
// 
//     for (VertexLayout::const_iterator layoutIter = layout.begin(); layoutIter != layout.end(); ++layoutIter)
//     {
//         const VertexAttributeProperties& properites = *layoutIter;
// 
//         ProgramBindAttribute( programID, properites.name, properites.count, properites.type, properites.normalize, properites.stride, properites.offset );
//     }
// 
//     GLCheckError();
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::DestroyVAO( const GLuint& vaoID )
// {
//     if (vaoID != NULL) 
//     {
//         glDeleteVertexArrays( 1, &vaoID );
//     }
// }\
// 
// ////===========================================================================================
// ///===========================================================================================
// // Public Sampler Functions
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// GLuint OGLRenderer::CreateSampler( const GLenum& minFilter /* = GL_NEAREST_MIPMAP_LINEAR */, const GLenum& magFilter /* = GL_LINEAR */, const GLenum& uWrap /* = GL_REPEAT */, const GLenum& vWrap /* = GL_REPEAT */ )
// {
//     GLuint samplerID;
//     glGenSamplers( 1, &samplerID );
// 
//     if (samplerID == NULL) {
//         MessageBoxAf( MBT_OK, MBI_ERROR, "Sampler ERROR", "Could not create Sampler." );
//         __debugbreak();
//         return NULL;
//     }
// 
//     glSamplerParameteri( samplerID, GL_TEXTURE_MIN_FILTER, minFilter );			// Default: GL_NEAREST_MIPMAP_LINEAR
//     glSamplerParameteri( samplerID, GL_TEXTURE_MAG_FILTER, magFilter );			// Default: GL_LINEAR
//     glSamplerParameteri( samplerID, GL_TEXTURE_WRAP_S, uWrap );					// Default: GL_REPEAT
//     glSamplerParameteri( samplerID, GL_TEXTURE_WRAP_T, vWrap );					// Default: GL_REPEAT
// 
//     return samplerID;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::BindSampler( const GLuint& unit, const GLuint& samplerID )
// {
//     glBindSampler( unit, samplerID );
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::DestroySampler( const GLuint& samplerID )
// {
//     glDeleteSamplers( 1, &samplerID );
// }
// 
// ////===========================================================================================
// ///===========================================================================================
// // Public Texture Functions
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::SetActiveTexture( const GLenum& textureIndex )
// {
//     glActiveTexture( textureIndex );
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::BindTexture( const GLenum& target, const GLuint& texture )
// {
//     glBindTexture( target, texture );
// }
// 
// ////===========================================================================================
// ///===========================================================================================
// //  Pulbic Update Functions
// ///===========================================================================================
// ////===========================================================================================
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::SwapBuffers()
// {
//     ::SwapBuffers( m_deviceContext );
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void OGLRenderer::Clear( const Rgba& color /* = Rgba::BLACK */, const float& depth /* = 1.0f */ )
// {
//     glClearColor( color.Rf(), color.Gf(), color.Bf(), 1.0f );
//     glClearDepth( depth );
//     glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
// }