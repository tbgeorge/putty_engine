//=================================================================================
// MathUtils.cpp
// Author: Tyler George
// Date  : January 16, 2015
//=================================================================================

#include "Engine/Common/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
int GetRandomIntLessThan( int maxValueNotInclusive ) 
{
	int randomInt = rand() % maxValueNotInclusive;
	return randomInt;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
int GetRandomIntInRange( int minValueInclusive, int maxValueInclusive )
{
	int offset = maxValueInclusive - minValueInclusive;
	int randomInt = ( rand() % ( offset + 1 ) ) + minValueInclusive;
	return randomInt;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float GetRandomFloatZeroToOne() 
{
	float randomFloatZeroToOne = INVERSE_RAND_MAX * (float) rand();
	return randomFloatZeroToOne;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float GetRandomFloatInRange( float minimumInclusive, float maximumInclusive ) 
{
	float offset = maximumInclusive - minimumInclusive;
	float randomFloatZeroToOne = INVERSE_RAND_MAX * (float) rand();
	float randomFloatInRange = minimumInclusive + ( randomFloatZeroToOne * offset );
	return randomFloatInRange;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
int TripleRandomInt()
{
	const unsigned int randomGoodBitsMask = 0x00007FE0;
	int randLowBits = rand() >> 5;
	int randMidBits = ( rand() & randomGoodBitsMask ) << 6;
	int randHighBits = ( rand() & randomGoodBitsMask ) << 17;

	return randHighBits | randMidBits | randLowBits;

}

///---------------------------------------------------------------------------------
/// fast
///---------------------------------------------------------------------------------
float CalcShortestAngularDisplacement( float startAngle, float destinationAngle )
{
	float angularDisplacement = destinationAngle - startAngle;
	
	while( angularDisplacement > 180.0f )
		angularDisplacement -= 360.0f;
	while( angularDisplacement < -180.0f )
		angularDisplacement += 360.0f;

	return angularDisplacement;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool AreFloatsEqual(  float lhs, float rhs, float tolerance )
{
	float difference = lhs - rhs;
	difference = fabs( difference );

	bool equal = false;
	if( difference <= tolerance )
		equal = true;
	return equal;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
float DegreeZeroTo360( float degrees )
{
	if( degrees < 0.0f )
	{
		while( degrees <= -360.0f )
			degrees += 360.0f;
		return 360.0f + degrees;
	}
	else
	{
		while( degrees >= 360.0f )
			degrees -= 360.0f;
		return degrees;
	}
}