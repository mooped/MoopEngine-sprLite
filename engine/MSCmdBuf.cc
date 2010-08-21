/*
 *  MSCmdBuf.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MSCmdBuf.h"

#include "MSEngine.h"

#include <string.h>

// Statics
namespace MSCmdBuf
{
	MSCmdBuf::CmdBuffer s_buffers[2];
	int s_cmdIdx = 0;
	int s_renderIdx = 1;
	MSCmdBuf::CmdBuffer* s_commands = &s_buffers[s_cmdIdx];
	MSCmdBuf::CmdBuffer* s_rendering = &s_buffers[s_renderIdx];
}

// Locking
void MSCmdBuf::LockForWriting()
{
	s_commands->lock.Lock();
	if ( s_commands->rendering )
	{
		s_commands->lock.Unlock();
		s_cmdIdx = ( s_cmdIdx == 0 ) ? 1 : 0;
		s_commands = &s_buffers[s_cmdIdx];
		s_commands->lock.Lock();
		if ( s_commands->rendering )
		{
			s_commands->lock.Unlock();
			while ( s_commands->rendering );
			LockForWriting();
		}
	}
}

void MSCmdBuf::UnlockForWriting()
{
	s_commands->lock.Unlock();
}

void MSCmdBuf::FinishedRendering()
{
	s_rendering->rendering = false;
}

void MSCmdBuf::FinishedWriting()
{
	s_commands->rendering = true;
}

void MSCmdBuf::SwapRenderingBuffer()
{
	s_renderIdx = ( s_renderIdx == 0 ) ? 1 : 0;
	s_rendering = &s_buffers[s_cmdIdx];
}

// Command buffer implementation
void MSCmdBuf::Clear()
{
	s_commands->length = 0;
	s_commands->valid_length = 0;
}

void MSCmdBuf::Extend( unsigned int size )
{
	const unsigned int maxSize = MAX_RENDER_COMMANDS + MAX_INDIVIDUAL_COMMAND;
	MASSERT( s_commands->length + size < maxSize, "Command buffer is full." );
	// ToDo: Just flush if full or should this be dealt with elsewhere?
	s_commands->length += size;
}

void MSCmdBuf::AddCommand( unsigned int cmd )
{
	*(unsigned int*)(&s_commands->buffer[s_commands->length]) = cmd;
	Extend( 1 );
}

void MSCmdBuf::AddInt( int i )
{
	*(int*)(&s_commands->buffer[s_commands->length]) = i;
	Extend( 1 );
}

void MSCmdBuf::AddUInt( unsigned int u )
{
	*(unsigned int*)(&s_commands->buffer[s_commands->length]) = u;
	Extend( 1 );
}

void MSCmdBuf::AddFloat( float f )
{
	*(float*)(&s_commands->buffer[s_commands->length]) = f;
	Extend( 1 );
}

void MSCmdBuf::AddVec( const MSVec& vec )
{
	const unsigned int length = s_commands->length;
	*(int*)(&s_commands->buffer[length + 0]) = vec.x;
	*(int*)(&s_commands->buffer[length + 1]) = vec.y;
	Extend( 2 );
}

void MSCmdBuf::AddPointer( void* const pPtr )
{
	const unsigned int length = s_commands->length;
	*(void**)(&s_commands->buffer[length]) = pPtr;
	Extend( sizeof( void* ) / sizeof( u_int ) );
}

void MSCmdBuf::AddData( const void* const pPtr, unsigned int size )
{
	const unsigned int length = s_commands->length;
	MASSERT( (size & 0x3) == 0, "Command buffer must be 4 byte aligned." );
	memcpy( s_commands->buffer + length, pPtr, size );
	Extend( size / 4 );
}

void MSCmdBuf::AddString( const char* __restrict const pStr )
{
	const unsigned int length = s_commands->length;
	unsigned int size = strlen( pStr ) + 1;
	strcpy( reinterpret_cast<char*>( s_commands->buffer + length ), pStr );
	size = size + PADDINGP2( size, 4 );	// Pad to 4 bytes
	Extend( size / 4 );
}

void MSCmdBuf::Dispatch( void )
{
	s_commands->valid_length = s_commands->length;
}

MSCmdBuf::CmdBuffer* MSCmdBuf::GetBuffer()
{
	return s_rendering;
}

