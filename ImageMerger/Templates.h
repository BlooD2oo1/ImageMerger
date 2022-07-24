#pragma once

template< class T >
__forceinline void Swap( T& a, T& b )
{
	T c = a;
	a = b;
	b = c;
}

template< class T, class T2 >
__forceinline T	Lerp( const T& f0, const T& f1, const T2 fX )
{ 
	//return ( T )( fA * ( 1.0f - fW ) + fB * fW );
	return (T)( f0 + fX * ( f1-f0 ) );

}

template< class T >
__forceinline T	Lerp( const T& f00, const T& f01, const T& f10, const T& f11, const float fX, const float fY )
{ 
	float f0 = Lerp( f00, f10, fX );
	float f1 = Lerp( f01, f11, fX );

	return Lerp( f0, f1, fY );
}

template< class T >
__forceinline T	Lerp( const T& f000, const T& f001, const T& f010, const T& f011, const T& f100, const T& f101, const T& f110, const T& f111, const float fX, const float fY, const float fZ )
{ 
	float f00 = Lerp( f000, f100, fX );
	float f01 = Lerp( f001, f101, fX );
	float f10 = Lerp( f010, f110, fX );
	float f11 = Lerp( f011, f111, fX );

	return Lerp( f00, f01, f10, f11, fY, fZ );
}

template< class T >
__forceinline float	LerpParam( const T& fA, const T& fB, const T& fVal ) 
{ 
	return ( fVal - fA )/( fB - fA );
}

template< class T >
__forceinline T Min( const T& a, const T& b )
{
	return (T) (((a) < (b)) ? (a) : (b));
}

template< class T >
__forceinline T Min( const T& a, const T& b, const T& c )
{
	T min = a;
	if ( b > min ) min = b;
	if ( c > min ) min = c;
	return min;
}

template< class T >
__forceinline T Max( const T& a, const T& b )
{
	return (T) (((a) > (b)) ? (a) : (b));
}

template< class T >
__forceinline T Max( const T& a, const T& b, const T& c )
{
	T max = a;
	if ( b > max ) max = b;
	if ( c > max ) max = c;
	return max;
}

template< class T >
__forceinline void MinMax( T& x, const T& a, const T& b )
{
	x = (((x) > (a)) ? (x) : (a));
	x = (((x) < (b)) ? (x) : (b));
}

template < typename T >
int Sign( T val )
{
	return ( (T(0) < val) - (val < T(0)) );
}