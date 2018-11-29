#pragma once

#include <vector>

namespace aesc
{
	// Remove an item from vector, returns true if removed.
	//  Only removes first item if there are multiple.
	template<typename T>
	bool remove_item( std::vector<T>& vec,const T& item )
	{
		for( int i = 0; i < int( vec.size() ); ++i )
		{
			if( vec[i] == item )
			{
				std::swap( vec[i],vec.back() );
				vec.pop_back();
				return( true );
			}
		}
		return( false );
	}
}