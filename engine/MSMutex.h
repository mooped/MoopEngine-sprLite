/*
 *  MSMutex.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MSMUTEX_DEF
#define MSMUTEX_DEF

#include "MSEngine.h"

// Atomic operations for various platforms

#ifdef WIN32

#include <windows.h>

namespace MSAtomic
{
	inline int Increment( int *value )
	{
		return InterlockedIncrement( reinterpret_cast<LPLONG>( value ) );
	}
	
	inline int Decrement( int *value )
	{
		return InterlockedDecrement( reinterpret_cast<LPLONG>( value ) );
	}
	
	inline bool CompareExchange( int oldValue, int newValue, int* value )
	{
		return InterlockedCompareExchange( reinterpret_cast<LPLONG>( value ), static_cast<LONG>( newValue ), static_cast<LONG>( oldValue ) ) == 0;
	}
};

#elif defined(LINUX)

// Use GCC builtin atomics on Linux
namespace MSAtomic
{
	inline int Increment( int *value )
	{
		__sync_fetch_and_add( value, 1 );
	}
	
	inline int Decrement( int *value )
	{
		__sync_fetch_and_sub( value, 1 );
	}
	
	inline bool CompareExchange( int oldValue, int newValue, int* value )
	{
		return __sync_val_compare_and_swap( value, oldValue, newValue );
	}
};

#elif defined(MACOSX)
// See: man atomic

#include <libkern/OSAtomic.h>

namespace MSAtomic
{
	inline int Increment( volatile int *value )
	{
		return OSAtomicIncrement32Barrier( value );
	}

	inline int Decrement( volatile int *value )
	{
		return OSAtomicDecrement32Barrier( value );
	}
	
	inline bool CompareExchange( int oldValue, int newValue, volatile int* value )
	{
		return OSAtomicCompareAndSwapIntBarrier( oldValue, newValue, value );
	}
};

#endif

// Mutex

class MSMutex
{
	int m_locked;

public:
	MSMutex( bool locked = false )
	: m_locked( ( locked == true ) ? 1 : 0 )
	{
	}

	~MSMutex()
	{
		Unlock();
	}

	void Lock()
	{
		while ( MSAtomic::CompareExchange( 0, 1, &m_locked ) != 0 );
	}

	bool TryLock()
	{
		return ( MSAtomic::CompareExchange( 0, 1, &m_locked ) == 0 );
	}

	void Unlock()
	{
		MSAtomic::CompareExchange( 1, 0, &m_locked );
	}
};

#endif
