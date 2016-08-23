//=================================================================================
// TypedProperty.hpp
// Author: Tyler George
// Date  : March 30, 2016
//=================================================================================
#pragma once

#include "Engine/Systems/Events/TypedPropertyBase.hpp"
#include <cstring>
// #include "Engine/Utilities/Console.hpp"

#ifndef __included_TypedProperty__
#define __included_TypedProperty__

///---------------------------------------------------------------------------------
/// template class TypedProperty
///---------------------------------------------------------------------------------
template< typename T >
class TypedProperty : public TypedPropertyBase
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    TypedProperty( const T& propertyValue );
//     TypedProperty( const char* propertyValue )
//         : m_propertyValue( nullptr )
//     {
//         /*m_propertyValue*/
//     }

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    virtual bool IsOfType( const type_info& compareType );
    virtual const char* GetTypeName() const;
    virtual TypedPropertyBase* Clone() const;

    ///---------------------------------------------------------------------------------
    /// Public member variables
    ///---------------------------------------------------------------------------------
    T m_propertyValue;
};

////===========================================================================================
///===========================================================================================
// Templated class function implementations
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T >
TypedProperty< T >::TypedProperty( const T& propertyValue )
    : m_propertyValue( propertyValue )
{
//     ConsolePrintf( "%s \n", typeid(T).name() );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
// template<>

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T >
inline bool TypedProperty< T >::IsOfType( const type_info& compareType )
{
    const type_info& myType = typeid(T);
    return (myType == compareType);
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T >
inline const char* TypedProperty< T >::GetTypeName() const
{
    return (typeid(T).name());
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
template< typename T >
inline TypedPropertyBase* TypedProperty<T>::Clone() const
{
    return new TypedProperty<T>( m_propertyValue );
}

#endif