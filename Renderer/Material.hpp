//=================================================================================
// Material.hpp
// Author: Tyler George
// Date  : June 29, 2015
//=================================================================================

#pragma once

#ifndef __included_Material__
#define __included_Material__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <string>
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Renderer/Uniforms.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"


///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class Material
{
public:
	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
    Material( OpenGLRenderer* renderer, const unsigned int& shaderID, const RenderState& renderstate );
    ~Material();

	///---------------------------------------------------------------------------------
	/// Initialization
	///---------------------------------------------------------------------------------
	
	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
    UniformBase* FindUniform( const int& bindPoint );

    unsigned int GetShaderID() { return m_shaderID; }
    RenderState GetRenderState() { return m_renderState; }

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
    void SetFloatUniform( const std::string& name, const float& val );
    void SetIntUniform( const std::string& name, const int& val );
    void SetVec2Uniform( const std::string& name, const Vector2& vec2 );
    void SetVec3Uniform( const std::string& name, const Vector3& vec3 );
    void SetVec4fUniform( const std::string& name, const Vector4f& vec4 );
    void SetMatrix4fUniform( const std::string& name, const Matrix4f& mat );
    void SetTextureUniform( const std::string& name, Texture* tex, const int& sampler = -1 );

    void BindAllUniforms();

    void SetRenderState( const RenderState& renderState ) { m_renderState = renderState; }

	///---------------------------------------------------------------------------------
	/// Update
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Render
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Public Member Variables
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Static Variables
	///---------------------------------------------------------------------------------


private:
	///---------------------------------------------------------------------------------
	/// Private Functions
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Private Member Variables
	///---------------------------------------------------------------------------------
    OpenGLRenderer* m_renderer;
    unsigned int m_shaderID;
    Uniforms m_uniforms;
    RenderState m_renderState;
    int m_currentTexIndex;
};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

#endif