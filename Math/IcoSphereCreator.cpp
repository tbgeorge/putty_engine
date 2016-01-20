//=================================================================================
// IcoSphereCreator.cpp
// Author: Tyler George
// Date  : May 28, 2015
//=================================================================================

#include "Engine/Math/IcoSphereCreator.hpp"

///---------------------------------------------------------------------------------
/// Static Variables
///---------------------------------------------------------------------------------
IcoSphereCreator* IcoSphereCreator::s_theIcoSphereCreator = NULL;


//===========================================================================================
//===========================================================================================
//  Public Functions
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
IcoSphereCreator* IcoSphereCreator::GetInstance()
{
    if ( !s_theIcoSphereCreator )
        s_theIcoSphereCreator = new IcoSphereCreator();

    return s_theIcoSphereCreator;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PUC_Vertexes IcoSphereCreator::CreateIcoSphere( int recursionLevel, IntendedRenderMode mode, Rgba color /* = Rgba() */ )
{
    m_vertexes.clear();
    m_points.clear();
//     m_midPointIndexCache.clear();
    m_index = 0;

    // create initial 12 vertices
    float t = (1.0f + sqrt( 5.0f )) / 2.0f;
  
    //testing
//     int a = AddVertex( Vector3( -1.0f, 0.0f, 1.0f ) );
//     int b = AddVertex( Vector3( 1.0f, 0.0f, 1.0f ) );
//     int c = AddVertex( Vector3( 0.0f, 0.0f, -1.0f )   
  
    AddVertex( Vector3( -1.0f, t, 0.0f ) );
    AddVertex( Vector3( 1.0f, t, 0.0f ) );
    AddVertex( Vector3( -1.0f, -t, 0.0f ) );
    AddVertex( Vector3( 1.0f, -t, 0.0f ) );
  
    AddVertex( Vector3( 0.0f, -1.0f, t ) );
    AddVertex( Vector3( 0.0f, 1.0f, t ) );
    AddVertex( Vector3( 0.0f, -1.0f, -t ) );
    AddVertex( Vector3( 0.0f, 1.0f, -t ) );
  
    AddVertex( Vector3( t, 0.0f, -1.0f ) );
    AddVertex( Vector3( t, 0.0f, 1.0f ) );
    AddVertex( Vector3( -t, 0.0f, -1.0f ) );
    AddVertex( Vector3( -t, 0.0f, 1.0f ) );
  
    // create 20 triangles of the icosahedron
    TriangleIndicesList faces;

    // 5 faces around point 0
     faces.push_back( new TriangleIndices( 0, 11, 5 ) );
     faces.push_back( new TriangleIndices( 0, 5, 1 ) );
     faces.push_back( new TriangleIndices( 0, 1, 7 ) );
     faces.push_back( new TriangleIndices( 0, 7, 10 ) );
     faces.push_back( new TriangleIndices( 0, 10, 11 ) );
 
     // 5 adjacent faces 
     faces.push_back( new TriangleIndices( 1, 5, 9 ) );
     faces.push_back( new TriangleIndices( 5, 11, 4 ) );
     faces.push_back( new TriangleIndices( 11, 10, 2 ) );
     faces.push_back( new TriangleIndices( 10, 7, 6 ) );
     faces.push_back( new TriangleIndices( 7, 1, 8 ) );
 
     // 5 faces around point 3
     faces.push_back( new TriangleIndices( 3, 9, 4 ) );
     faces.push_back( new TriangleIndices( 3, 4, 2 ) );
     faces.push_back( new TriangleIndices( 3, 2, 6 ) );
     faces.push_back( new TriangleIndices( 3, 6, 8 ) );
     faces.push_back( new TriangleIndices( 3, 8, 9 ) );
 
     // 5 adjacent faces 
     faces.push_back( new TriangleIndices( 4, 9, 5 ) );
     faces.push_back( new TriangleIndices( 2, 4, 11 ) );
     faces.push_back( new TriangleIndices( 6, 2, 10 ) );
     faces.push_back( new TriangleIndices( 8, 6, 7 ) );
     faces.push_back( new TriangleIndices( 9, 8, 1 ) );


  // refine triangles
     for (int i = 0; i < recursionLevel; i++)
     {
         TriangleIndicesList faces2;
 
         for ( TriangleIndicesList::iterator faceIter = faces.begin(); faceIter != faces.end(); faceIter++ ) 
         {
             TriangleIndices* tri = ( *faceIter );

             int a = AddVertex( ( m_points[tri->m_v1] + m_points[tri->m_v2] ) / 2.0f );
             int b = AddVertex( ( m_points[tri->m_v2] + m_points[tri->m_v3] ) / 2.0f );
             int c = AddVertex( ( m_points[tri->m_v3] + m_points[tri->m_v1] ) / 2.0f );

 
             faces2.push_back( new TriangleIndices( tri->m_v1, a, c ) );
             faces2.push_back( new TriangleIndices( tri->m_v2, b, a ) );
             faces2.push_back( new TriangleIndices( tri->m_v3, c, b ) );
             faces2.push_back( new TriangleIndices( a, b, c ) );
         }
         faces.clear();
         faces.swap( faces2 );
 
     }
 
     // done, now add triangles to vertex list
     Vertex3D_PUC vertex;
     vertex.color = color;
 
     for (TriangleIndicesList::iterator faceIter = faces.begin(); faceIter != faces.end(); faceIter++)
     {
         TriangleIndices* tri = *faceIter;
 
         if (mode == TRIANGLES)
         {
             vertex.position = m_points[tri->m_v1];
             m_vertexes.push_back( vertex );

             vertex.position = m_points[tri->m_v2];
             m_vertexes.push_back( vertex );

             vertex.position = m_points[tri->m_v3];
             m_vertexes.push_back( vertex );
         }

         else if (mode == LINES)
         {
             vertex.position = m_points[tri->m_v1];
             m_vertexes.push_back( vertex );

             vertex.position = m_points[tri->m_v2];
             m_vertexes.push_back( vertex );
             m_vertexes.push_back( vertex );

             vertex.position = m_points[tri->m_v3];
             m_vertexes.push_back( vertex );
             m_vertexes.push_back( vertex );

             vertex.position = m_points[tri->m_v1];
             m_vertexes.push_back( vertex );
         }
 
     }

    return m_vertexes;

}


//===========================================================================================
//===========================================================================================
//  Private Functions
//===========================================================================================
//===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
int IcoSphereCreator::AddVertex( Vector3 vertex )
{
    vertex.Normalize();
    m_points.push_back( vertex );  

    return m_index++;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
// int IcoSphereCreator::GetMidPoint( int pointIndexA, int pointIndexB, int pointIndexC )
// {
//     // first check if we have it already
//     bool firstIsSmaller = pointIndexA < pointIndexB;
//     int64_t smallerIndex = firstIsSmaller ? pointIndexA : pointIndexB;
//     int64_t greaterIndex = firstIsSmaller ? pointIndexB : pointIndexA;
//     int64_t key = ( smallerIndex << 32 ) + greaterIndex;
// 
// 
//     if ( m_midPointIndexCache.find( key ) != m_midPointIndexCache.end() )
//     {
//         return m_midPointIndexCache[key];
//     }
// 
//     // if not in cache, calculate
//     Vector3 pointA = m_points[pointIndexA];
//     Vector3 pointB = m_points[pointIndexB];
//     Vector3 midPoint = Vector3(
//         (pointA.x + pointB.x) / 2.0f,
//         (pointA.y + pointB.y) / 2.0f,
//         (pointA.z + pointB.z) / 2.0f );
// 
// 
//     int midPointIndex = AddVertex( midPoint );
// 
//     m_midPointIndexCache[key] = midPointIndex;
//     return midPointIndex;
// 
// }

