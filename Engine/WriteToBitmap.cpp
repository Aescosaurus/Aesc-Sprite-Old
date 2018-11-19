#include "WriteToBitmap.h"
#include <fstream>
#include <cassert>

void WriteToBitmap::Write( const Surface& data,
	const std::string& name )
{
	std::ofstream out{ name };
	assert( out.good() );

	const int mcuWidth = ( data.GetWidth() + 7 ) / 8;
	const int mcuHeight = ( data.GetHeight() + 7 ) / 8;
	const int paddingSize = data.GetWidth() % 4;
	const int size = 14 + 12 +
		( data.GetWidth() * data.GetHeight() * 3 ) +
		( paddingSize * data.GetHeight() );

	// First part of the header.
	out.put( 'B' ); // Tells that this is a bitmap.
	out.put( 'M' );
	PutInt( out,size ); // Total file size.
	PutInt( out,0 ); // Four unused values as four zeroes.
	PutInt( out,0x1A ); // Starting address of pixel array.

	// DIB header.
	PutInt( out,12 ); // Size of the DIB header.
	PutShort( out,data.GetWidth() ); // Width and height as
	PutShort( out,data.GetHeight() ); //  2 byte integers.
	PutShort( out,1 ); // Number of planes.
	PutShort( out,24 ); // Number of bits per pixel.

	// Write all the rows to the output file.
	for( int y = data.GetWidth() - 1; y >= 0; --y )
	{
		for( int x = 0; x < data.GetWidth(); ++x )
		{
			// PutInt( out,int( data.GetPixel( x,y ).dword ) );
			const auto pix = data.GetPixel( x,y );
			out.put( pix.GetB() );
			out.put( pix.GetG() );
			out.put( pix.GetR() );
		}
		for( int i = 0; i < paddingSize; ++i )
		{
			out.put( 0 );
		}
	}
}

void WriteToBitmap::PutShort( std::ofstream& out,int v )
{
	out.put( ( v >> 0 ) & 0xFF );
	out.put( ( v >> 8 ) & 0xFF );
}

void WriteToBitmap::PutInt( std::ofstream& out,int v )
{
	out.put( ( v >> 0 ) & 0xFF );
	out.put( ( v >> 8 ) & 0xFF );
	out.put( ( v >> 16 ) & 0xFF );
	out.put( ( v >> 24 ) & 0xFF );
}
