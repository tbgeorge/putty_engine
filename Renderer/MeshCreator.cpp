//=================================================================================
// MeshCreator.cpp
// Author: Tyler George
// Date  : June 29, 2015
//=================================================================================

#include "Engine/Renderer/MeshCreator.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PUTBNC_Vertexes GenerateUVSphereVertexes( const Vector3& center, const float& radius, const int& slices, const int& stacks, const Rgba& color )
{
    PUTBNC_Vertexes verts;

    Vertex3D_PUTBNC vertex1;
    vertex1.color = color;

    Vertex3D_PUTBNC vertex2;
    vertex2.color = color;

    Vertex3D_PUTBNC vertex3;
    vertex3.color = color;

    Vertex3D_PUTBNC vertex4;
    vertex4.color = color;

    for (float t = 0.0f; t < stacks; t += 1.0f) // stacks are ELEVATION so they count theta
    {
        float theta1 = PI * (t / stacks);
        float theta2 = PI * ((t + 1) / stacks);

        for (float p = 0.0f; p < slices; p += 1.0f) // slices are ORANGE SLICES so the count azimuth
        {
            float phi1 = TWO_PI * (p / slices); // azimuth goes around 0 .. 2*PI
            float phi2 = TWO_PI * ((p + 1) / slices);

            //phi2   phi1
            // |      |
            // 2------1 -- theta1
            // |\ _   |
            // |    \ |
            // 3------4 -- theta2
            //

            float sinTheta1 = sin( theta1 );
            float cosTheta1 = cos( theta1 );
            float sinTheta2 = sin( theta2 );
            float cosTheta2 = cos( theta2 );

            float sinPhi1 = sin( phi1 );
            float cosPhi1 = cos( phi1 );
            float sinPhi2 = sin( phi2 );
            float cosPhi2 = cos( phi2 );

            vertex1.position = Vector3(
                (radius * sinPhi1 * sinTheta1),
                (radius * cosTheta1),
                (radius * cosPhi1 * sinTheta1)
                ) + center;
            vertex1.uv = Vector2( phi1 / TWO_PI, theta1 / PI );
            vertex1.normal = vertex1.position.Normalized();
            vertex1.tangent = Vector3(
                (radius * cosPhi1 * sinTheta1),
                (radius * 0),
                (radius * -sinPhi1 * sinTheta1) ).Normalized();
            vertex1.bitangent = CrossProduct( vertex1.tangent, vertex1.normal );

            vertex2.position = Vector3(
                (radius * sinPhi2 * sinTheta1),
                (radius * cosTheta1),
                (radius * cosPhi2 * sinTheta1)
                ) + center;
            vertex2.uv = Vector2( phi2 / TWO_PI, theta1 / PI );
            vertex2.normal = vertex2.position.Normalized();
            vertex2.tangent = Vector3(
                (radius * cosPhi2 * sinTheta1),
                (radius * 0),
                (radius * -sinPhi2 * sinTheta1) ).Normalized();
            vertex2.bitangent = CrossProduct( vertex2.tangent, vertex2.normal );

            vertex3.position = Vector3(
                (radius * sinPhi2 * sinTheta2),
                (radius * cosTheta2),
                (radius * cosPhi2 * sinTheta2)
                ) + center;
            vertex3.uv = Vector2( phi2 / TWO_PI, theta2 / PI );
            vertex3.normal = vertex3.position.Normalized();
            vertex3.tangent = Vector3(
                (radius * cosPhi2 * sinTheta2),
                (radius * 0),
                (radius * -sinPhi2 * sinTheta2) ).Normalized();
            vertex3.bitangent = CrossProduct( vertex3.tangent, vertex3.normal );

            vertex4.position = Vector3(
                (radius * sinPhi1 * sinTheta2),
                (radius * cosTheta2),
                (radius * cosPhi1 * sinTheta2)
                ) + center;
            vertex4.uv = Vector2( phi1 / TWO_PI, theta2 / PI );
            vertex4.normal = vertex4.position.Normalized();
            vertex4.tangent = Vector3(
                (radius * cosPhi1 * sinTheta2),
                (radius * 0),
                (radius * -sinPhi1 * sinTheta2) ).Normalized();
            vertex4.bitangent = CrossProduct( vertex4.tangent, vertex4.normal );


            // facing out
            if (t == 0) // top cap
            {
                verts.push_back( vertex4 ); //t1p1, t2p2, t2p1
                verts.push_back( vertex3 );
                verts.push_back( vertex1 );

            }
            else if (t + 1 == stacks) //end cap
            {
                verts.push_back( vertex2 ); //t2p2, t1p1, t1p2
                verts.push_back( vertex1 );
                verts.push_back( vertex3 );
            }
            else
            {
                // body, facing OUT:
                verts.push_back( vertex4 );
                verts.push_back( vertex2 );
                verts.push_back( vertex1 );

                verts.push_back( vertex4 );
                verts.push_back( vertex3 );
                verts.push_back( vertex2 );
            }
        }
    }
    return verts;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PUTBNC_Vertexes GenerateQuadVertexes( const Vector3& mins, const Vector3& maxs, const Rgba& color )
{
    PUTBNC_Vertexes verts;

    verts.push_back( Vertex3D_PUTBNC( Vector3( mins.x, mins.y, mins.z ), Vector2( 0.0f, 1.0f ), Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( maxs.x, mins.y, maxs.z ), Vector2( 1.0f, 1.0f ), Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( maxs.x, maxs.y, maxs.z ), Vector2( 1.0f, 0.0f ), Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( mins.x, maxs.y, mins.z ), Vector2( 0.0f, 0.0f ), Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, color ) );

    return verts;

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PUTBNC_Vertexes GenerateLineVertexes( const Vector3& startPos, const Vector3& endPos, const Rgba& color )
{
    PUTBNC_Vertexes verts;

    verts.push_back( Vertex3D_PUTBNC( startPos, Vector2( 0.0f, 0.0f ), Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, color ) );
    verts.push_back( Vertex3D_PUTBNC( endPos, Vector2( 0.0f, 0.0f ), Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, color ) );

    return verts;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
PUTBNC_Vertexes GenerateCameraShapeVertexes( const Rgba& color )
{
    PUTBNC_Vertexes verts;

    // Top square part
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, 1.0f,  1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3(  1.0f, 1.0f,  1.0f ), Vector2( 1.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3(  1.0f, 1.0f, -2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), color ) );

    verts.push_back( Vertex3D_PUTBNC( Vector3(  1.0f, 1.0f, -2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, 1.0f, -2.0f ), Vector2( 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, 1.0f,  1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), color ) );

    // Bottom square part
    verts.push_back( Vertex3D_PUTBNC( Vector3(  1.0f, -1.0f,  1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, -1.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, -1.0f,  1.0f ), Vector2( 1.0f, 1.0f ), Vector3( 0.0f, -1.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, -1.0f, -2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, -1.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), color ) );

    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, -1.0f, -2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, -1.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3(  1.0f, -1.0f, -2.0f ), Vector2( 0.0f, 0.0f ), Vector3( 0.0f, -1.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3(  1.0f, -1.0f,  1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, -1.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), color ) );

    // Back square part
    verts.push_back( Vertex3D_PUTBNC( Vector3(  1.0f, -1.0f, -2.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, 0.0f, -1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, -1.0f, -2.0f ), Vector2( 1.0f, 1.0f ), Vector3( 0.0f, 0.0f, -1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f,  1.0f, -2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );

    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f,  1.0f, -2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3(  1.0f,  1.0f, -2.0f ), Vector2( 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3(  1.0f, -1.0f, -2.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, 0.0f, -1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );

    // front square part
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, -1.0f, 1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3(  1.0f, -1.0f, 1.0f ), Vector2( 1.0f, 1.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3(  1.0f,  1.0f, 1.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );

    verts.push_back( Vertex3D_PUTBNC( Vector3(  1.0f,  1.0f, 1.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f,  1.0f, 1.0f ), Vector2( 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, -1.0f, 1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );

    // +x square part
    verts.push_back( Vertex3D_PUTBNC( Vector3( 1.0f, -1.0f,  1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( 1.0f, -1.0f, -2.0f ), Vector2( 1.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( 1.0f,  1.0f, -2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );

    verts.push_back( Vertex3D_PUTBNC( Vector3( 1.0f,  1.0f, -2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( 1.0f,  1.0f,  1.0f ), Vector2( 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( 1.0f, -1.0f,  1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );

    // -x square part
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, -1.0f, -2.0f ), Vector2( 0.0f, 1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, -1.0f,  1.0f ), Vector2( 1.0f, 1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f,  1.0f,  1.0f ), Vector2( 1.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );

    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f,  1.0f,  1.0f ), Vector2( 1.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f,  1.0f, -2.0f ), Vector2( 0.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, -1.0f, -2.0f ), Vector2( 0.0f, 1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), color ) );

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // top trapezoid part
    verts.push_back( Vertex3D_PUTBNC( Vector3( -0.25f, 0.25f, 1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, 1.0f, 2.0f ), Vector2( 1.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( 1.0f, 1.0f, 2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );

    verts.push_back( Vertex3D_PUTBNC( Vector3( 1.0f, 1.0f, 2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( 0.25f, 0.25f, 1.0f ), Vector2( 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -0.25f, 0.25f, 1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );

    // bottom trapezoid part
    verts.push_back( Vertex3D_PUTBNC( Vector3( 0.25f, -0.25f, 1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, -1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( 1.0f, -1.0f, 2.0f ), Vector2( 1.0f, 1.0f ), Vector3( 0.0f, -1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, -1.0f, 2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, -1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );

    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, -1.0f, 2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, -1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -0.25f, -0.25f, 1.0f ), Vector2( 0.0f, 0.0f ), Vector3( 0.0f, -1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( 0.25f, -0.25f, 1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, -1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( -1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );

    // +x trapezoid part
    verts.push_back( Vertex3D_PUTBNC( Vector3( 0.25f, 0.25f, 1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( 1.0f, 1.0f, 2.0f ), Vector2( 1.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( 1.0f, -1.0f, 2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );

    verts.push_back( Vertex3D_PUTBNC( Vector3( 1.0f, -1.0f, 2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( 0.25f, -0.25f, 1.0f ), Vector2( 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( 0.25f, 0.25f, 1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );

    // -x trapezoid part
    verts.push_back( Vertex3D_PUTBNC( Vector3( -0.25f, -0.25f, 1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, -1.0f, 2.0f ), Vector2( 1.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, 1.0f, 2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );

    verts.push_back( Vertex3D_PUTBNC( Vector3( -1.0f, 1.0f, 2.0f ), Vector2( 1.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -0.25f, 0.25f, 1.0f ), Vector2( 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );
    verts.push_back( Vertex3D_PUTBNC( Vector3( -0.25f, -0.25f, 1.0f ), Vector2( 0.0f, 1.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 1.0f, 0.0f, 0.0f ), Rgba::BLACK ) );


    return verts;

}