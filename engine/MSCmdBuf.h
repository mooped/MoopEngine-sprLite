/*
 *  MSCmdBuf.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MCOMMANDBUFFER_DEF
#define MCOMMANDBUFFER_DEF

#define MAX_RENDER_COMMANDS 65536

#include "MSPoint.h"
#include "MSMutex.h"

namespace MSCmdBuf
{
	struct CmdBuffer
	{
		CmdBuffer( void ) : start( 0 ), end( 0 ), working( 0 ) {}

		unsigned int buffer[MAX_RENDER_COMMANDS];
		unsigned int start;
		unsigned int end;
		unsigned int working;
	};

	enum Commands
	{
		eCmd_NOP = 0,
		eCmd_SKP = 1,
	};

	void SetMultithread( bool mt );
	bool IsMultithreaded();

	void Clear();
	void Check( unsigned int size );
	void Extend( unsigned int size );

	void AddCommand( unsigned int cmd );
	void AddInt( int i );
	void AddUInt( unsigned int u );
	void AddFloat( float f );
	void AddVec( const MSVec& vec );
	void AddPointer( void* const pPtr );
	void AddData( const void* const pPtr, unsigned int size );
	void AddString( const char* __restrict const pStr );

	void AddNOP();
	void AddSKP( unsigned int n );

	void Dispatch();

	CmdBuffer* GetBuffer();
};

#endif

