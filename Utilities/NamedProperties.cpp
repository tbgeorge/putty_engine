//=================================================================================
// NamedProperties.cpp
// Author: Tyler George
// Date  : April 20, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Utilities/NamedProperties.hpp"

////===========================================================================================
///===========================================================================================
//  class NamedProperties
///===========================================================================================
////===========================================================================================

////---------------------------------------------------------------------------------
///
// Mutators
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NamedProperties::Set( const std::string& propertyName, const std::string& propertyValue )
{
    auto found = m_properties.find( propertyName );
    // check if already found
    if (found != m_properties.end())
    {
        TypedPropertyBase* propertyBase = found->second;
        // check if existing type is already correct
        if (propertyBase->IsOfType( typeid(std::string) ))
        {
            TypedProperty< std::string >* property = reinterpret_cast<TypedProperty< std::string >*>(propertyBase);
            property->m_propertyValue = propertyValue;
            return;
        }
        else
        {
            delete propertyBase;
        }
    }


    TypedPropertyBase* newProperty = new TypedProperty< std::string >( propertyValue );
    m_properties[propertyName] = newProperty;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NamedProperties::Set( const std::string& propertyName, const char* propertyValue )
{
    auto found = m_properties.find( propertyName );
    // check if already found
    if (found != m_properties.end())
    {
        TypedPropertyBase* propertyBase = found->second;
        // check if existing type is already correct
        if (propertyBase->IsOfType( typeid(std::string) ))
        {
            TypedProperty< std::string >* property = reinterpret_cast<TypedProperty< std::string >*>(propertyBase);
            property->m_propertyValue = std::string( propertyValue );
            return;
        }
        else
        {
            delete propertyBase;
        }
    }


    TypedPropertyBase* newProperty = new TypedProperty< std::string >( std::string( propertyValue ) );
    m_properties[propertyName] = newProperty;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NamedProperties::AddProperties( const NamedProperties& props )
{
    for (auto iter = props.m_properties.begin(); iter != props.m_properties.end(); ++iter)
    {
        const std::string& propertyName = iter->first;
        TypedPropertyBase* propertyValue = iter->second;
        //todo memory leak

        m_properties[propertyName] = (TypedPropertyBase*)propertyValue->Clone();
    }
}

////---------------------------------------------------------------------------------
///
// Overloaded operators
///
////---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
const NamedProperties NamedProperties::operator=( const NamedProperties& npToAssign )
{
    for (auto iter = npToAssign.m_properties.begin(); iter != npToAssign.m_properties.end(); ++iter)
    {
        const std::string& propertyName = iter->first;
        TypedPropertyBase* propertyValue = iter->second;

        m_properties.insert( std::pair< std::string, TypedPropertyBase* >( propertyName, (TypedPropertyBase*)propertyValue->Clone() ) );
        
    }

    return *this;
}
