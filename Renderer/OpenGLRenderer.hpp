//=================================================================================
// OpenGLRenderer.hpp
// Author: Tyler George
// Date  : January 15, 2015
//=================================================================================

#pragma once

#ifndef __included_OpenGLRenderer__
#define __included_OpenGLRenderer__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define OPENGL_CHECK_ERRORS
#define GLCheckError() GLCheckErrorFL( __FILE__, __LINE__ )

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Renderer/OpenGL/glext.h"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Renderer/Camera3D.hpp"
#include "Engine/Utilities/Rgba.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/Vertex3D.hpp"
#include "Engine/Utilities/Console.hpp"
#include "Engine/Renderer/Lighting/PuttyLight.hpp"
class FontRenderer;
class GraphicsDebugger;

const int MAX_NUM_LIGHTS = 16;

class DeveloperConsole;

enum DepthTestMode
{
    DT_ENABLED,
    DT_DISABLED,
    DT_DUAL
};

struct RenderState
{
    bool culling;
    bool depthTest;
    bool blend;
    GLenum blendSrc;
    GLenum blendDst;
    bool filling;
    GLenum fillFace;
    GLenum fillMode;

    RenderState( bool cull, bool dt, bool blend, bool fill )
        : culling( cull ), depthTest( dt ), blend( blend ), filling( fill ) {}

    void SetBlendMode( GLenum src, GLenum dst )
    {
        blendSrc = src;
        blendDst = dst;
    }

    void SetFillMode( GLenum face, GLenum mode )
    {
        fillFace = face;
        fillMode = mode;
    }
};


class OpenGLRenderer
{
public:
	///---------------------------------------------------------------------------------
	/// Constructor/Destructor
	///---------------------------------------------------------------------------------
	OpenGLRenderer();
	~OpenGLRenderer();

    //===========================================================================================
    //===========================================================================================
    // Modern Stuff
    //===========================================================================================
    //===========================================================================================
	///---------------------------------------------------------------------------------
	/// Initialization
	///---------------------------------------------------------------------------------
	void InitializeAdvancedOpenGLFunctions();
    void InitializeDefaultTextures();

    unsigned int GetDefaultSampler();

    void BindRenderState( RenderState rs );
    int GetUniformLocation( unsigned int programID, const std::string& name );
    bool CheckCompatability();
    static unsigned int LoadProgram( const char* vertexFilePath, const char* fragmentFilePath );
    static void GLCheckErrorFL( const char* fileName, int lineNum );

    void CopyToBuffer( GLuint bufferID, void const *data, size_t size );
    unsigned int CreateBuffer( void const *buffer, size_t size );
    unsigned int CreateBuffer();
    void BindBufferData( GLenum target, const GLuint& bufferID, const void* data, const size_t& size, const GLenum& bufferUsage = GL_DYNAMIC_DRAW );

    void DestroyBuffer( unsigned int buffer );
    GLuint CreateVAO();
    void BindVAOBuffer( const GLuint& programID, const GLuint& vaoID, const GLuint& bufferID, const VertexLayout& layout );

    unsigned int CreateVAO( unsigned int programID, unsigned int bufferID, VertexLayout layout );
    void DestroyVAO( unsigned int vaoID );
    unsigned int CreateSampler( GLenum min_filter, GLenum mag_filter, GLenum u_wrap, GLenum v_wrap );
    void DestroySampler( unsigned int samplerID );
    void DrawWithTexture( unsigned int programID, unsigned int vaoID, unsigned int numVertexes, const Matrix4f& projection, const Matrix4f& model, unsigned int samplerID, Texture* texture = nullptr );
    void DrawWithTexture( unsigned int programID, VertexLayout layout, unsigned int numVertexes, const Matrix4f& projection, const Matrix4f& model, unsigned int samplerID, Texture* texture = nullptr );
    void DrawWithTextureOrtho( unsigned int programID, VertexLayout layout, unsigned int numVertexes, unsigned int samplerID, Texture* texture = nullptr );

    void Draw( unsigned int programID, unsigned int vaoID, unsigned int numVertexes, const Matrix4f& projection, const Matrix4f& model, const unsigned int& drawMode );


    void DrawColoredQuad( unsigned int programID, const Vector2& mins, const Vector2& maxs, const float& z, const Rgba& faceColor );
    void DrawVertexes( unsigned int programID, const PUC_Vertexes& vertexes, GLenum drawMode );
    void DrawVertexesOrtho( unsigned int programID, const PUC_Vertexes& vertexes, GLenum drawMode, const Matrix4f& model = Matrix4f::CreateIdentity() );

        
    void DrawTexturedSphere( unsigned int programID, unsigned int samplerID, int debugInt, const Matrix4f& model, float radius, unsigned int slices, unsigned int stacks, Texture* texture = nullptr );
    void DrawTexturedCuboid( unsigned int programID, unsigned int samplerID, const Matrix4f& model, float xSize, float ySize, float zSize, Texture* texture = nullptr );

    
    static GLuint LoadShader( const char* fileName, GLenum shaderType );
    static void FreeShader( GLuint shaderID );
    static bool CheckShaderStatus( GLuint shaderID, const char* fileName, MessageBoxReturnValue& out_userChoice );

    static bool LinkProgram( GLuint programID, GLuint vertexShader, GLuint fragmentShader );
    void DestroyProgram( GLuint program );
    static bool CheckProgramStatus( GLuint programID );
    bool ProgramBindMatrix( GLuint progID, const char* name, const Matrix4f& mat );
    bool ProgramBindAttribute( GLuint programID, char const *name, GLint count, GLenum type, GLboolean normalize, GLsizei stride, GLsizei offset );
    void BindProgram( GLuint progID );
    void UnbindProgram();
    bool ProgramBindSamplerIndex( GLuint programID, const char* name, GLint idx );
    bool ProgramBindFloat( GLuint programID, const char* name, const float& val );
    bool ProgramBindInt32( GLuint programID, const char* name, const int& val );
    bool ProgramBindVec2( GLuint programID, const char* name, const Vector2& val );
    bool ProgramBindVec3( GLuint programID, const char* name, const Vector3& val );
    bool ProgramBindVec4f( GLuint programID, const char* name, const Vector4f& val );
    bool ProgramBindLights( GLuint programID, const char* name, const Lights& lights );
    GLint ProgramFindAndCheck( GLuint programID, const char* name, GLenum type );

    void BindVertexArray( GLuint vaoID );
    void BindIndexArray( GLuint iboID );

    void SetActiveTexture( GLenum textureIndex );
//     void BindTexture( GLenum target, GLuint texture );
    void BindSampler( GLuint unit, GLuint sampler );
    void DrawArrays( GLenum mode, GLint first, GLsizei count );
    void DrawElements( GLenum mode, GLsizei count );

    unsigned int CreateFrameBuffer();
    unsigned int CreateFrameBuffer( int width, int height, GLuint colorTexture, GLuint depthTexture = NULL );

    void FrameBufferAttachTexture2D( GLenum attachment, GLuint texture );
    void FrameBufferAttachRenderBuffer( GLenum attachment, GLuint renderbufferID );
    void FramebufferBind( GLuint fboID, GLuint width, GLuint height );
    void FramebufferUnbind();
    void FramebufferCopyToBack( GLuint fboID, GLuint width, GLuint height );
    bool CheckFramebufferStatus();

    unsigned int CreateRenderBuffer( GLenum internalFormat, GLsizei width, GLsizei height );

    static MessageBoxReturnValue FormatAndPrintLog( const char* fileName, const char* log );

    PUC_Vertexes GetSpherePUCVertexArray( float radius, unsigned int slices, unsigned int stacks );
    PUC_Vertexes GetCuboidPUCVertexArray( float xSize, float ySize, float zSize );

public:
	///---------------------------------------------------------------------------------
	/// Accessors
	///---------------------------------------------------------------------------------
	float GetDisplayWidth()	const	{ return m_displayWidth; }
	float GetDisplayHeight() const	{ return m_displayHeight; }

    static unsigned int CreateOrGetShader( const std::string& shaderName );
    void* ReadPixels();

	///---------------------------------------------------------------------------------
	/// Public Methods
	///---------------------------------------------------------------------------------
	void Startup( HWND windowHandle );

//     void SetDevConsole( DeveloperConsole* devConsole );
	void Shutdown();
	void SwapBuffers();
	void Clear( const Rgba& color = Rgba::BLACK );
	void PushMatrix();
	void PopMatrix();
	void Begin( GLenum drawMode );
	void End();
	void Enable( GLenum mode );
	void Disable( GLenum mode );
	void Vertex2f( const Vector2& vertex );
	void Vertex3f( const Vector3& vertex );
	void TexCoord2f( const Vector2& texCoord );
	void LoadIdentity();
	void LoadMatrixf( const GLfloat* matrixPtr );
	void BlendFunct( GLenum sfactor, GLenum dfactor );
	void BindTexture( GLenum target, GLuint texture );
	void SetColor( const Rgba& color );
	void SetLineSize( float lineSize );
	void SetPointSize( float pointSize );
	void SetModelViewMatrixMode();
	void SetProjectionMatrixMode();
	void SetModelViewRotation( float angleDegrees, float amountAroundX, float amountAroundY, float amountAroundZ );
	void SetOrthographicView( float width = 0.0f, float height = 0.0f );
	void SetPerspective( float fieldOfViewDegreesVertical, float aspectRatio, float nearDepth, float farDepth );
	void SetModelViewTranslation( float x, float y, float z = 0.0f);
	void SetModelViewScale( float xScale, float yScale, float zScale = 1.0f );

	void ApplyCameraTransformZUp( const Camera3D& camera );

    Matrix4f CalculateProjectionPerspectiveMatrix( float fov, float ratio, float nearZ, float farZ, bool setMatrix = false );
    Matrix4f CalculateOrthographicMatrix( float width, float height, float farZ, float nearZ, bool setMatrix = false );
    Matrix4f CalculateViewMatrix( const Vector3& cameraPos, const Vector3& lookAtTarget, bool setMatrix = false );
    Matrix4f CalculateViewMatrix( const Camera3D& camera, bool setMatrix = false );

    Matrix4f& GetViewMatrix() { return m_view; }
    Matrix4f& GetPerspectiveMatrix() { return m_perspectiveProj; }
    Matrix4f& GetOrthoMatrix() { return m_orthoProj; }
    FontRenderer* GetFontRenderer() { return m_fontRenderer; }
    GraphicsDebugger* GetGraphicsDebugger() { return m_graphicsDebugger; }

	///---------------------------------------------------------------------------------
	/// Draw Methods
	///---------------------------------------------------------------------------------
	
    ///---------------------------------------------------------------------------------
    /// glBegin
    ///---------------------------------------------------------------------------------
	//2D
	void DrawTriangle2D( const Vector2& v0, const Vector2& v1, const Vector2& v2, const Rgba& color = Rgba() );
	void DrawPolygon2D( const Vector2s& vertices, const Rgba& color = Rgba(), float lineSize = 1.0f );
	void DrawFilledPolygon2D( const Vector2s& vertices, const Rgba& color = Rgba() );
	void DrawPoints2D( const Vector2s& points, const Rgba& color = Rgba(), float pointSize = 3.0f );
	void DrawPoint2D( const Vector2& point, const Rgba& color = Rgba(), float pointSize = 3.0f );
	void DrawQuad2D( const Vector2s& vertices, const Rgba& color = Rgba() );
	void DrawLineSegment2D( const Vector2& startPoint, const Vector2& endPoint, const Rgba& color = Rgba() );
	void DrawTexturedQuad2D( const Texture& texture, const Vector2s& vertices, const Vector2s& textureCoordinates, const Rgba& tint = Rgba() );

	//3D
	void DrawTriangle3D( const Vector3& v0, const Vector3& v1, const Vector3& v2, const Rgba& color = Rgba() );
	void DrawPolygon3D( const Vector3s& vertices, const Rgba& color = Rgba(), float lineSize = 1.0f );
	void DrawFilledPolygon3D( const Vector3s& vertices, const Rgba& color = Rgba() );
	void DrawPoints3D( const Vector3s& points, const Rgba& color = Rgba(), float pointSize = 3.0f );
	void DrawPoint3D( const Vector3& point, const Rgba& color = Rgba(), float pointSize = 3.0f );
	void DrawQuad3D( const Vector3s& vertices, const Rgba& color = Rgba() );
	void DrawLineSegment3D( const Vector3& startPoint, const Vector3& endPoint, const Rgba& color = Rgba() );
	void DrawTexturedQuad3D( const Texture& texture, const Vector3s& vertices, const Vector2s& textureCoordinates, const Rgba& tint = Rgba() );
	void DrawOriginAxes( const float& size );
	void DrawAxes( unsigned int programID, float length, Matrix4f proj, Matrix4f view );

	//Geometries
	void DrawWireTeapot( GLdouble size );
	void DrawSolidTeapot( GLdouble size );
	void DrawWireCube( GLdouble size );
	void DrawSolidCube( GLdouble size );
	void DrawWireSphere( GLdouble radius, GLint slices, GLint stacks );
	void DrawSolidSphere( GLdouble radius, GLint slices, GLint stacks );
	void DrawWireCone( GLdouble base, GLdouble height, GLint slices, GLint stacks );
	void DrawSolidCone( GLdouble base, GLdouble height, GLint slices, GLint stacks );
	void DrawWireTorus( GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings );
	void DrawSolidTorus( GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings );
	void DrawWireDodecahedron();
	void DrawSolidDodecahedron();
	void DrawWireOctahedron();
	void DrawSolidOctahedron();
	void DrawWireTetrahedron();
	void DrawSolidTetrahedron();
	void DrawWireIcosahedron();
	void DrawSolidIcosahedron();
	void DrawWireSierpinskiSponge( int num_levels, GLdouble offset[3], GLdouble scale );
	void DrawSolidSierpinskiSponge( int num_levels, GLdouble offset[3], GLdouble scale );
   

protected:

	///---------------------------------------------------------------------------------
	/// Protected Methods
	///---------------------------------------------------------------------------------
	void CreateOpenGLRenderingContext();
	void DestroyOpenGLRenderingContext();
	
private:

    ///---------------------------------------------------------------------------------
    ///
    ///---------------------------------------------------------------------------------
    ///---------------------------------------------------------------------------------
    /// VBOs
    ///---------------------------------------------------------------------------------
//     void DrawPoint( const Vector3& position, const Rgba& color = Rgba() );
//     void DrawLineSegment( const Vector3& startPos, const Vector3& endPos, const Rgba& startColor = Rgba(), const Rgba& endColor = Rgba() );
//     void DrawArrow( const Vector3& startPos, const Vector3& endPos, const Rgba& color = Rgba() );
//     void DrawWireAABB( const Vector3& mins, const Vector3& maxs, const Rgba& lineColor = Rgba() );
//     void DrawAABB( const Vector3& mins, const Vector3& maxs, const Rgba& lineColor = Rgba(), const Rgba& faceColor = Rgba() );
//     void DrawWireUVSphere( const Vector3& center, const float& radius, const int& slices, const int& stacks, const Rgba& color = Rgba() );
//     void DrawUVSphere( const Vector3& center, const float& radius, const int& slices, const int& stacks, const Rgba& color = Rgba() );


	///---------------------------------------------------------------------------------
	/// Private member variables
	///---------------------------------------------------------------------------------
	HWND	m_windowHandle;
	HDC		m_deviceContext;
	HGLRC	m_renderingContext;

	float	m_displayWidth;
	float	m_displayHeight;

    Matrix4f m_perspectiveProj;
    Matrix4f m_view;
    Matrix4f m_orthoProj;

    FontRenderer* m_fontRenderer;
    GraphicsDebugger* m_graphicsDebugger;

    unsigned int m_defaultShaderID;
    unsigned int m_currentlyBoundShaderID;

    static std::map< std::string, unsigned int > s_shaderMap;
    //static DeveloperConsole* m_devConsole;

};

// Advanced OpenGL Function pointers
extern PFNGLGENBUFFERSPROC		            glGenBuffers;
extern PFNGLDELETEBUFFERSPROC	            glDeleteBuffers;
extern PFNGLBINDBUFFERPROC		            glBindBuffer;
extern PFNGLBUFFERDATAPROC		            glBufferData;
extern PFNGLBINDBUFFERBASEPROC              glBindBufferBase;
extern PFNGLGENERATEMIPMAPPROC	            glGenerateMipmap;
extern PFNGLCREATESHADERPROC                glCreateShader;
extern PFNGLCOMPILESHADERPROC               glCompileShader;
extern PFNGLSHADERSOURCEPROC                glShaderSource;
extern PFNGLDELETESHADERPROC                glDeleteShader;
extern PFNGLGETSHADERIVPROC                 glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC            glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC               glCreateProgram;
extern PFNGLATTACHSHADERPROC                glAttachShader;
extern PFNGLDETACHSHADERPROC                glDetachShader;
extern PFNGLLINKPROGRAMPROC                 glLinkProgram;
extern PFNGLDELETEPROGRAMPROC               glDeleteProgram;
extern PFNGLPROGRAMPARAMETERIPROC           glProgramParameteri;
extern PFNGLGETPROGRAMIVPROC                glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC           glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC                  glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC          glGetUniformLocation;
extern PFNGLGETUNIFORMINDICESPROC           glGetUniformIndices;
extern PFNGLGETUNIFORMBLOCKINDEXPROC        glGetUniformBlockIndex;
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC     glGetActiveUniformBlockiv;
extern PFNGLGETACTIVEUNIFORMSIVPROC         glGetActiveUniformsiv;
extern PFNGLGETACTIVEUNIFORMPROC            glGetActiveUniform;
extern PFNGLBINDATTRIBLOCATIONPROC          glBindAttribLocation;
extern PFNGLUNIFORM1FVPROC                  glUniform1fv;
extern PFNGLUNIFORMMATRIX4FVPROC            glUniformMatrix4fv;
extern PFNGLUNIFORM2FVPROC                  glUniform2fv;
extern PFNGLUNIFORM3FVPROC                  glUniform3fv;
extern PFNGLUNIFORM4FVPROC                  glUniform4fv;
extern PFNGLUNIFORM1IVPROC                  glUniform1iv;
extern PFNGLGENVERTEXARRAYSPROC             glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC          glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC             glBindVertexArray;
extern PFNGLGETATTRIBLOCATIONPROC           glGetAttribLocation;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC     glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC         glVertexAttribPointer;
extern PFNGLGETPROGRAMBINARYPROC            glGetProgramBinary;
extern PFNGLPROGRAMBINARYPROC               glProgramBinary;
extern PFNGLGENSAMPLERSPROC                 glGenSamplers;
extern PFNGLSAMPLERPARAMETERIPROC           glSamplerParameteri;
extern PFNGLACTIVETEXTUREPROC               glActiveTexture;
extern PFNGLBINDSAMPLERPROC                 glBindSampler;
extern PFNGLDELETESAMPLERSPROC              glDeleteSamplers;
extern PFNGLBINDFRAMEBUFFERPROC             glBindFramebuffer;
extern PFNGLBLITFRAMEBUFFERPROC             glBlitFramebuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC      glCheckFramebufferStatus;
extern PFNGLDELETEFRAMEBUFFERSPROC          glDeleteFramebuffers;
extern PFNGLFRAMEBUFFERTEXTUREPROC          glFramebufferTexture;
extern PFNGLGENFRAMEBUFFERSPROC             glGenFramebuffers;
extern PFNGLGENRENDERBUFFERSPROC            glGenRenderbuffers;
extern PFNGLBINDRENDERBUFFERPROC            glBindRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEPROC         glRenderbufferStorage;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC        glFramebufferTexture2D;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC     glFramebufferRenderbuffer;


#endif
