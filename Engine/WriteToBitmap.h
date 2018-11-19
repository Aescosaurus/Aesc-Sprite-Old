#pragma once

#include <string>
#include "Surface.h"

// Used this video to make this:
//  https://www.youtube.com/watch?v=ldsdJqGr9uc
class WriteToBitmap
{
public:
	static void Write( const Surface& data,
		const std::string& name );
private:
	static void PutShort( std::ofstream& out,int v );
	static void PutInt( std::ofstream& out,int v );
};