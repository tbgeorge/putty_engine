//=================================================================================
// XMLHelper.cpp
// Author: Tyler George
// Date  : September 10, 2015
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include <shlwapi.h>

#include "Engine/Utilities/XMLHelper.hpp"
#include "Engine/Utilities/Utilities.hpp"

////===========================================================================================
///===========================================================================================
// Get Helpers
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::string	GetXMLNodeName( const XMLNode& xmlNode )
{
    return std::string( xmlNode.getName() );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::string	GetStringProperty( const XMLNode& node, const std::string& propertyName, const std::string& defaultValue, bool returnAsLowerCase )
{
    const char* prop = node.getAttribute( propertyName.c_str() );
    if (!prop)
    {
        if ( returnAsLowerCase )
            return ConvertToLowerCase( defaultValue );
        return defaultValue;
    }

    std::string strProp( prop );

    if (returnAsLowerCase)
        return ConvertToLowerCase( strProp );
    return strProp;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Strings GetStringsProperty( const XMLNode& node, const std::string& propertyName, const std::string& delimiters, const Strings& defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    std::string stringsProp( prop );
    Strings strings;
    Tokenize( stringsProp, strings, delimiters );

    return strings;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
char GetCharacterProperty( const XMLNode& node, const std::string& propertyName, char defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    return prop[0];
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
int GetIntProperty( const XMLNode& node, const std::string& propertyName, int defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    if (IsInt( std::string( prop ) ))
    {
        int intProp = atoi( prop );
        return intProp;
    }
    else
        return defaultValue;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
unsigned int GetUnsignedIntProperty( const XMLNode& node, const std::string& propertyName, unsigned int defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;


    if (IsUnsignedInt( std::string( prop ) ))
    {
        unsigned int uIntProp = strtoul( prop, NULL, 10 );
        return uIntProp;
    }
    else
        return defaultValue;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float GetFloatProperty( const XMLNode& node, const std::string& propertyName, float defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    if (IsFloat( std::string( prop ) ) )
    {
        float fltProp = strtof( prop, NULL );
        return fltProp;
    }
    else
        return defaultValue;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
double GetDoubleProperty( const XMLNode& node, const std::string& propertyName, double defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    if (IsFloat( std::string( prop ) ))
    {
        double dblProp = strtod( prop, NULL );
        return dblProp;
    }
    else
        return defaultValue;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool GetBooleanProperty( const XMLNode& node, const std::string& propertyName, bool defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    std::string propStr( prop );

    if (ConvertToLowerCase( propStr ) == "true")
        return true;
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Rgba GetRgbaProperty( const XMLNode& node, const std::string& propertyName, const Rgba& defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    std::string strProp( prop );
    Strings tokens;
    Tokenize( strProp, tokens, ",");

    if (tokens.size() == 1)
    {
        std::string str = ConvertToLowerCase( tokens[0] );
        if (str == "red")
            return Rgba::RED;

        else if (str == "blue")
            return Rgba::BLUE;

        else if (str == "green")
            return Rgba::GREEN;

        else if (str == "black")
            return Rgba::BLACK;

        else if (str == "white")
            return Rgba::WHITE;

        else if (str == "aqua")
            return Rgba::AQUA;

        else if (str == "orange")
            return Rgba::ORANGE;

        else if (str == "yellow")
            return Rgba::YELLOW;

        else if (str == "grey")
            return Rgba::GREY;

        else if (str == "magenta")
            return Rgba::MAGENTA;

        else if (IsHexInt( str ))
        {
            unsigned int colorVal = strtoul( str.c_str(), nullptr, 0 );
            return Rgba( colorVal );
        }
    }

    else if (tokens.size() == 3)
    {
        if (IsUnsignedInt( tokens[0] ) && IsUnsignedInt( tokens[1] ) && IsUnsignedInt( tokens[2] ) )
        {
            unsigned int r = strtoul( tokens[0].c_str(), nullptr, 10 );
            unsigned int g = strtoul( tokens[1].c_str(), nullptr, 10 );
            unsigned int b = strtoul( tokens[2].c_str(), nullptr, 10 );
            unsigned int a = 255;

            return Rgba( r, g, b, a );
        }

        if (IsFloat( tokens[0] ) && IsFloat( tokens[1] ) && IsFloat( tokens[2] ))
        {
            float r = strtof( tokens[0].c_str(), nullptr );
            float g = strtof( tokens[1].c_str(), nullptr );
            float b = strtof( tokens[2].c_str(), nullptr );
            float a = 1.0f;

            return Rgba( r, g, b, a );
        }
    }

    else if (tokens.size() == 4)
    {
        if (IsUnsignedInt( tokens[0] ) && IsUnsignedInt( tokens[1] ) && IsUnsignedInt( tokens[2] ) && IsUnsignedInt( tokens[3] ) )
        {
            unsigned int r = strtoul( tokens[0].c_str(), nullptr, 10 );
            unsigned int g = strtoul( tokens[1].c_str(), nullptr, 10 );
            unsigned int b = strtoul( tokens[2].c_str(), nullptr, 10 );
            unsigned int a = strtoul( tokens[3].c_str(), nullptr, 10 );

            return Rgba( r, g, b, a );
        }

        if (IsFloat( tokens[0] ) && IsFloat( tokens[1] ) && IsFloat( tokens[2] ) && IsFloat( tokens[3] ))
        {
            float r = strtof( tokens[0].c_str(), nullptr );
            float g = strtof( tokens[1].c_str(), nullptr );
            float b = strtof( tokens[2].c_str(), nullptr );
            float a = strtof( tokens[3].c_str(), nullptr );

            return Rgba( r, g, b, a );
        }
    }

    return defaultValue;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Vector2	GetVector2Property( const XMLNode& node, const std::string& propertyName, const Vector2& defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    std::string strProp( prop );
    Strings tokens;
    Tokenize( strProp, tokens, "," );

    if (tokens.size() == 2)
    {
        if (IsFloat( tokens[0] ) && IsFloat( tokens[1] ))
        {
            float x = strtof( tokens[0].c_str(), nullptr );
            float y = strtof( tokens[1].c_str(), nullptr );

            return Vector2( x, y );
        }
        else return defaultValue;
    }
    else
        return defaultValue;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
Vector3 GetVector3Property( const XMLNode& node, const std::string& propertyName, const Vector3& defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    std::string strProp( prop );
    Strings tokens;
    Tokenize( strProp, tokens, "," );

    if (tokens.size() == 3)
    {
        if (IsFloat( tokens[0] ) && IsFloat( tokens[1] ) && IsFloat( tokens[2] ))
        {
            float x = strtof( tokens[0].c_str(), nullptr );
            float y = strtof( tokens[1].c_str(), nullptr );
            float z = strtof( tokens[2].c_str(), nullptr );

            return Vector3( x, y, z );
        }
        else
            return defaultValue;
    }
    else
        return defaultValue;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
IntVector2 GetIntVector2Property( const XMLNode& node, const std::string& propertyName, const IntVector2& defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    std::string strProp( prop );
    Strings tokens;
    Tokenize( strProp, tokens, "," );

    if (tokens.size() == 2)
    {
        if (IsInt( tokens[0] ) && IsInt( tokens[1] ))
        {
            int x = strtol( tokens[0].c_str(), nullptr, 10 );
            int y = strtol( tokens[1].c_str(), nullptr, 10 );

            return IntVector2( x, y );
        }
        else
            return defaultValue;
    }
    else
        return defaultValue;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
IntVector3 GetIntVector3Property( const XMLNode& node, const std::string& propertyName, const IntVector3& defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    std::string strProp( prop );
    Strings tokens;
    Tokenize( strProp, tokens, "," );

    if (tokens.size() == 3)
    {
        if (IsInt( tokens[0] ) && IsInt( tokens[1] ) && IsInt( tokens[2] ))
        {
            int x = strtol( tokens[0].c_str(), nullptr, 10 );
            int y = strtol( tokens[1].c_str(), nullptr, 10 );
            int z = strtol( tokens[2].c_str(), nullptr, 10 );
            return IntVector3( x, y, z );
        }
        else
            return defaultValue;
    }
    else
        return defaultValue;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
FltRange GetFloatIntervalProperty( const XMLNode& node, const std::string& propertyName, const FltRange& defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    std::string strProp( prop );
    Strings tokens;
    Tokenize( strProp, tokens, "~" );

    if (tokens.size() == 1)
    {
        if (IsFloat( tokens[0] ))
        {
            float min = strtof( tokens[0].c_str(), nullptr );
            float max = min;

            return FltRange( min, max );
        }
        else
            return defaultValue;
    }

    else if (tokens.size() == 2)
    {
        if (IsFloat( tokens[0] ) && IsFloat( tokens[1] ))
        {
            float min = strtof( tokens[0].c_str(), nullptr );
            float max = strtof( tokens[1].c_str(), nullptr );

            return FltRange( min, max );
        }
        else
            return defaultValue;
    }
    else
        return defaultValue;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
IntRange GetIntIntervalProperty( const XMLNode& node, const std::string& propertyName, const IntRange& defaultValue )
{
    const char* prop = node.getAttribute( propertyName.c_str() );

    if (!prop)
        return defaultValue;

    std::string strProp( prop );
    Strings tokens;
    Tokenize( strProp, tokens, "~" );

    if (tokens.size() == 1)
    {
        if (IsInt( tokens[0] ))
        {
            int min = strtol( tokens[0].c_str(), nullptr, 10 );
            int max = min;

            return IntRange( min, max );
        }
        else
            return defaultValue;
    }

    else if (tokens.size() == 2)
    {
        if (IsFloat( tokens[0] ) && IsFloat( tokens[1] ))
        {
            int min = strtol( tokens[0].c_str(), nullptr, 10 );
            int max = strtol( tokens[1].c_str(), nullptr, 10 );

            return IntRange( min, max );
        }
        else
            return defaultValue;
    }
    else
        return defaultValue;
}

////===========================================================================================
///===========================================================================================
// Set Helpers
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
XMLNode CreateRootNode()
{
    // creates a root xml node with this declaration
    // <?xml version = "1.0" encoding = "utf-8"?>

    XMLNode root = XMLNode::createXMLTopNode( "xml", true );
    root.addAttribute( "version", "1.0" );
    root.addAttribute( "encoding", "utf-8" );

    return root;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
XMLNode CreateNode( const std::string& tagName )
{
    XMLNode node = XMLNode::createXMLTopNode( tagName.c_str(), false );
    return node;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void AddChild( XMLNode& parent, const XMLNode& newChild )
{
    parent.addChild( newChild );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void AddText( XMLNode& node, const std::string& text )
{
    node.addText( text.c_str() );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void WriteToFile( const std::string& filePath, XMLNode& root )
{
    root.writeToFile( filePath.c_str() );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetStringProperty( XMLNode& node, const std::string& propertyName, const std::string& value, const std::string& defaultValue )
{
    if (value == defaultValue)
        return;

    node.addAttribute( propertyName.c_str(), value.c_str() );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetStringsProperty( XMLNode& node, const std::string& propertyName, const Strings& value, const char& delimiter, const Strings& defaultValue )
{
    if (value == defaultValue)
        return;

    std::string combinedValue = "";

    for each (std::string str in value)
        combinedValue += str + delimiter;

    combinedValue = combinedValue.substr( 0, combinedValue.size() - 1 );

    SetStringProperty( node, propertyName, combinedValue, "" );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetCharacterProperty( XMLNode& node, const std::string& propertyName, char value, char defaultValue )
{
    if (value == defaultValue)
        return;

    std::string val = std::string( &value );

    SetStringProperty( node, propertyName, val, "" );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetIntProperty( XMLNode& node, const std::string& propertyName, int value, int defaultValue )
{
    if (value == defaultValue)
        return;

    SetStringProperty( node, propertyName, ConvertToString( value ), "" );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetUnsignedIntProperty( XMLNode& node, const std::string& propertyName, unsigned int value, unsigned int defaultValue )
{
    if (value == defaultValue)
        return;

    SetStringProperty( node, propertyName, ConvertToString( value ), "" );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetFloatProperty( XMLNode& node, const std::string& propertyName, float value, float defaultValue )
{
    if (value == defaultValue)
        return;

    SetStringProperty( node, propertyName, ConvertToString( value ), "" );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetDoubleProperty( XMLNode& node, const std::string& propertyName, double value, double defaultValue )
{
    if (value == defaultValue)
        return;

    SetStringProperty( node, propertyName, ConvertToString( value ), "" );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetBooleanProperty( XMLNode& node, const std::string& propertyName, bool value, bool defaultValue )
{
    if (value == defaultValue)
        return;

    std::string booleanVal = value ? "true" : "false";
    SetStringProperty( node, propertyName, booleanVal, "" );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetRgbaProperty( XMLNode& node, const std::string& propertyName, const Rgba& value, const Rgba& defaultValue )
{
    if (value == defaultValue)
        return;

    std::string val = ConvertToString( value.r ) + "," + ConvertToString( value.g ) + "," + ConvertToString( value.b ) + "," + ConvertToString( value.a );
    SetStringProperty( node, propertyName, val, "" );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetVector2Property( XMLNode& node, const std::string& propertyName, const Vector2& value, const Vector2& defaultValue )
{
    if (value == defaultValue)
        return;

    std::string val = ConvertToString( value.x ) + "," + ConvertToString( value.y );
    SetStringProperty( node, propertyName, val, "" );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetVector3Property( XMLNode& node, const std::string& propertyName, const Vector3& value, const Vector3& defaultValue )
{
    if (value == defaultValue)
        return;

    std::string val = ConvertToString( value.x ) + "," + ConvertToString( value.y ) + "," + ConvertToString(value.z );
    SetStringProperty( node, propertyName, val, "" );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetIntVector2Property( XMLNode& node, const std::string& propertyName, const IntVector2& value, const IntVector2& defaultValue )
{
    if (value == defaultValue)
        return;

    std::string val = ConvertToString( value.x ) + "," + ConvertToString( value.y );
    SetStringProperty( node, propertyName, val, "" );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetIntVector3Property( XMLNode& node, const std::string& propertyName, const IntVector3& value, const IntVector3& defaultValue )
{
    if (value == defaultValue)
        return;

    std::string val = ConvertToString( value.x ) + "," + ConvertToString( value.y ) + "," + ConvertToString( value.z );
    SetStringProperty( node, propertyName, val, "" );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetFloatIntervalProperty( XMLNode& node, const std::string& propertyName, const FltRange& value, const FltRange& defaultValue )
{
    if (value == defaultValue)
        return;

    std::string val = ConvertToString( value.m_min ) + "~" + ConvertToString( value.m_max );
    SetStringProperty( node, propertyName, val, "" );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SetIntIntervalProperty( XMLNode& node, const std::string& propertyName, const IntRange& value, const IntRange& defaultValue )
{
    if (value == defaultValue)
        return;


    std::string val = ConvertToString( value.m_min ) + "~" + ConvertToString( value.m_max );
    SetStringProperty( node, propertyName, val, "" );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template <typename T >
std::string ConvertToString( const T& value )
{
    return std::to_string( value );
}