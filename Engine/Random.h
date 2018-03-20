#pragma once

#include <random>

class Random
{
public:
	Random();
	// int NextInt( int min,int max );
	// float NextFloat( float min,float max );
	int RangeI( int min,int max ) const;
	float RangeF( float min,float max ) const;
private:
	mutable std::mt19937 rng;
};