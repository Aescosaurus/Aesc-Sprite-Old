#pragma once

#include "Vec2.h"

template<typename T>
class Rect_
{
public:
	constexpr Rect_() = default;
	constexpr Rect_( T left_in,T right_in,T top_in,T bottom_in )
		:
		left( left_in ),
		right( right_in ),
		top( top_in ),
		bottom( bottom_in )
	{}
	constexpr Rect_( const Vec2_<T>& topLeft,const Vec2_<T>& bottomRight )
		:
		Rect_( topLeft.x,bottomRight.x,topLeft.y,bottomRight.y )
	{}
	constexpr Rect_( const Vec2_<T>& topLeft,T width,T height )
		:
		Rect_( topLeft,topLeft + Vec2_<T>( width,height ) )
	{}

	template<typename U>
	constexpr operator Rect_<U>() const
	{
		return Rect_<U>( U( left ),U( right ),U( top ),U( bottom ) );
	}

	constexpr bool IsOverlappingWith( const Rect_& other ) const
	{
		return right > other.left && left < other.right
			&& bottom > other.top && top < other.bottom;
	}
	constexpr bool IsContainedBy( const Rect_& other ) const
	{
		return left >= other.left && right <= other.right &&
			top >= other.top && bottom <= other.bottom;
	}
	constexpr bool ContainsPoint( const Vec2_<T>& pos ) const
	{
		return pos.x > left && pos.x < right &&
			pos.y > top && pos.y < bottom;
	}

	constexpr void MoveTo( const Vec2_<T>& point )
	{
		right += point.x - left;
		bottom += point.y - top;
		left = point.x;
		top = point.y;
	}
	constexpr void MoveBy( const Vec2_<T>& amount )
	{
		left += amount.x;
		right += amount.x;
		top += amount.y;
		bottom += amount.y;
	}
	constexpr Rect_ GetMovedBy( const Vec2_<T>& amount ) const
	{
		auto temp = *this;
		temp.MoveBy( amount );
		return( temp );
	}

	constexpr void Squareize()
	{
		if( GetWidth() > GetHeight() )
		{
			SetWidth( GetHeight() );
		}
		else
		{
			SetHeight( GetWidth() );
		}
	}
	constexpr void FloatDivide( const Vei2& amount )
	{
		auto temp = int( left );
		temp /= amount.x;
		left = int( temp );

		temp = int( right );
		temp /= amount.x;
		right = int( temp );

		temp = int( top );
		temp /= amount.y;
		top = int( temp );

		temp = int( bottom );
		temp /= amount.y;
		bottom = int( temp );
	}

	static constexpr Rect_ FromCenter( const Vec2_<T>& center,
		T halfWidth,T halfHeight )
	{
		const Vec2_<T> half( halfWidth,halfHeight );
		return Rect( center - half,center + half );
	}
	constexpr Rect_ GetExpanded( T offset ) const
	{
		return Rect_( left - offset,right + offset,top - offset,bottom + offset );
	}
	constexpr Rect_ GetExpandedByScale( const Vec2_<T>& scale ) const
	{
		RectI temp = *this;
		// temp.MoveTo( Vei2{ 0,0 } );
		temp.left *= scale.x;
		temp.top *= scale.y;
		temp.right *= scale.x;
		temp.bottom *= scale.y;
		return( temp );
	}
	constexpr Vec2_<T> GetCenter() const
	{
		return Vec2_<T>( ( left + right ) / T( 2 ),( top + bottom ) / T( 2 ) );
	}
	constexpr Rect_ GetClipped( const Rect_& clip )
	{
		Rect_ temp = *this;
		temp.left = std::max( temp.left,clip.left );
		temp.top = std::max( temp.top,clip.top );
		temp.right = std::min( temp.right,clip.right );
		temp.bottom = std::min( temp.bottom,clip.bottom );
		return( temp );
	}

	constexpr T GetWidth() const
	{
		return right - left;
	}
	constexpr T GetHeight() const
	{
		return bottom - top;
	}
	constexpr void SetWidth( int newWidth )
	{
		right = left + newWidth;
	}
	constexpr void SetHeight( int newHeight )
	{
		bottom = top + newHeight;
	}
	constexpr Vec2_<T> GetSize() const
	{
		return( Vec2_<T>{ GetWidth(),GetHeight() } );
	}
public:
	T left;
	T right;
	T top;
	T bottom;
};

typedef Rect_<float> Rect;
typedef Rect_<int> RectI;
typedef Rect_<double> Recd;