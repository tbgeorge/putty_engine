//=================================================================================
// UIElement.hpp
// Author: Tyler George
// Date  : November 8, 2015
//=================================================================================

#pragma once

#ifndef __included_UIElement__
#define __included_UIElement__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Math/AABB2D.hpp"


///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
/// Enums
///---------------------------------------------------------------------------------
enum Anchor
{
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    CENTER_LEFT,
    CENTER_CENTER,
    CENTER_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
};

////===========================================================================================
///===========================================================================================
// UIElement Class
///===========================================================================================
////===========================================================================================
class UIElement
{
public:
	///---------------------------------------------------------------------------------
	/// Constructors/Destructors
	///---------------------------------------------------------------------------------
    UIElement( OpenGLRenderer* renderer, UIElement* parent = nullptr );
    UIElement( OpenGLRenderer* renderer, const Vector2& size, UIElement* parent = nullptr, Anchor anchor = BOTTOM_LEFT, const Vector2& offset = Vector2::ZERO );
    ~UIElement();

	///---------------------------------------------------------------------------------
	/// Initialization
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Accessors/Queries
	///---------------------------------------------------------------------------------
    Vector2 GetSize() const { return m_size; }

	///---------------------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------------------
    void CalcVerts();
    Vector2 CalculatePosition();

    void SetOffset( const Vector2& offset ) { m_offset = offset; }
    void AddOffset( const Vector2& additionalOffset ) { m_offset += additionalOffset; }
    void SetAnchor( Anchor alignment ); 

	///---------------------------------------------------------------------------------
	/// Update
	///---------------------------------------------------------------------------------
    virtual void ProcessInput( InputSystem* inputSystem ) { UNUSED( inputSystem ); }
    virtual void Update( bool debugModeEnabled ) { UNUSED( debugModeEnabled ); }

	///---------------------------------------------------------------------------------
	/// Render
	///---------------------------------------------------------------------------------
    virtual void Render( bool debugModeEnabled ) { UNUSED( debugModeEnabled ); }

	///---------------------------------------------------------------------------------
	/// Public Member Variables
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Static Variables
	///---------------------------------------------------------------------------------


protected:
	///---------------------------------------------------------------------------------
	/// Private Functions
	///---------------------------------------------------------------------------------

	///---------------------------------------------------------------------------------
	/// Private Member Variables
	///---------------------------------------------------------------------------------
    OpenGLRenderer* m_renderer;
    UIElement* m_parent;

    AABB2D m_bounds;
    float m_zVal;
    Vector2 m_size;
    Anchor m_anchor;
    Vector2 m_offset;

    PUC_Vertexes m_verts;

};

///---------------------------------------------------------------------------------
/// Inline function implementations
///---------------------------------------------------------------------------------

#endif