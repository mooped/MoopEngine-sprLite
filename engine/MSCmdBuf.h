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
#define MAX_INDIVIDUAL_COMMAND 1024

#include "MSPoint.h"
#include "MSMutex.h"

namespace MSCmdBuf
{
	struct CmdBuffer
	{
		CmdBuffer( void ) : valid_length( 0 ), length( 0 ) {}

		unsigned int buffer[MAX_RENDER_COMMANDS];
		unsigned int valid_length;	// Data in the buffer that is complete
		unsigned int length;	// Data in the buffer
		MSMutex lock;
		bool rendering;	// True if rendering from, otherwise writing to
	};

	void SetMultithread( bool mt );
	bool IsMultithreaded();

	void LockForWriting();
	void UnlockForWriting();
	void FinishedRendering();
	void FinishedWriting();
	void SwapRenderingBuffer();

	void Clear();
	void Extend( unsigned int size );

	void AddCommand( unsigned int cmd );
	void AddInt( int i );
	void AddUInt( unsigned int u );
	void AddFloat( float f );
	void AddVec( const MSVec& vec );
	void AddPointer( void* const pPtr );
	void AddData( const void* const pPtr, unsigned int size );
	void AddString( const char* __restrict const pStr );

	void Dispatch();

	CmdBuffer* GetBuffer();
};

#endif

