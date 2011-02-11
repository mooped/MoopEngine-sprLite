/*
 *  MSCmdBuf.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#include "MSCmdBuf.h"

#include "MSEngine.h"

#include <string.h>

// Statics
namespace MSCmdBuf
{
	MSCmdBuf::CmdBuffer s_buffer;
	MSCmdBuf::CmdBuffer* s_commands = &s_buffer;
	bool multithread = false;
}

void MSCmdBuf::SetMultithread( bool mt )
{
	multithread = mt;
}

bool MSCmdBuf::IsMultithreaded()
{
	return multithread;
}

// Command buffer implementation
void MSCmdBuf::Clear()
{
	s_commands->start = 0;
	s_commands->end = 0;
	s_commands->working = 0;
}

void MSCmdBuf::Check( unsigned int size )
{
	const unsigned int maxSize = MAX_RENDER_COMMANDS;
	// Next command will hit the end?
	if ( s_commands->end + size >= maxSize )
	{
		const unsigned int diff = maxSize - s_commands->end;
		if ( diff == 1 )
		{
			AddNOP();
		}
		else
		{
			AddSKP( diff );
		}
	}
}

void MSCmdBuf::Extend( unsigned int size )
{
	const unsigned int maxSize = MAX_RENDER_COMMANDS;
	s_commands->working += size;
	if ( s_commands->working >= maxSize )
	{
		s_commands->working = 0;
	}
}

void MSCmdBuf::AddCommand( unsigned int cmd )
{
	*(unsigned int*)(&s_commands->buffer[s_commands->working]) = cmd;
	Extend( 1 );
}

void MSCmdBuf::AddInt( int i )
{
	*(int*)(&s_commands->buffer[s_commands->working]) = i;
	Extend( 1 );
}

void MSCmdBuf::AddUInt( unsigned int u )
{
	*(unsigned int*)(&s_commands->buffer[s_commands->working]) = u;
	Extend( 1 );
}

void MSCmdBuf::AddFloat( float f )
{
	*(float*)(&s_commands->buffer[s_commands->working]) = f;
	Extend( 1 );
}

void MSCmdBuf::AddVec( const MSVec& vec )
{
	const unsigned int pos = s_commands->working;
	*(int*)(&s_commands->buffer[pos + 0]) = vec.x;
	*(int*)(&s_commands->buffer[pos + 1]) = vec.y;
	Extend( 2 );
}

void MSCmdBuf::AddPointer( void* const pPtr )
{
	const unsigned int pos = s_commands->working;
	*(void**)(&s_commands->buffer[pos]) = pPtr;
	Extend( sizeof( void* ) / sizeof( u_int ) );
}

void MSCmdBuf::AddData( const void* const pPtr, unsigned int size )
{
	const unsigned int pos = s_commands->working;
	MASSERT( (size & 0x3) == 0, "Command buffer must be 4 byte aligned." );
	memcpy( s_commands->buffer + pos, pPtr, size );
	Extend( size / sizeof( u_int ) );
}

void MSCmdBuf::AddString( const char* __restrict const pStr )
{
	const unsigned int pos = s_commands->working;
	unsigned int size = strlen( pStr ) + 1;
	strcpy( reinterpret_cast<char*>( s_commands->buffer + pos ), pStr );
	size = size + PADDINGP2( size, sizeof( u_int ) );	// Pad to 4 bytes
	Extend( size / sizeof( u_int ) );
}

void MSCmdBuf::AddNOP()
{
	AddCommand( eCmd_NOP );
}

void MSCmdBuf::AddSKP( unsigned int n )
{
	AddCommand( eCmd_SKP );
	AddUInt( n );
	for ( unsigned int u = 0; u < n - 2; ++u ) { AddNOP(); }
}

void MSCmdBuf::Dispatch()
{
	s_commands->end = s_commands->working;
}

MSCmdBuf::CmdBuffer* MSCmdBuf::GetBuffer()
{
	return s_commands;
}

