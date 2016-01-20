//=================================================================================
// Noise.cpp
// Author: Squirrel Eiserloh
//=================================================================================

#include "Engine/Math/Noise.hpp"


//---------------------------------------------------------------------------
// Computes a random Perlin noise value based on a 2D input <position> and
//	Perlin noise parameters.  Recursive (for additional octaves).
//
//	<perlinNoiseGridCellSize>: Noise density.  Larger values produce longer
//		wavelength noise (e.g. gentle, sweeping hills).
//	<numOctaves>: 0 is flat, 1 is simple smoothed noise. Values of 2+ add one
//		or more additional "octave" harmonics.  Each additional octave has
//		double the frequency/density but only a fraction of the amplitude of
//		the base noise.
//	<baseAmplitude>: The minimum (-amplitude) and maximum (+amplitude) values
//		produced by the first octave of the noise.  Note that adding
//		additional octaves can push the final total Perlin noise values above
//		or below the maximum base amplitude; the noise can be "normalized" by
//		the caller (omitted from this function for optimization purposes) via:
//				noise *= A / (A + (A * P))
//		...where A is the <baseAmplitude> and P is the <persistance>.
//	<persistance>: The fraction of amplitude of each subsequent octave, based on the amplitude of the previous octave.  For
//		example, with a persistance of 0.3, each octave is only 30% as strong as the previous octave.
//
float ComputePerlinNoiseValueAtPosition2D( const Vector2& position, float perlinNoiseGridCellSize,
	int numOctaves, float baseAmplitude, float persistance, int randomSeed )
{
	int numOctavesRemaining = numOctaves;
	float amplitude = baseAmplitude;
	float gridSize = perlinNoiseGridCellSize;
	float totalPerlinNoise = 0.f;
	while( numOctavesRemaining > 0 )
	{
		Vector2 perlinPosition = position / gridSize;
		Vector2 perlinPositionFloor( floor( perlinPosition.x ), floor( perlinPosition.y ) );
		IntVector2 perlinCell( (int) perlinPositionFloor.x, (int) perlinPositionFloor.y );
		Vector2 perlinPositionUV = perlinPosition - perlinPositionFloor;
		Vector2 perlinPositionAntiUV( perlinPositionUV.x - 1.f, perlinPositionUV.y - 1.f );
		float eastWeight = SmoothStep( perlinPositionUV.x );
		float northWeight = SmoothStep( perlinPositionUV.y );
		float westWeight = 1.f - eastWeight;
		float southWeight = 1.f - northWeight;

		Vector2 southwestNoiseGradient = GetPseudoRandomNoiseUnitVector2D( perlinCell.x, perlinCell.y, randomSeed );
		Vector2 southeastNoiseGradient = GetPseudoRandomNoiseUnitVector2D( perlinCell.x + 1, perlinCell.y, randomSeed );
		Vector2 northeastNoiseGradient = GetPseudoRandomNoiseUnitVector2D( perlinCell.x + 1, perlinCell.y + 1, randomSeed );
		Vector2 northwestNoiseGradient = GetPseudoRandomNoiseUnitVector2D( perlinCell.x, perlinCell.y + 1, randomSeed );

		float southwestDot = DotProduct( southwestNoiseGradient, perlinPositionUV );
		float southeastDot = DotProduct( southeastNoiseGradient, Vector2( perlinPositionAntiUV.x, perlinPositionUV.y ) );
		float northeastDot = DotProduct( northeastNoiseGradient, perlinPositionAntiUV );
		float northwestDot = DotProduct( northwestNoiseGradient, Vector2( perlinPositionUV.x, perlinPositionAntiUV.y ) );

		float southBlend = (eastWeight * southeastDot) + (westWeight * southwestDot);
		float northBlend = (eastWeight * northeastDot) + (westWeight * northwestDot);
		float fourWayBlend = (southWeight * southBlend) + (northWeight * northBlend);
		float perlinNoiseAtThisOctave = amplitude * fourWayBlend;

		-- numOctavesRemaining;
		totalPerlinNoise += perlinNoiseAtThisOctave;
		amplitude *= persistance;
		gridSize *= 0.5f;
	}

	return totalPerlinNoise;
}

// /---------------------------------------------------------------------------------
// /
// /---------------------------------------------------------------------------------
// float ComputePerlinNoiseValueAtPosition3D( const Vector3& position, float perlinNoiseGridCellSize,
// 	int numOctaves, float baseAmplitude, float persistance, int randomSeed )
// {
// 	int numOctavesRemaining = numOctaves;
// 	float amplitude = baseAmplitude;
// 	float gridSize = perlinNoiseGridCellSize;
// 	float totalPerlinNoise = 0.f;
// 	while( numOctavesRemaining > 0 )
// 	{
// 		Vector3 perlinPosition = position / gridSize;
// 		Vector3 perlinPositionFloor( floorf( perlinPosition.x ), floorf( perlinPosition.y ), floorf( perlinPosition.z ) );
// 		IntVector3 perlinCell( (int) perlinPositionFloor.x, (int) perlinPositionFloor.y, (int) perlinPositionFloor.z );
// 		Vector3 perlinPositionUV = perlinPosition - perlinPositionFloor;
// 		Vector3 perlinPositionAntiUV( perlinPositionUV.x - 1.f, perlinPositionUV.y - 1.f, perlinPositionUV.z = 1.f );
// 		float eastWeight = SmoothStep( perlinPositionUV.x );
// 		float northWeight = SmoothStep( perlinPositionUV.y );
// 		float topWeight = SmoothStep( perlinPositionUV.z );
// 		float westWeight = 1.f - eastWeight;
// 		float southWeight = 1.f - northWeight;
// 		float bottomWeight = 1.f - topWeight;
// 
// 		Vector3 southwestBottomNoiseGradient = GetPseudoRandomNoiseUnitVector3D( perlinCell.x, perlinCell.y, perlinCell.z, randomSeed );
// 		Vector3 southeastBottomNoiseGradient = GetPseudoRandomNoiseUnitVector3D( perlinCell.x + 1, perlinCell.y, perlinCell.z, randomSeed );
// 		Vector3 northeastBottomNoiseGradient = GetPseudoRandomNoiseUnitVector3D( perlinCell.x + 1, perlinCell.y + 1, perlinCell.z, randomSeed );
// 		Vector3 northwestBottomNoiseGradient = GetPseudoRandomNoiseUnitVector3D( perlinCell.x, perlinCell.y + 1, perlinCell.z, randomSeed );
// 		Vector3 southwestTopNoiseGradient = GetPseudoRandomNoiseUnitVector3D( perlinCell.x, perlinCell.y, perlinCell.z + 1, randomSeed );
// 		Vector3 southeastTopNoiseGradient = GetPseudoRandomNoiseUnitVector3D( perlinCell.x + 1, perlinCell.y, perlinCell.z + 1, randomSeed );
// 		Vector3 northeastTopNoiseGradient = GetPseudoRandomNoiseUnitVector3D( perlinCell.x + 1, perlinCell.y + 1, perlinCell.z + 1, randomSeed );
// 		Vector3 northwestTopNoiseGradient = GetPseudoRandomNoiseUnitVector3D( perlinCell.x, perlinCell.y + 1, perlinCell.z + 1, randomSeed );
// 
// 		float southwestBottomDot = DotProduct( southwestBottomNoiseGradient, perlinPositionUV );
// 		float southeastBottomDot = DotProduct( southeastBottomNoiseGradient, Vector3( perlinPositionAntiUV.x, perlinPositionUV.y, perlinPositionUV.z ) );
// 		float northeastBottomDot = DotProduct( northeastBottomNoiseGradient, perlinPositionAntiUV );
// 		float northwestBottomDot = DotProduct( northwestBottomNoiseGradient, Vector3( perlinPositionUV.x, perlinPositionAntiUV.y, perlinPositionUV.z ) );
// 		float southwestTopDot = DotProduct( southwestTopNoiseGradient, perlinPositionUV );
// 		float southeastTopDot = DotProduct( southeastTopNoiseGradient, Vector3( perlinPositionAntiUV.x, perlinPositionUV.y, perlinPositionAntiUV.z ) );
// 		float northeastTopDot = DotProduct( northeastTopNoiseGradient, perlinPositionAntiUV );
// 		float northwestTopDot = DotProduct( northwestTopNoiseGradient, Vector3( perlinPositionUV.x, perlinPositionAntiUV.y, perlinPositionAntiUV.z ) );
// 
// 		float southBottomBlend = (eastWeight * southeastBottomDot) + (westWeight * southwestBottomDot);
// 		float northBottomBlend = (eastWeight * northeastBottomDot) + (westWeight * northwestBottomDot);
// 		float southTopBlend = (eastWeight * southeastTopDot) + (westWeight * southwestTopDot);
// 		float northTopBlend = (eastWeight * northeastTopDot) + (westWeight * northwestTopDot);
// 
// 		float fourWayBlend = (southWeight * southBlend) + (northWeight * northBlend);
// 		float perlinNoiseAtThisOctave = amplitude * fourWayBlend;
// 
// 		-- numOctavesRemaining;
// 		totalPerlinNoise += perlinNoiseAtThisOctave;
// 		amplitude *= persistance;
// 		gridSize *= 0.5f;
// 	}
// 
// 	return totalPerlinNoise;
// }

