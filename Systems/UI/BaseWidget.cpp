//=================================================================================
// BaseWidget.cpp
// Author: Tyler George
// Date  : April 25, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Systems/UI/BaseWidget.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Utilities/Rgba.hpp"
#include "Engine/Utilities/Error.hpp"
#include "../../Utilities/XMLHelper.hpp"
#include "UISystem.hpp"

////===========================================================================================
///===========================================================================================
// class BaseWidget
///===========================================================================================
////===========================================================================================

////---------------------------------------------------------------------------------
///
// Constructors/Destructors
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
BaseWidget::BaseWidget( const std::string& widgetName, const WidgetPropertyLayer& instanceProperties )
    : m_currentState( WidgetState_IDLE )
    , m_type( WidgetType_NUM_WIDGET_TYPES )
    , m_parent( nullptr )
    , m_instanceProperties( instanceProperties )
    , m_name( widgetName )
{
    // set code defaults
    m_codeProperties.m_defaults.Set( "Offset", Vector2( 100.0f, 100.0f ) );
    m_codeProperties.m_defaults.Set( "Size", Vector2( 200.0f, 200.0f ) );
    m_codeProperties.m_defaults.Set( "BGColor", Rgba::WHITE );
    m_codeProperties.m_defaults.Set( "EdgeColor", Rgba::BLACK );
    m_codeProperties.m_defaults.Set( "EdgeThickness", 3.0f );
    m_codeProperties.m_defaults.Set( "Opacity", 1.0f );


    // initialize content verts
    Rgba bgColor;
    GetPropertyByName( "BGColor", bgColor );
    AABB2D bounds = GetWidgetBounds();

    Vector2 mins = bounds.m_mins;
    Vector2 maxs = bounds.m_maxs;

    m_contentVerts.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, 1.0f ), Vector2( 0.0f, 1.0f ), bgColor ) );
    m_contentVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, 1.0f ), Vector2( 1.0f, 1.0f ), bgColor ) );
    m_contentVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, 1.0f ), Vector2( 1.0f, 0.0f ), bgColor ) );

    m_contentVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, 1.0f ), Vector2( 1.0f, 0.0f ), bgColor ) );
    m_contentVerts.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, 1.0f ), Vector2( 0.0f, 0.0f ), bgColor ) );
    m_contentVerts.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, 1.0f ), Vector2( 0.0f, 1.0f ), bgColor ) );

    // initialize edge verts
    Rgba edgeColor;
    GetPropertyByName( "EdgeColor", edgeColor );
    float edgeThickness;
    GetPropertyByName( "EdgeThickness", edgeThickness );

    // bottom border
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( mins.x - edgeThickness, mins.y - edgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                     // 0
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( maxs.x + edgeThickness, mins.y - edgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );            // 1
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                                            // 2

    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                                            // 2
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                                                     // 3
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( mins.x - edgeThickness, mins.y - edgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                     // 0

    // left border
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( mins.x - edgeThickness, mins.y - edgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                     // 0
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( mins.x, mins.y, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                                                     // 3
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                                            // 4

    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                                            // 4
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( mins.x - edgeThickness, maxs.y + edgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );            // 5
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( mins.x - edgeThickness, mins.y - edgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                     // 0

    // top border
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                                            // 4
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                                   // 6
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( maxs.x + edgeThickness, maxs.y + edgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );   // 7

    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( maxs.x + edgeThickness, maxs.y + edgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );   // 7
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( mins.x - edgeThickness, maxs.y + edgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );            // 5
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( mins.x, maxs.y, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                                            // 4

    // right border
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                                   // 6
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, mins.y, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                                            // 2
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( maxs.x + edgeThickness, mins.y - edgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );            // 1

    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( maxs.x + edgeThickness, mins.y - edgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );            // 1
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( maxs.x + edgeThickness, maxs.y + edgeThickness, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );   // 7
    m_edgeVerts.push_back( Vertex3D_PUC( Vector3( maxs.x, maxs.y, 1.0f ), Vector2( 0.0f, 0.0f ), edgeColor ) );                                   // 6





}

////---------------------------------------------------------------------------------
///
// Accessors/Queries
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
AABB2D BaseWidget::GetWidgetBounds() const
{
    AABB2D myBounds( Vector2::ZERO, Vector2::ZERO );

    if (m_parent)
        m_parent->GetWidgetBoundsForChild( *this, myBounds );
    else
    {
        Vector2 myOffset;
        Vector2 mySize;
        GetPropertyByName( "Offset", myOffset );
        GetPropertyByName( "Size", mySize );

        myBounds += myOffset;
        myBounds.m_maxs += mySize;
    }

    return myBounds;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BaseWidget::GetWidgetBoundsForChild( const BaseWidget& child, AABB2D& out_childBounds ) const
{
    AABB2D myBounds( Vector2::ZERO, Vector2::ZERO );

    if (m_parent)
        m_parent->GetWidgetBoundsForChild( *this, myBounds );
    else
        myBounds = GetWidgetBounds();

    Vector2 childOffset;
    Vector2 childSize;
    child.GetPropertyByName( "Offset", childOffset );
    child.GetPropertyByName( "Size", childSize );

    out_childBounds.m_mins = myBounds.m_mins + childOffset;
    out_childBounds.m_maxs = out_childBounds.m_mins + childSize;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float BaseWidget::GetWidgetOpacity() const
{
    float opacity;
    GetPropertyByName( "Opacity", opacity );

    if (m_parent)
        return opacity * m_parent->GetWidgetOpacity();
    else
        return opacity;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BaseWidget::GetPropertiesFromXML( const XMLNode& node, NamedProperties& in_out_properties )
{
    XMLNode offsetNode = node.getChildNode( "Offset" );
    if (!offsetNode.isEmpty())
    {
        Vector2 offset = GetVector2Property( offsetNode, "value" );
        in_out_properties.Set( "Offset", offset );

    }

    XMLNode sizeNode = node.getChildNode( "Size" );
    if (!sizeNode.isEmpty())
    {
        Vector2 size = GetVector2Property( sizeNode, "value", Vector2( -1.0, -1.0 ) );
//         if (size == Vector2( -1.0, -1.0 ))
//         {
//             Strings size = GetStringsProperty( sizeNode, "value", ", ", Strings() );
//         }

        in_out_properties.Set( "Size", size );

    }

    XMLNode bgColorNode = node.getChildNode( "BGColor" );
    if (!bgColorNode.isEmpty())
    {
        Rgba bgColor = GetRgbaProperty( bgColorNode, "value", Rgba::WHITE );
        in_out_properties.Set( "BGColor", bgColor );

    }

    XMLNode edgeColorNode = node.getChildNode( "EdgeColor" );
    if (!edgeColorNode.isEmpty())
    {
        Rgba edgeColor = GetRgbaProperty( edgeColorNode, "value", Rgba::BLACK );
        in_out_properties.Set( "EdgeColor", edgeColor );

    }

    XMLNode edgeThicknessNode = node.getChildNode( "EdgeThickness" );
    if (!edgeThicknessNode.isEmpty())
    {
        float edgeThickness = GetFloatProperty( edgeThicknessNode, "value", 5.0f );
        in_out_properties.Set( "EdgeThickness", edgeThickness );

    }

    XMLNode opacityNode = node.getChildNode( "Opacity" );
    if (!opacityNode.isEmpty())
    {
        float opacity = GetFloatProperty( opacityNode, "value", 1.0f );
        in_out_properties.Set( "Opacity", opacity );

    }
}

////---------------------------------------------------------------------------------
///
// Mutators
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BaseWidget::SetStyle( const std::string& styleName )
{
    UIStyle* style = UISystem::GetStyleByName( styleName );

    if( style )
        m_styleProperties = style->m_styleByWidgetType[m_type];
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BaseWidget::SetState( WidgetState state )
{
    m_currentState = state;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BaseWidget::SetParent( BaseWidget* parent )
{
    m_parent = parent;
}

///---------------------------------------------------------------------------------
/// returns false if unable to add child, true if adding child was successful
///---------------------------------------------------------------------------------
bool BaseWidget::AddChild( BaseWidget* child )
{
    return false;
}

////---------------------------------------------------------------------------------
///
// Update
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BaseWidget::ProcessInput( InputSystem* inputSystem )
{
    if (m_currentState == WidgetState_DISABLED || m_currentState == WidgetState_HIDDEN)
        return;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BaseWidget::Update()
{
    if ( m_currentState == WidgetState_HIDDEN)
        return;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BaseWidget::UpdateVertsFromBounds()
{
    // initialize content verts
    AABB2D bounds = GetWidgetBounds();
    Vector2 mins = bounds.m_mins;
    Vector2 maxs = bounds.m_maxs;

    m_contentVerts[0].position = Vector3( mins.x, mins.y, 1.0f );
    m_contentVerts[1].position = Vector3( maxs.x, mins.y, 1.0f );
    m_contentVerts[2].position = Vector3( maxs.x, maxs.y, 1.0f );
    
    m_contentVerts[3].position = Vector3( maxs.x, maxs.y, 1.0f );
    m_contentVerts[4].position = Vector3( mins.x, maxs.y, 1.0f );
    m_contentVerts[5].position = Vector3( mins.x, mins.y, 1.0f );

    // initialize edge verts
    float edgeThickness;
    GetPropertyByName( "EdgeThickness", edgeThickness );

    // bottom border
    m_edgeVerts[0].position = Vector3( mins.x - edgeThickness, mins.y - edgeThickness, 1.0f );                     // 0
    m_edgeVerts[1].position = Vector3( maxs.x + edgeThickness, mins.y - edgeThickness, 1.0f );            // 1
    m_edgeVerts[2].position = Vector3( maxs.x, mins.y, 1.0f );                                            // 2
    m_edgeVerts[3].position = Vector3( maxs.x, mins.y, 1.0f );                                            // 2
    m_edgeVerts[4].position = Vector3( mins.x, mins.y, 1.0f );                                                     // 3
    m_edgeVerts[5].position = Vector3( mins.x - edgeThickness, mins.y - edgeThickness, 1.0f );                     // 0
    m_edgeVerts[6].position = Vector3( mins.x - edgeThickness, mins.y - edgeThickness, 1.0f );                     // 0
    m_edgeVerts[7].position = Vector3( mins.x, mins.y, 1.0f );                                                     // 3
    m_edgeVerts[8].position = Vector3( mins.x, maxs.y, 1.0f );                                            // 4
    m_edgeVerts[9].position = Vector3( mins.x, maxs.y, 1.0f );                                            // 4
    m_edgeVerts[10].position = Vector3( mins.x - edgeThickness, maxs.y + edgeThickness, 1.0f );            // 5
    m_edgeVerts[11].position = Vector3( mins.x - edgeThickness, mins.y - edgeThickness, 1.0f );                     // 0
    m_edgeVerts[12].position = Vector3( mins.x, maxs.y, 1.0f );                                            // 4
    m_edgeVerts[13].position = Vector3( maxs.x, maxs.y, 1.0f );                                   // 6
    m_edgeVerts[14].position = Vector3( maxs.x + edgeThickness, maxs.y + edgeThickness, 1.0f );   // 7
    m_edgeVerts[15].position = Vector3( maxs.x + edgeThickness, maxs.y + edgeThickness, 1.0f );   // 7
    m_edgeVerts[16].position = Vector3( mins.x - edgeThickness, maxs.y + edgeThickness, 1.0f );            // 5
    m_edgeVerts[17].position = Vector3( mins.x, maxs.y, 1.0f );                                            // 4
    m_edgeVerts[18].position = Vector3( maxs.x, maxs.y, 1.0f );                                   // 6
    m_edgeVerts[19].position = Vector3( maxs.x, mins.y, 1.0f );                                            // 2
    m_edgeVerts[20].position = Vector3( maxs.x + edgeThickness, mins.y - edgeThickness, 1.0f );            // 1
    m_edgeVerts[21].position = Vector3( maxs.x + edgeThickness, mins.y - edgeThickness, 1.0f );            // 1
    m_edgeVerts[22].position = Vector3( maxs.x + edgeThickness, maxs.y + edgeThickness, 1.0f );   // 7
    m_edgeVerts[23].position = Vector3( maxs.x, maxs.y, 1.0f );                                   // 6
}

////---------------------------------------------------------------------------------
///
// Render
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void BaseWidget::Render( OpenGLRenderer* renderer )
{
    if (!renderer)
        return;
    
    if (m_currentState == WidgetState_HIDDEN)
        return;
}