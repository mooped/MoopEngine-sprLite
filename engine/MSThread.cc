/*
 *  MSThread.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MSThread.h"

#if defined( USE_PTHREADS )
#	include <pthread.h>
#elif defined( USE_WIN32THREADS )
#	include <windows.h>
#	include <process.h>
#else
#	warning "No threading support!"
#endif

#ifdef USE_PTHREADS

// PThreads user data
struct ThreadInstance
{
	pthread_t ref;
};

// Implementation
MSThread::MSThread( void )
: m_data( new ThreadInstance )
, m_kill( false )
, m_dead( false )
{
}

MSThread::~MSThread( void )
{
	m_kill = true;	// Request exit
	if ( m_data )
	{
		pthread_join( m_data->ref, NULL );
		while ( !m_dead );	// Wait for the thread to finish
		delete m_data;
	}
}

void MSThread::Run( void )
{
	pthread_attr_t attr;
	pthread_attr_init( &attr );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );
	if ( pthread_create( &m_data->ref, NULL, RunThread_Impl, this ) )
	{
		MASSERT( 0, "Create thread failed.\n" );
	}
	pthread_attr_destroy( &attr );
}

void MSThread::RunThread( void )
{
	Exit();
}

void MSThread::Exit( void )
{
	m_dead = true;	// Mark as dead
	pthread_exit( m_data );
}

void* MSThread::RunThread_Impl( void* pInst )
{
	static_cast<MSThread*>(pInst)->RunThread();
	return NULL;
}

void MSThread::RunThreadNoRV_Impl( void* pInst )
{
	RunThread_Impl( pInst );
}

#endif

#ifdef USE_WIN32THREADS
// Windows threads user data
struct ThreadInstance
{
	HANDLE ref;
};

// Implementation
MSThread::MSThread( void )
: m_data( new ThreadInstance )
, m_kill( false )
, m_dead( false )
{
}

MSThread::~MSThread( void )
{
	m_kill = true;	// Request exit
	if ( m_data )
	{
		CloseHandle( m_data->ref );
		delete m_data;
	}
}

void MSThread::Run( void )
{
#if MTHREAD_DEBUG
	fprintf( stderr, "Creating thread...\n" );
#endif
	m_data->ref = (HANDLE)_beginthread( &RunThreadNoRV_Impl, 0, this );
	if ( !m_data->ref )
	{
		MASSERT( 0, "Create thread failed.\n" );
	}
}

void MSThread::RunThread( void )
{
	Exit();
}

void MSThread::Exit( void )
{
	m_dead = true;	// Mark as dead
	_endthreadex( 0 );
}

void* MSThread::RunThread_Impl( void* pInst )
{
	static_cast<MSThread*>(pInst)->RunThread();
	return NULL;
}

void MSThread::RunThreadNoRV_Impl( void* pInst )
{
	RunThread_Impl( pInst );
}

#endif

