#include "Random.h"

Random::Random()
	:
	rng( std::random_device{}( ) )
{
}

int Random::RangeI( int min,int max ) const
{
	std::uniform_int_distribution<int> dist( min,max );
	return dist( rng );
}

float Random::RangeF( float min,float max ) const
{
	std::uniform_real_distribution<float> dist( min,max );
	return dist( rng );
}

// int Random::NextInt( int min,int max )
// {
// 	std::uniform_int_distribution<int> dist( min,max );
// 	return dist( rng );
// }
// 
// float Random::NextFloat( float min,float max )
// {
// 	std::uniform_real_distribution<float> dist( min,max );
// 	return dist( rng );
// }
