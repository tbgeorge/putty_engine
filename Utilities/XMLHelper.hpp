//=================================================================================
// XMLHelper.hpp
// Author: Tyler George
// Date  : September 10, 2015
//=================================================================================

#pragma  once

#ifndef __XMLHelper__
#define __XMLHelper__

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================


#include <string>

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Utilities/XMLParser.h"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/FltRange.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/IntVector3.hpp"
#include "Engine/Math/IntRange.hpp"
#include "Engine/Utilities/Rgba.hpp"



////===========================================================================================
///===========================================================================================
// Get Helpers
///===========================================================================================
////===========================================================================================

std::string				GetXMLNodeName( const XMLNode& xmlNode );
std::string				GetStringProperty( const XMLNode& node, const std::string& propertyName, const std::string& defaultValue, bool returnAsLowerCase = false );
Strings					GetStringsProperty( const XMLNode& node, const std::string& propertyName, const std::string& delimiters, const Strings& defaultValue );
char					GetCharacterProperty( const XMLNode& node, const std::string& propertyName, char defaultValue );
int						GetIntProperty( const XMLNode& node, const std::string& propertyName, int defaultValue );
unsigned int			GetUnsignedIntProperty( const XMLNode& node, const std::string& propertyName, unsigned int defaultValue );
float					GetFloatProperty( const XMLNode& node, const std::string& propertyName, float defaultValue );
double					GetDoubleProperty( const XMLNode& node, const std::string& propertyName, double defaultValue );
bool					GetBooleanProperty( const XMLNode& node, const std::string& propertyName, bool defaultValue );
Rgba					GetRgbaProperty( const XMLNode& node, const std::string& propertyName, const Rgba& defaultValue );
Vector2					GetVector2Property( const XMLNode& node, const std::string& propertyName, const Vector2& defaultValue = Vector2::ZERO );
Vector3					GetVector3Property( const XMLNode& node, const std::string& propertyName, const Vector3& defaultValue = Vector3::ZERO );
IntVector2				GetIntVector2Property( const XMLNode& node, const std::string& propertyName, const IntVector2& defaultValue = IntVector2::ZERO );
IntVector3				GetIntVector3Property( const XMLNode& node, const std::string& propertyName, const IntVector3& defaultValue = IntVector3::ZERO );
FltRange			    GetFloatIntervalProperty( const XMLNode& node, const std::string& propertyName, const FltRange& defaultValue = FltRange::ZERO );
IntRange				GetIntIntervalProperty( const XMLNode& node, const std::string& propertyName, const IntRange& defaultValue = IntRange::ZERO );

////===========================================================================================
///===========================================================================================
// Set Helpers
///===========================================================================================
////===========================================================================================

XMLNode                 CreateRootNode();
XMLNode                 CreateNode( const std::string& tagName );
void                    AddChild( XMLNode& parent, const XMLNode& newChild );
void                    AddText( XMLNode& node, const std::string& text );
void                    WriteToFile( const std::string& filePath, XMLNode& root );

void                    SetStringProperty( XMLNode& node, const std::string& propertyName, const std::string& value, const std::string& defaultValue );
void                    SetStringsProperty( XMLNode& node, const std::string& propertyName, const Strings& value, const char& delimiter, const Strings& defaultValue );
void                    SetCharacterProperty( XMLNode& node, const std::string& propertyName, char value, char defaultValue );
void                    SetIntProperty( XMLNode& node, const std::string& propertyName, int value, int defaultValue );
void                    SetUnsignedIntProperty( XMLNode& node, const std::string& propertyName, unsigned int value, unsigned int defaultValue );
void                    SetFloatProperty( XMLNode& node, const std::string& propertyName, float value, float defaultValue );
void                    SetDoubleProperty( XMLNode& node, const std::string& propertyName, double value, double defaultValue );
void                    SetBooleanProperty( XMLNode& node, const std::string& propertyName, bool value, bool defaultValue );
void                    SetRgbaProperty( XMLNode& node, const std::string& propertyName, const Rgba& value, const Rgba& defaultValue );
void                    SetVector2Property( XMLNode& node, const std::string& propertyName, const Vector2& value, const Vector2& defaultValue );
void                    SetVector3Property( XMLNode& node, const std::string& propertyName, const Vector3& value, const Vector3& defaultValue );
void                    SetIntVector2Property( XMLNode& node, const std::string& propertyName, const IntVector2& value, const IntVector2& defaultValue );
void                    SetIntVector3Property( XMLNode& node, const std::string& propertyName, const IntVector3& value, const IntVector3& defaultValue );
void                    SetFloatIntervalProperty( XMLNode& node, const std::string& propertyName, const FltRange& value, const FltRange& defaultValue );
void                    SetIntIntervalProperty( XMLNode& node, const std::string& propertyName, const IntRange& value, const IntRange& defaultValue );

template < typename T >
std::string             ConvertToString( const T& value );

#endif




