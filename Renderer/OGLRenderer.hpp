// //=================================================================================
// // OGLRenderer.hpp
// // Author: Tyler George
// // Date  : June 24, 2015
// //=================================================================================
// 
// #pragma once
// 
// #ifndef __included_OGLRenderer__
// #define __included_OGLRenderer__
// 
// #define OGL_CHECK_ERRORS
// #define GLCheckError() GLCheckErrorFL( __FILE__, __LINE__ )
// 
// //---------------------------------------------------------------------------------
// // Includes
// //---------------------------------------------------------------------------------
// #include "Engine/Renderer/OpenGL/glcorearb.h" // includes windows.h
// #include "Engine/Math/Vector2.hpp"
// #include "Engine/Math/Matrix4.hpp"
// #include "Engine/Utilities/Console.hpp"
// #include "Engine/Renderer/Vertex3D.hpp"
// 
// //---------------------------------------------------------------------------------
// // Forward Declarations
// //---------------------------------------------------------------------------------
// 
// 
// //---------------------------------------------------------------------------------
// // OpenGL typedefs
// //---------------------------------------------------------------------------------
// typedef void GLvoid;
// typedef unsigned int GLenum;
// typedef float GLfloat;
// typedef int GLint;
// typedef int GLsizei;
// typedef unsigned int GLbitfield;
// typedef double GLdouble;
// typedef unsigned int GLuint;
// typedef unsigned char GLboolean;
// typedef unsigned char GLubyte;
// typedef float GLclampf;
// typedef double GLclampd;
// typedef char GLchar;
// typedef short GLshort;
// typedef signed char GLbyte;
// typedef unsigned short GLushort;
// typedef unsigned short GLhalf;
// 
// //---------------------------------------------------------------------------------
// // Enumerations
// //---------------------------------------------------------------------------------
// // enum DepthTestMode
// // {
// //     DT_ENABLED,
// //     DT_DISABLED,
// //     DT_DUAL
// // };
// 
// 
// class OGLRenderer
// {
// 
// public:
//     ///---------------------------------------------------------------------------------
//     /// Constructors/Destructors
//     ///---------------------------------------------------------------------------------
//     OGLRenderer();
//     ~OGLRenderer();
// 
//     ///---------------------------------------------------------------------------------
//     /// Public Initialization
//     ///---------------------------------------------------------------------------------
//     void Startup( HWND windowHandle );
// 
//     ///---------------------------------------------------------------------------------
//     /// Public Destruction
//     ///---------------------------------------------------------------------------------
//     void Shutdown();
// 
//     ///---------------------------------------------------------------------------------
//     /// Non-Static Accessors/Queries
//     ///---------------------------------------------------------------------------------
//     bool CheckCompatibility();
//     float GetDisplayWidth() const;
//     float GetDisplayHeight() const;
//     const Vector2 GetDisplaySize() const;
// 
//     ///---------------------------------------------------------------------------------
//     /// Static Accessors/Queries
//     ///---------------------------------------------------------------------------------
//     static void GLCheckErrorFL( const char* fileName, const int& lineNum );
// 
//     ///---------------------------------------------------------------------------------
//     /// Public GPU Program Functions
//     ///---------------------------------------------------------------------------------
//     GLuint LoadProgram( const char* vertexFilePath, const char* fragmentFilePath );
//     void DestroyProgram( const GLuint& programID );
//     void BindProgram( const GLuint& programID );
//     bool ProgramBindAttribute( const GLuint& programID, char const *name, const GLint& count, const GLenum& type, const GLboolean& normalize, const GLsizei& stride, const GLsizei& offset );
//     bool ProgramBindInt32( const GLuint& programID, const char* name, const int& val );
//     bool ProgramBindFloat( const GLuint& programID, const char* name, const float& val );
//     bool ProgramBindVec3( const GLuint& programID, const char* name, const Vector3& vec3 );
//     bool ProgramBindVec4f( const GLuint& programID, const char* name, const Vector4f& vec4 );
//     bool ProgramBindMatrixf( const GLuint& programID, const char* name, const Matrix4f& mat );
//     bool ProgramBindSamplerIndex( const GLuint& programID, const char* name, GLint idx );
// 
//     ///---------------------------------------------------------------------------------
//     /// Public Vertex Buffer Functions
//     ///---------------------------------------------------------------------------------
//     GLuint CreateBuffer();
//     GLuint CreateBuffer( const void* buffer, const size_t& size, const GLenum& bufferUsage = GL_DYNAMIC_DRAW );
//     void BindBufferData( const GLuint& bufferID, const void* data, const size_t& size, const GLenum& bufferUsage = GL_DYNAMIC_DRAW );
//     void DestroyBuffer( const GLuint& bufferID );
// 
//     ///---------------------------------------------------------------------------------
//     /// Public Vertex Array Functions
//     ///---------------------------------------------------------------------------------
//     GLuint CreateVAO();
//     GLuint CreateVAO( const GLuint& programID, const GLuint& bufferID, const VertexLayout& layout );
//     void BindVAO( const GLuint& vaoID );
//     void BindVAOBuffer( const GLuint& programID, const GLuint& vaoID, const GLuint& bufferID, const VertexLayout& layout );
//     void DestroyVAO( const GLuint& vaoID );
// 
//     ///---------------------------------------------------------------------------------
//     /// Public Sampler Functions
//     ///---------------------------------------------------------------------------------
//     GLuint CreateSampler( const GLenum& minFilter = GL_NEAREST_MIPMAP_LINEAR, const GLenum& magFilter = GL_LINEAR, const GLenum& uWrap = GL_REPEAT, const GLenum& vWrap = GL_REPEAT );
//     void BindSampler( const GLuint& unit, const GLuint& samplerID );
//     void DestroySampler( const GLuint& samplerID );
// 
//     ///---------------------------------------------------------------------------------
//     /// Public Texture Functions
//     ///---------------------------------------------------------------------------------
//     void SetActiveTexture( const GLenum& textureIndex );
//     void BindTexture( const GLenum& target, const GLuint& texture );
// 
//     ///---------------------------------------------------------------------------------
//     /// Public Update Functions
//     ///---------------------------------------------------------------------------------
//     void SwapBuffers();
//     void Clear( const Rgba& color = Rgba::BLACK, const float& depth = 1.0f );
// 
// protected:
//     ///---------------------------------------------------------------------------------
//     /// Protected Initialization
//     ///---------------------------------------------------------------------------------
//     void CreateOpenGLRenderingContext();
//     void InitializeAdvancedOpenGLFunctions();
// 
//     ///---------------------------------------------------------------------------------
//     /// Protected Destruction
//     ///---------------------------------------------------------------------------------
//     void DestroyOpenGLRenderingContext();
// 
//     ///---------------------------------------------------------------------------------
//     /// Protected GPU Program Functions
//     ///---------------------------------------------------------------------------------
//     bool LinkProgram( const GLuint& programID, const GLuint& vertexShader, const GLuint& fragmentShader );
//     bool CheckProgramStatus( const GLuint& programID );
//     GLint ProgramFindAndCheck( const GLuint& programID, const char* name, const GLenum& type );
// 
// 
//     ///---------------------------------------------------------------------------------
//     /// Protected Shader File Functions
//     ///---------------------------------------------------------------------------------
//     GLuint LoadShader( const char* fileName, const GLenum& shaderType );
//     void FreeShader( const GLuint& shaderID );
//     bool CheckShaderStatus( const GLuint& shaderID, const char* fileName, MessageBoxReturnValue& out_userChoice );
//     MessageBoxReturnValue FormatAndPrintShaderErrorLog( const char* fileName, const char* log );
// 
// 
// private:
//     ///---------------------------------------------------------------------------------
//     /// Private Member Variables
//     ///---------------------------------------------------------------------------------
//     HWND m_windowHandle;
//     HDC m_deviceContext;
//     HGLRC m_renderingContext;
// 
//     Vector2 m_displaySize;
// 
//     Matrix4f m_perspectiveProj;
//     Matrix4f m_orthographicProj;
//     Matrix4f m_currentView;
// };
// 
// ///---------------------------------------------------------------------------------
// /// Externs
// ///---------------------------------------------------------------------------------
// // extern PFNGLGENBUFFERSPROC		            glGenBuffers;
// // extern PFNGLDELETEBUFFERSPROC	            glDeleteBuffers;
// // extern PFNGLBINDBUFFERPROC		            glBindBuffer;
// // extern PFNGLBUFFERDATAPROC		            glBufferData;
// // extern PFNGLGENERATEMIPMAPPROC	            glGenerateMipmap;
// // extern PFNGLCREATESHADERPROC                glCreateShader;
// // extern PFNGLCOMPILESHADERPROC               glCompileShader;
// // extern PFNGLSHADERSOURCEPROC                glShaderSource;
// // extern PFNGLDELETESHADERPROC                glDeleteShader;
// // extern PFNGLGETSHADERIVPROC                 glGetShaderiv;
// // extern PFNGLGETSHADERINFOLOGPROC            glGetShaderInfoLog;
// // extern PFNGLCREATEPROGRAMPROC               glCreateProgram;
// // extern PFNGLATTACHSHADERPROC                glAttachShader;
// // extern PFNGLDETACHSHADERPROC                glDetachShader;
// // extern PFNGLLINKPROGRAMPROC                 glLinkProgram;
// // extern PFNGLDELETEPROGRAMPROC               glDeleteProgram;
// // extern PFNGLPROGRAMPARAMETERIPROC           glProgramParameteri;
// // extern PFNGLGETPROGRAMIVPROC                glGetProgramiv;
// // extern PFNGLGETPROGRAMINFOLOGPROC           glGetProgramInfoLog;
// // extern PFNGLUSEPROGRAMPROC                  glUseProgram;
// // extern PFNGLGETUNIFORMLOCATIONPROC          glGetUniformLocation;
// // extern PFNGLGETUNIFORMINDICESPROC           glGetUniformIndices;
// // extern PFNGLGETACTIVEUNIFORMPROC            glGetActiveUniform;
// // extern PFNGLBINDATTRIBLOCATIONPROC          glBindAttribLocation;
// // extern PFNGLUNIFORM1FVPROC                  glUniform1fv;
// // extern PFNGLUNIFORMMATRIX4FVPROC            glUniformMatrix4fv;
// // extern PFNGLUNIFORM3FVPROC                  glUniform3fv;
// // extern PFNGLUNIFORM4FVPROC                  glUniform4fv;
// // extern PFNGLUNIFORM1IVPROC                  glUniform1iv;
// // extern PFNGLGENVERTEXARRAYSPROC             glGenVertexArrays;
// // extern PFNGLDELETEVERTEXARRAYSPROC          glDeleteVertexArrays;
// // extern PFNGLBINDVERTEXARRAYPROC             glBindVertexArray;
// // extern PFNGLGETATTRIBLOCATIONPROC           glGetAttribLocation;
// // extern PFNGLENABLEVERTEXATTRIBARRAYPROC     glEnableVertexAttribArray;
// // extern PFNGLVERTEXATTRIBPOINTERPROC         glVertexAttribPointer;
// // extern PFNGLGETPROGRAMBINARYPROC            glGetProgramBinary;
// // extern PFNGLPROGRAMBINARYPROC               glProgramBinary;
// // extern PFNGLGENSAMPLERSPROC                 glGenSamplers;
// // extern PFNGLSAMPLERPARAMETERIPROC           glSamplerParameteri;
// // extern PFNGLACTIVETEXTUREPROC               glActiveTexture;
// // extern PFNGLBINDSAMPLERPROC                 glBindSampler;
// // extern PFNGLDELETESAMPLERSPROC              glDeleteSamplers;
// 
// #endif