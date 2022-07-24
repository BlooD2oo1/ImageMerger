#pragma once

#include <vector>
#include <algorithm>

template< class T >
class A2Array : public std::vector< T >
{
public:
	__forceinline A2Array();
	A2Array( T item );
	virtual ~A2Array();

	typedef typename std::vector< T >::const_iterator		array_const_iterator;
	typedef typename std::vector< T >::iterator				array_iterator;

	// kulonbsegkepzes
	void		Difference( const A2Array<T>& aOther, A2Array< int >& aOnlyInThis, A2Array< int >& aOnlyInThat, A2Array< int >& aCommon ) const;

	// kulonbsegkepzes
	A2Array< T >	operator-( const A2Array<T>& aOther ) const;

	// egy elem hozzaadasa
	int			Add( const T item );

	// egy elem hozzaadasa
	int			SortedAdd( const T item );

	// egy elem hozzaadasa, duplikatumok nelkul
	int			AddNoDuplicates( const T item );

	// egy elem hozzaadasa
	int			SortedAddNoDuplicates( const T item );

	// egy ugyanilyen elemekbol allo masik A2Array hozzaadasa
	void		Add( const A2Array<T> array, bool fDuplicatesEnabled = true );

	// egy ugyanilyen elemekbol allo masik A2Array hozzaadasa
	void		operator+=( const A2Array<T> array );

	// egy elem hozzaadasa - NAGYON NEM HATÉKONY, CSAK VÉGSZÜKSÉG ESETÉN HASZNÁLD
	void		SortedAdd( const A2Array<T> array, bool fDuplicatesEnabled = true );

	// egy elem beszurasa adott poziciora - NAGYON NEM HATÉKONY, CSAK VÉGSZÜKSÉG ESETÉN HASZNÁLD
	int			Insert( int nIndex, const T item );

	// egy elem beszurasa adott poziciora, duplikatumok nelkul - NAGYON NEM HATÉKONY, CSAK VÉGSZÜKSÉG ESETÉN HASZNÁLD
	int			InsertNoDuplicates( int nIndex, const T item );

	// visszaadja az elemek szamat
	__forceinline int			GetCount() const;

	// visszaadja hogy érvényes-e adott index
	__forceinline bool			IsValidIndex( int nIndex ) const;

	// visszaadja, hogy tartalmazza-e az adott elemet
	bool		Contains( const T& item ) const;

	// visszaadja, hogy tartalmazza-e az adott elemet (elsosorban rendezett tombokre)
	bool		SortedContains( const T& item ) const;

	//ez meg azt is visszaadja, hogy hany ilyen elem van benne
	int			Count( const T& item ) const;

	// tombindexelessel el lehet kerni az elem referenciajat
	__forceinline T&			operator[](int nIndex);
	__forceinline const T&	operator[](int nIndex) const;

	// egy elem torlese index alapjan
	void		Delete( int nIndex );
	void		DeletePointer( int nIndex );

	// range törlése indexek alapján
	void		Delete( int nStartIndex, int nEndIndex );

	// range törlése iteratorok alapján
	void		Delete( array_iterator startiter, array_iterator enditer );

	//elem torlese iterator alapjan, visszaadja a kovetkezo elemre mutato iteratort
	array_iterator	Delete( array_iterator iter );
	array_iterator	DeletePointer( array_iterator iter );

	// elemek torlese masik tomb alapjan
	void		Delete( const A2Array< T >& array );
	void		SortedDelete( const A2Array< T >& array );

	// egy elem torlese ertek alapjan
	void		DeleteValue( const T item );
	void		SortedDeleteValue( const T item );

	// az osszes elem torlese
	__forceinline void		Clear();

	// pointerek tarolasa eseten, az osszes pointerre meghivja a delete-t
	__forceinline void		ClearPointers();

	// ugyanaz, mint az [] operator
	T&			Item( int nIndex );
	const T&	Item( int nIndex ) const;

	// az adott elem elso elofordulasanak indexet adja vissza
	int			GetIndex( const T&, int nSearchFrom = 0 ) const;

	// az adott elem elso elofordulasanak indexet adja vissza, logaritmikus keresest hasznal
	int			GetIndexLogSearch( const T&, int nSearchFrom = 0 ) const;

	// visszaadja, melyik poziciora kell beszurni az elemet, hogy megmaradjon a rendezettseg
	int			GetInsertIndexLogSearch( const T& item );

	// visszaadja a legkisebb elemet (minden elemet vegignez!)
	int			GetMinimalItemIndex() const;
	int			GetMinimalItemIndex( bool (* pCompareFunc)( const T& item1, const T& item2) ) const;

	// visszaadja a legnagyobb elemet (minden elemet vegignez!)
	int			GetMaximalItemIndex() const;
	int			GetMaximalItemIndex( bool (* pCompareFunc)( const T& item1, const T& item2) ) const;

	// megforditja az elemek sorrendjet
	void		MakeReverse();

	// megallapitja, ugyanazokat az elemeket tartalmazza-e, mint a parameterul megadott tomb
	bool		ContainsSameElements( const A2Array<T>& src ) const;

	// megallapitja, ugyanazok az elemek ugyanabban a sorrendben szerepelnek-e
	bool		operator==( const A2Array<T>& src ) const;

	// rendezes
	void		Sort();

	// rendezes adott fuggvennyel
	void		Sort( bool (* _Comp)( const T& item1, const T& item2) );

	// ertekadas
	A2Array<T>&	operator=( const A2Array<T>& src );

	// rendezi a tombot es kiszuri a duplikatumokat
	void		SortAndRemoveDuplicates();

	// lefoglal elõre adott méretet
	void		Reserve( DWORD nCount );

	// elemek torlese masik rendezett tomb adott range-e alapjan 
	void		Delete( const A2Array< T >& array, int nFrom, int nTo );

	// visszaadja, rendezett tombrol van-e szo
	bool		IsSorted() const;

	array_const_iterator	GetBegin() const;
	array_const_iterator	GetEnd() const;

	array_iterator	GetBegin();
	array_iterator	GetEnd();

	DWORD		size()	const;

protected:	
	bool			m_bSorted;
	DWORD			m_nReserved;
};

template< class T >
A2Array<T>::A2Array()
: m_bSorted( true )
, m_nReserved( 0 )
{
}

template< class T >
A2Array<T>::A2Array( T item )
: m_bSorted( true )
, m_nReserved( 0 )
{
	Add( item );
}

template< class T >
A2Array<T>::~A2Array()
{
	Clear();
}

template< class T >
A2Array<T>& A2Array<T>::operator=( const A2Array<T>& src )
{	
	m_bSorted = src.m_bSorted;
	std::vector< T >::operator=( src );

	return *this;
}

template< class T >
void A2Array<T>::Reserve( DWORD nCount )
{
	reserve( nCount );
	m_nReserved = nCount;
}

template< class T >
DWORD A2Array<T>::size()	const
{
	return (DWORD)__super::size();
}


/** Visszaadja azt a tombot, amelyet ugy kapunk, hogy ebbol kitoroljuk a parameterul kapott tomb elemeit.

\param aOther A tomb, amelynek elemeit ki kell torolni.
\return Az eredmenyul kapott tomb.
*/
template< class T >
A2Array< T >	A2Array<T>::operator-( const A2Array< T >& aOther ) const
{
	A2Array< T >	aRet = *this;

	aRet.Sort();

	for( int i = 0 ; i < aOther.GetCount(); i++ )
	{
		aRet.DeleteValue( aOther[ i ] );
	}

	return aRet;
}

template< class T >
void A2Array<T>::Difference( const A2Array<T>& aOther, A2Array< int >& aOnlyInThis, A2Array< int >& aOnlyInThat, A2Array< int >& aCommon ) const
{
	A2Array< int >	aCommonIndexesInThat;	// a kozos elemek indexei a masik tombben

	// elokeszites
	aOnlyInThis.Clear();
	aOnlyInThat.Clear();
	aCommon.Clear();

	// vegigmegyunk ennek a tombnek az elemein
	for( DWORD i=0; i<size(); i++ )
	{
		int	nIndexInThat = aOther.GetIndex( at( i ) );

		if( nIndexInThat != -1 )
		{
			// kozos elem
			aCommon.Add( i );
			aCommonIndexesInThat.Add( nIndexInThat );
		}
		else
		{
			// csak ebben a tombben van benne
			aOnlyInThis.Add( i );
		}
	}

	// ha nem maradt a masik tombben olyan elem ami nem kozos, akkor kesz
	if( aCommonIndexesInThat.GetCount() == aOther.GetCount() )
	{
		return;
	}

	 // osszeszedjuk az osszes indexet a masik tombhoz
	for( int i=0; i<aOther.GetCount(); i++ )
	{
		aOnlyInThat.Add( i );
	}

	// majd kitoroljuk belole azokat, amik a kozos elemek indexei
	aOnlyInThat.Delete( aCommonIndexesInThat );
}

template< class T >
int A2Array<T>::Add( const T item )
{
	// ha kell helyet foglalni, akkor progresszivan novo modon elore helyet foglalunk elemeknek
	if( m_nReserved <= size() )
	{
		Reserve( m_nReserved + 1 + m_nReserved / 3 );
	}

	push_back( item );

	// ha ez az elso elem, akkor meg rendezettnek szamit, kulonben nem
	m_bSorted = ( size() == 0 );	

	//nem tudom mi ertelme van ennek, de mashol igy varjuk
	return (int)size() -1;
}

template< class T >
int A2Array<T>::SortedAdd( const T item )
{
	if( !IsSorted() )
	{
		Sort();
	}

	// megkeressuk a helyet, ahova be kell szurni
	int	nIndex = GetInsertIndexLogSearch( item );

	std::vector<T>::iterator	iter = insert( begin() + nIndex, item );

	return (int)( iter - begin() );
}

template< class T >
int A2Array<T>::AddNoDuplicates( const T item )
{
	if( GetIndex( item ) != -1 ) return -1;

	return Add( item );
}

template< class T >
int A2Array<T>::SortedAddNoDuplicates( const T item )
{
	if( !IsSorted() )
	{
		Sort();
	}

	if( GetIndexLogSearch( item ) != -1 ) return -1;

	return SortedAdd( item );
}

template< class T >
void A2Array<T>::Add( const A2Array<T> array, bool fDuplicatesEnabled /*=true*/ )
{
	// nem biztos, hogy szukseg lesz ennyi helyre, de sebaj
	DWORD	nReservedNow = m_nReserved;
	Reserve( GetCount() + array.GetCount() );

	for( int i=0; i<array.GetCount(); i++ )
	{
		// ha a duplikatumok nem engedelyezettek, akkor elobb vegignezzuk,
		// szerepel-e mar a tombben az adott elem.
		if( !fDuplicatesEnabled )
		{
			if( GetIndex( array[i] ) != -1 )
			{
				continue;
			}
		}

		Add( array[i] );
	}

	Reserve( nReservedNow );
}

/** egy ugyanilyen elemekbol allo masik A2Array hozzaadasa */
template< class T >
void A2Array<T>::operator+=( const A2Array<T> array )
{
	Add( array );
}

template< class T >
void A2Array<T>::SortedAdd( const A2Array<T> array, bool fDuplicatesEnabled /*=true*/ )
{
	if( !IsSorted() )
	{
		Sort();
	}

	// nem biztos, hogy szukseg lesz ennyi helyre, de sebaj
	DWORD	nReservedNow = m_nReserved;
	Reserve( GetCount() + array.GetCount() );

	for( int i=0; i<array.GetCount(); i++ )
	{
		// ha a duplikatumok nem engedelyezettek, akkor elobb vegignezzuk,
		// szerepel-e mar a tombben az adott elem.
		if( !fDuplicatesEnabled )
		{
			if( GetIndexLogSearch( array[i] ) != -1 )
			{
				continue;
			}
		}

		SortedAdd( array[i] );
	}

	Reserve( nReservedNow );
}

template< class T >
int A2Array<T>::GetCount() const
{
	return int( size() );
}

template< class T >
bool A2Array<T>::IsValidIndex( int nIndex ) const
{
	return ( nIndex >= 0 && nIndex < int( size() ) );
}

template< class T >
void A2Array<T>::Clear()
{
	clear();
	m_bSorted = true;
}

template< class T >
void A2Array<T>::ClearPointers()
{
	for( std::vector<T>::iterator it = begin(); it !=  end(); it++ )
	{
		SAFE_DELETE( *it );
	}

	Clear();
}

template< class T >
T& A2Array<T>::operator[]( int nIndex )
{
	return std::vector< T >::operator[]( nIndex );
}

template< class T >
const T& A2Array<T>::operator[]( int nIndex ) const
{
	return (const T&)std::vector< T >::operator[]( nIndex );
}

template< class T >
T& A2Array<T>::Item( int nIndex )
{
	return at( nIndex );
}

template< class T >
const T& A2Array<T>::Item( int nIndex ) const
{
	return at( nIndex );
}

template< class T >
void A2Array<T>::Delete( int nIndex )
{
	unsigned int uSize = (unsigned int)size();

	if( uSize == 0 || uSize <= (unsigned int)nIndex )
	{
		throw _T("A2Array does not contain this index!");
		return;
	}

	erase( begin() + nIndex );

	// ha mar csak egyetlen (vagy nulla) elem maradt, akkor "rendezette valik" a tomb
	if( size() <= 1 )
	{
		m_bSorted = true;
	}
}

template< class T >
void A2Array<T>::DeletePointer( int nIndex )
{
	unsigned int uSize = (unsigned int)size();

	if( uSize == 0 || uSize <= (unsigned int)nIndex )
	{
		throw _T("A2Array does not contain this index!");
		return;
	}

	SAFE_DELETE( at( nIndex ) );
	erase( begin() + nIndex );

	// ha mar csak egyetlen (vagy nulla) elem maradt, akkor "rendezette valik" a tomb
	if( size() <= 1 )
	{
		m_bSorted = true;
	}
}


template< class T >
void A2Array<T>::Delete( int nStartIndex, int nEndIndex )
{
	if( nStartIndex > nEndIndex )
	{
		int nTmp = nStartIndex;
		nStartIndex = nEndIndex;
		nEndIndex = nTmp;
	}
	else if( nStartIndex == nEndIndex )
	{
		return;
	}

	unsigned int uSize = size();

	if( uSize == 0 || uSize <= (unsigned int)nStartIndex )
	{
		throw _T("A2Array does not contain this index!");
		return;
	}

	if( (unsigned int)nEndIndex >= uSize )
	{
		nEndIndex = (int)uSize - 1;
	}

	erase( begin() + nStartIndex, begin() + nEndIndex );
	
	// ha mar csak egyetlen (vagy nulla) elem maradt, akkor "rendezette valik" a tomb
	if( size() <= 1 )
	{
		m_bSorted = true;
	}
}

template< class T >
void A2Array<T>::Delete( array_iterator startiter, array_iterator enditer )
{
	if( startiter > enditer )
	{
		array_iterator tmpiter = startiter;
		startiter = enditer;
		enditer = tmpiter;
	}
	else if( startiter == enditer )
	{
		return;
	}

	erase( startiter, enditer );
	
	// ha mar csak egyetlen (vagy nulla) elem maradt, akkor "rendezette valik" a tomb
	if( size() <= 1 )
	{
		m_bSorted = true;
	}
}

template< class T >
typename A2Array<T>::array_iterator A2Array<T>::Delete( array_iterator iter )
{		
	if( size() <= 1 )
	{
		m_bSorted = true;
	}

	return erase( iter );
}

template< class T >
typename A2Array<T>::array_iterator A2Array<T>::DeletePointer( array_iterator iter )
{		
	if( size() <= 1 )
	{
		m_bSorted = true;
	}

	SAFE_DELETE( (*iter) );
	return erase( iter );
}


template< class T >
void A2Array<T>::Delete( const A2Array< T >& array )
{
	for( int i=0; i<array.GetCount(); i++ )
	{
		int	nIndex = GetIndex( array[ i ] );

		if( nIndex == -1 ) continue;

		Delete( nIndex );
	}
}

template< class T >
void A2Array<T>::SortedDelete( const A2Array< T >& array )
{
	for( int i=0; i<array.GetCount(); i++ )
	{
		int	nIndex = GetIndexLogSearch( array[ i ] );

		if( nIndex == -1 ) continue;

		Delete( nIndex );
	}
}

template< class T >
void A2Array<T>::DeleteValue( const T item )
{
	int	nIndex = GetIndex( item );

	if( nIndex == -1 ) return;

	Delete( nIndex );
}

template< class T >
void A2Array<T>::SortedDeleteValue( const T item )
{
	int	nIndex = GetIndexLogSearch( item );

	if( nIndex == -1 ) return;

	Delete( nIndex );
}

template< class T >
int A2Array<T>::GetIndex( const T& item, int nSearchFrom /*=0*/ ) const
{
	// ha nincs egyetlen elem sem benne, akkor visszaterunk
	if( GetCount() == 0 ) return -1;

	int								nIndex = nSearchFrom;
	std::vector<T>::const_iterator	Iter = begin();

	for ( Iter += nIndex; Iter != end() ; Iter++, nIndex++ )
	{
		if( *Iter == item )
		{
			return nIndex;
		}
	}

	return -1;
}

template< class T >
int A2Array<T>::GetIndexLogSearch( const T& item, int nSearchFrom /*=0*/ ) const
{
	// ha nincs egyetlen elem sem benne, akkor visszaterunk
	if( GetCount() == 0 ) return -1;

	// ha nem rendezett, akkor a sima keresest hasznaljuk
	if( !m_bSorted )
	{
		return GetIndex( item );
	}

	int		nBegin = nSearchFrom;
	int		nEnd =(int)size() - 1;

	static bool bTrukk = true;
	while( bTrukk )
	{
		// amit megvizsgalunk: a kozepso elem
		int	nActual = (nEnd + nBegin) / 2;

		const T& itemActual = at( nActual );

		if( itemActual == item )
		{
			// megtalaltuk
			return nActual;
		}

		if( nBegin == nEnd )
		{
			return -1;
		}
		
		// vajon kisebb az adott elem?
		if( itemActual < item )
		{
			nBegin = nActual + 1;
		}
		else
		{
			nEnd = nActual;
		}
	}

	//nem talaltuk meg
	return -1;
}

template< class T >
int A2Array<T>::GetInsertIndexLogSearch( const T& item )
{
	// ha nincs egyetlen elem sem a tombben, akkor egyertelmu, hova kell beszurni
	if( GetCount() == 0 )
	{
		return 0;
	}

	// ha nem rendezett, akkor rendezzuk
	if( !IsSorted() )
	{
		Sort();
	}

	// az elso elem ele kell tenni?
    if( item < at( 0 ) || item == at( 0 ) )
	{
		return 0;
	}

	// az utolso elem utan kell tenni?
	if( at( size() - 1 ) < item || at( size() - 1 ) == item )
	{
		return (int)size();
	}

	// keresunk egy olyan poziciot, ami utan nagyobb elem van, mint a megadott elem
	int		nBegin = 1;
	int		nEnd =(int)size() - 1;

	while( nBegin < 55555555 ) //hulye fordito nem engedi a while(true)-t
	{
		// amit megvizsgalunk: a kozepso elem
		int	nActual = (nEnd + nBegin) / 2;

		const T& itemRight = at( nActual );

		if( itemRight == item )
		{
			// ide pont jo lesz beszurni
			return nActual;
		}

		// vajon kisebb az adott elem?
		// akkor tovabb kell mennunk jobbra
		if( itemRight < item )
		{
			nBegin = nActual + 1;
			continue;
		}

		// tehat a jobbra levo elem nagyobb vagy egyenlo
		// meg kell vizsgalnunk, mi a helyzet a balra fekvo elemmel, 
		// abban az esetben ha van balra elem es nem ez az utolso. ha az akkor megtalaltuk a helyet amit kerestunk.
		//const T& itemLeft = at( nActual - 1 );

		// ha bezarult az intervallum
		if( nBegin >= nEnd )
		{
			// megtalaltuk az idealis helyet a beszurasra
			return nActual;
		}

		// tehat balra kell tovabb keresgelnunk
		nEnd = nActual;
	}

	// ide nem juthat el a program futasa...
	ASSERT( 0 );
	return -1;
}

// visszaadja a legkisebb elemet (minden elemet vegignez!)
template< class T >
int A2Array<T>::GetMinimalItemIndex( bool (* pLesserFunc)( const T& item1, const T& item2) ) const
{
	if( GetCount() == 0 )
	{
		return -1;
	}

	// feltesszuk, hogy az elso a legkisebb
	int		nIndex = 0;

	// vegignezzuk a tobbit
	for( int i = 1; i < GetCount(); i++ )
	{
		// ha kisebb, mint az eddigi legkisebb, akkor megjegyezzuk
		if( (*pLesserFunc)( at( i ), at( nIndex ) ) )
		{
			nIndex = i;
		}
	}

	return nIndex;
}

// visszaadja a legkisebb elemet (minden elemet vegignez!)
template< class T >
int A2Array<T>::GetMinimalItemIndex() const
{
	if( GetCount() == 0 )
	{
		return -1;
	}

	// feltesszuk, hogy az elso a legkisebb
	int		nIndex = 0;

	// vegignezzuk a tobbit
	for( int i = 1; i < GetCount(); i++ )
	{
		// ha kisebb, mint az eddigi legkisebb, akkor megjegyezzuk
		if( at( i ) < at( nIndex ) )
		{
			nIndex = i;
		}
	}

	return nIndex;
}

// visszaadja a legnagyobb elemet (minden elemet vegignez!)
template< class T >
int A2Array<T>::GetMaximalItemIndex( bool (* pGreaterFunc)( const T& item1, const T& item2) ) const
{
	if( GetCount() == 0 )
	{
		return -1;
	}

	// feltesszuk, hogy az elso a legnagyobb
	int		nIndex = 0;

	// vegignezzuk a tobbit
	for( int i = 1; i < GetCount(); i++ )
	{
		// ha nagyobb, mint az eddigi legnagyobb, akkor megjegyezzuk
		if( (*pGreaterFunc)( at( i ), at( nIndex ) ) )
		{
			nIndex = i;
		}
	}

	return nIndex;
}

// visszaadja a legnagyobb elemet (minden elemet vegignez!)
template< class T >
int A2Array<T>::GetMaximalItemIndex() const
{
	if( GetCount() == 0 )
	{
		return -1;
	}

	// feltesszuk, hogy az elso a legnagyobb
	int		nIndex = 0;

	// vegignezzuk a tobbit
	for( int i = 1; i < GetCount(); i++ )
	{
		// ha nagyobb, mint az eddigi legnagyobb, akkor megjegyezzuk
		if( at( i ) > at( nIndex ) )
		{
			nIndex = i;
		}
	}

	return nIndex;
}

template< class T >
void A2Array<T>::MakeReverse()
{
	DWORD	nCountHalf = size() / 2;
	T	tmp;

	for( DWORD i=0; i<nCountHalf && i != (size() - i - 1); i++ )
	{
		tmp = at(i);
		at(i) = at( size() - i - 1 );
		at( size() - i - 1 ) = tmp;
	}

	m_bSorted = false;
}

template< class T >
bool A2Array<T>::Contains( const T& item ) const
{
	return( GetIndex( item ) != -1 );
}

template< class T >
bool A2Array<T>::SortedContains( const T& item ) const
{
	if( m_bSorted )
	{
		return( GetIndexLogSearch( item ) != -1 );
	}
	else
	{
		return( GetIndex( item ) != -1 );
	}
}

template< class T >
int A2Array<T>::Count( const T& item ) const
{
	int nCount = 0;

	std::vector<T>::const_iterator Iter;

	for ( Iter = begin() ; Iter != end() ; Iter++ )
	{
		if( *Iter == item )
		{
			nCount++;
		}
	}

	return nCount;
}

template< class T >
int A2Array<T>::Insert( int nIndex, const T item )
{
	std::vector<T>::iterator	iter = insert( begin() + nIndex, item );

	// ha ez az elso elem, amit beteszunk, akkor nem rontja el a rendezettseget
	m_bSorted = ( size() == 1 );

	return (int)( iter - begin() );
}

template< class T >
int A2Array<T>::InsertNoDuplicates( int nIndex, const T item )
{
	if( GetIndex( item ) != -1 ) return -1;

	return Insert( nIndex, item );
}

template< class T >
bool A2Array<T>::ContainsSameElements( const A2Array<T>& src ) const
{
	int	nCount = GetCount();

	// letszam-egyeztetes
	if( nCount != src.GetCount() ) return false;

	for( int i=0; i<nCount; i++ )
	{
		if( !src.Contains( at( i ) ) )
		{
			return false;
		}
	}

	return true;
}

template< class T >
bool A2Array<T>::operator==( const A2Array<T>& src ) const
{
	int	nCount = GetCount();

	// letszam-egyeztetes
	if( nCount != src.GetCount() ) return false;

	for( int i=0; i<nCount; i++ )
	{
		if( !( src[ i ] == at( i ) ) )
		{
			return false;
		}
	}

	return true;
}

template< class T >
void A2Array<T>::Sort()
{
	sort( begin(), end() );
	m_bSorted = true;
}

template< class T >
void A2Array<T>::Sort( bool (* _Comp)( const T& item1, const T& item2) )
{
	sort( begin(), end(), _Comp );
	m_bSorted = true;
}


template< class T >
void A2Array<T>::SortAndRemoveDuplicates()
{
	Sort();

	int		nExamineIndex = 1;		// eppen ez a poziciot vizsgalja
	int		nDuplicateCount = 0;	// az eddig talalt duplikatumok szama
	int		nCount = GetCount();	// az elemek szama a tombben

	for( nExamineIndex = 1; nExamineIndex < nCount; nExamineIndex++ )
	{
		bool	fDuplicate = ( at( nExamineIndex - 1 ) == at( nExamineIndex ) );

		// ha duplikatum, akkor noveljuk eggyel a duplikatumok szamlalojat, mas teendonk nincsen
		if( fDuplicate )
		{
			nDuplicateCount++;
			continue;
		}

		// ha meg eddig egyetlen duplikatumot sem talaltunk, akkor mehetunk tovabb
		if( nDuplicateCount == 0 ) continue;

		// ha viszont mar volt duplikatum, akkor ezt az elemet elobbre kell mozgatnunk
		// megpedig eppen annyival ahany duplikatumot talaltunk eddig (mert ennyi elem fog kiesni elole)
		at( nExamineIndex - nDuplicateCount ) = at( nExamineIndex );
	}

	// torlunk a tomb vegerol megfelelo szamu elemet (ahany duplikatumot talaltunk)
	for( int i = 0; i < nDuplicateCount; i++ )
	{
		Delete( nCount - 1 );
		nCount--;
	}
}

template< class T >
void A2Array<T>::Delete( const A2Array< T >& array, int nFrom, int nTo )
{
	if (nFrom >= GetCount() )
	{
		return;
	}

	if ( nTo >= GetCount() )
	{
		nTo = GetCount() - 1;
	}

	if ( m_bSorted )
	{
		for( int i=nFrom; i < nTo; i++ )
		{
			int	nIndex = GetIndexLogSearch( array[ i ] );

			if( nIndex == -1 ) continue;

			Delete( nIndex );
		}
	}
	else
	{
		for( int i=nFrom; i < nTo; i++ )
		{
			int	nIndex = GetIndex( array[ i ] );
	
			if( nIndex == -1 ) continue;

			Delete( nIndex );
		}
	}
}

template< class T >
bool A2Array<T>::IsSorted() const
{
	return m_bSorted;
}

template< class T >
typename A2Array<T>::array_const_iterator A2Array<T>::GetBegin() const
{
	return begin();
}

template< class T >
typename A2Array<T>::array_const_iterator A2Array<T>::GetEnd() const
{
	return end();
}

template< class T >
typename A2Array<T>::array_iterator A2Array<T>::GetBegin()
{
	return begin();
}

template< class T >
typename A2Array<T>::array_iterator A2Array<T>::GetEnd()
{
	return end();
}
