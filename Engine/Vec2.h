#pragma once

#include <cmath>

template<typename T>
class Vec2_
{
public:
	Vec2_() = default;
	Vec2_( T x_in,T y_in )
		:
		x( x_in ),
		y( y_in )
	{}
	Vec2_( const Vec2_<float>& src )
		:
		x( T( src.x ) ),
		y( T( src.y ) )
	{}

	template<typename U>
	operator Vec2_<U>() const
	{
		return Vec2_<U>( U( x ),U( y ) );
	}

	Vec2_ operator+( const Vec2_& rhs ) const
	{
		return Vec2_( x + rhs.x,y + rhs.y );
	}
	Vec2_& operator+=( const Vec2_& rhs )
	{
		return *this = *this + rhs;
	}
	Vec2_ operator*( T rhs ) const
	{
		return Vec2_( x * rhs,y * rhs );
	}
	Vec2_& operator*=( T rhs )
	{
		return *this = *this * rhs;
	}
	Vec2_ operator-( const Vec2_& rhs ) const
	{
		return Vec2_( x - rhs.x,y - rhs.y );
	}
	Vec2_& operator-=( const Vec2_& rhs )
	{
		return *this = *this - rhs;
	}
	Vec2_ operator/( T rhs ) const
	{
		return Vec2_{ x / rhs,y / rhs };
	}
	Vec2_& operator/=( T rhs )
	{
		*this = ( *this ) / rhs;
		return *this;
	}

	T GetLength() const
	{
		return T( std::sqrt( GetLengthSq() ) );
	}
	T GetLengthSq() const
	{
		return x * x + y * y;
	}

	Vec2_& Normalize()
	{
		return *this = GetNormalized();
	}
	Vec2_ GetNormalized() const
	{
		const T len = GetLength();
		if( len != T( 0.0f ) )
		{
			return *this * ( 1.0f / len );
		}
		return *this;
	}

	Vec2_ Rotation( const T angle ) const
	{
		Vec2_ result;
		result.x = x * cosf( angle ) - y * sinf( angle );
		result.y = x * sinf( angle ) + y * cosf( angle );
		return result;
	}

	static Vec2_ Up()
	{
		return Vec2_{ 0.0f,-1.0f };
	}
	static Vec2_ Down()
	{
		return Vec2_{ 0.0f,1.0f };
	}
	static Vec2_ Left()
	{
		return Vec2_{ -1.0f,0.0f };
	}
	static Vec2_ Right()
	{
		return Vec2_{ 1.0f,0.0f };
	}
public:
	T x;
	T y;
};

typedef Vec2_<float> Vec2;
typedef Vec2_<int> Vei2;