//=================================================================================
// NamedProperties.hpp
// Author: Tyler George
// Date  : March 23, 2016
//=================================================================================
#pragma once


#ifndef __included_NamedProperties__
#define __included_NamedProperties__

#include "..\Systems\Events\TypedProperty.hpp"
#include <string>
#include <map>

///---------------------------------------------------------------------------------
/// Enum
///---------------------------------------------------------------------------------
enum PropertyGetResult
{
    RESULT_SUCCESS,
    RESULT_FAILED_NOT_FOUND,
    RESULT_WRONG_TYPE,
    RESULT_PROPERTIES_WAS_EMPTY,
    NUM_RESULTS
};

///---------------------------------------------------------------------------------
/// class NamedProperties
///---------------------------------------------------------------------------------
class NamedProperties
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    ~NamedProperties();
    NamedProperties();
    NamedProperties( const NamedProperties& copy );

    template< typename T_P1 >
    NamedProperties( const std::string& p1Name, const T_P1& p1 );

    template< typename T_P1, typename T_P2 >
    NamedProperties( const std::string& p1Name, const T_P1& p1, const std::string& p2Name, const T_P2& p2 );

    template< typename T_P1, typename T_P2, typename T_P3 >
    NamedProperties( const std::string& p1Name, const T_P1& p1, const std::string& p2Name, const T_P2& p2, const std::string& p3Name, const T_P3& p3 );



    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    template< typename T >
    void Set( const std::string& propertyName, const T& propertyValue );

    void Set( const std::string& propertyName, const std::string& propertyValue );
    void Set( const std::string& propertyName, const char* propertyValue );

    void AddProperties( const NamedProperties& props );

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    template< typename T >
    PropertyGetResult Get( const std::string& propertyName, T& out_propertyValue ) const;

    const char* GetPropertyTypeName( const std::string& propertyName ) const;

    ///---------------------------------------------------------------------------------
    /// Overloaded operators
    ///---------------------------------------------------------------------------------
    // need to overload = operator
    const NamedProperties	operator= ( const NamedProperties& npToAssign );



private:
    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    std::map< std::string, TypedPropertyBase* > m_properties;
};

////===========================================================================================
///===========================================================================================
// Template function implementations
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T >
void NamedProperties::Set( const std::string& propertyName, const T& propertyValue )
{
    auto found = m_properties.find( propertyName );
    // check if already found
    if (found != m_properties.end())
    {
        TypedPropertyBase* propertyBase = found->second;
        // check if existing type is already correct
        if (propertyBase->IsOfType( typeid(T) ) )
        {
            TypedProperty< T >* property = reinterpret_cast<TypedProperty< T >*>(propertyBase);
            property->m_propertyValue = propertyValue;
            return;
        }
        else
        {
            delete propertyBase;
        }
    }


    TypedPropertyBase* newProperty = new TypedProperty< T >( propertyValue );
    m_properties[propertyName] = newProperty;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T >
PropertyGetResult NamedProperties::Get( const std::string& propertyName, T& out_propertyValue ) const
{
    if (m_properties.empty())
        return RESULT_PROPERTIES_WAS_EMPTY;

    auto found = m_properties.find( propertyName );

    if (found == m_properties.end())
        return RESULT_FAILED_NOT_FOUND;

    TypedPropertyBase* property = found->second;
    if (property->IsOfType( typeid(T) ) )
    {
        TypedProperty< T >*  typedProperty = reinterpret_cast<TypedProperty< T >*>(property);
        out_propertyValue = typedProperty->m_propertyValue;
        return RESULT_SUCCESS;
    }
    else
        return RESULT_WRONG_TYPE;

    return NUM_RESULTS;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T_P1 >
NamedProperties::NamedProperties( const std::string& p1Name, const T_P1& p1 )
{
    Set( p1Name, p1 );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T_P1, typename T_P2 >
NamedProperties::NamedProperties( const std::string& p1Name, const T_P1& p1, const std::string& p2Name, const T_P2& p2 )
{
    Set( p1Name, p1 );
    Set( p2Name, p2 );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T_P1, typename T_P2, typename T_P3 >
NamedProperties::NamedProperties( const std::string& p1Name, const T_P1& p1, const std::string& p2Name, const T_P2& p2, const std::string& p3Name, const T_P3& p3 )
{
    Set( p1Name, p1 );
    Set( p2Name, p2 );
    Set( p3Name, p3 );


}

////===========================================================================================
///===========================================================================================
// Inline function implementations
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline const char* NamedProperties::GetPropertyTypeName( const std::string& propertyName ) const
{
    auto found = m_properties.find( propertyName );

    if (found == m_properties.end())
        return "PROPERTY NOT FOUND";

    TypedPropertyBase* propertyBase = found->second;

    return propertyBase->GetTypeName();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline NamedProperties::~NamedProperties()
{
    for (auto iter = m_properties.begin(); iter != m_properties.end(); ++iter)
    {
        TypedPropertyBase* propertyVal = iter->second;
        delete propertyVal;
    }

    m_properties.clear();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline NamedProperties::NamedProperties()
{
    m_properties.clear();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
inline NamedProperties::NamedProperties( const NamedProperties& copy )
{
    for (auto iter = copy.m_properties.begin(); iter != copy.m_properties.end(); ++iter)
    {
        const std::string& propertyName = iter->first;
        TypedPropertyBase* propertyValue = iter->second;

        m_properties.insert( std::pair< std::string, TypedPropertyBase* >( propertyName, (TypedPropertyBase*)propertyValue->Clone() ) );

    }
}

#endif