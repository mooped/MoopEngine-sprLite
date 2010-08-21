/*
 *  MSRenderThread.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MSRenderThread.h"

#include "MSLauncher.h"
#include "MSCmdBuf.h"
#include "MSRender.h"

// System libraries
#if defined(WIN32)
#include "GL.h"
#include "GLU.h"
#include "GLEXT.h"  // For GL_BGRA among other things
#elif defined(MACOSX)
#include "OpenGL/GL.h"
#include "OpenGL/GLU.h"
#endif

namespace MSRenderThread
{
	// Command implementations
	void BeginScene();
	void EndScene();
	void Sync();

	void ClearColour( Colour rgba );

	void SetTexture( class MSImage* pImage );
	void Quad( MSVec verts[4], int layer, MSVec uvs[2] );

	bool ProcessCommand();

	u_int s_curCmd = 0;
	Colour s_clearcolour;
	unsigned int s_width;
	unsigned int s_height;
};

void MSRenderThread::BeginScene()
{
	// Clear screen
	const unsigned char ucRed = ( (s_clearcolour & 0xff000000) >> 24 );
	const unsigned char ucGreen = ( (s_clearcolour & 0x00ff0000) >> 16 );
	const unsigned char ucBlue = ( (s_clearcolour & 0x0000ff00) >> 8 );
	const unsigned char ucAlpha = ( s_clearcolour & 0x000000ff );
	// ToDo: Avoid float conversion
	glClearColor( ucRed, ucGreen, ucBlue, ucAlpha );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Fixed function pixel state
	glEnable( GL_DEPTH_TEST );
	glAlphaFunc( GL_GREATER, 0.1f );
	glEnable( GL_ALPHA_TEST );

	// Fixed function vertex state
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 256.0f );
}

void MSRenderThread::EndScene()
{
	MSLauncher::SwapBuffers();
}

void MSRenderThread::Sync()
{

}

void MSRenderThread::ClearColour( Colour rgba )
{
	s_clearcolour = rgba;
}

void MSRenderThread::SetTexture( class MSImage* pImage )
{

}

void MSRenderThread::Quad( MSVec verts[4], int layer, MSVec uvs[2] )
{

}

void MSRenderThread::CreateRT()
{
	MSLauncher::Launch( &ProcessCommands );
}

bool MSRenderThread::ProcessCommand()
{
	unsigned int &idx = s_curCmd;
	if ( idx >= MSCmdBuf::GetBuffer()->valid_length )
	{
		MSCmdBuf::Clear();
		idx = 0;
		return false;
	}
	MSRender::ECmd cmd = static_cast<MSRender::ECmd>( MSCmdBuf::GetBuffer()->buffer[idx++] );
	switch( cmd )
	{
		case MSRender::eCmd_BeginScene:
		{
			BeginScene();
		} break;
		case MSRender::eCmd_EndScene:
		{
			EndScene();
		} break;
		case MSRender::eCmd_Sync:
		{
			Sync();
		} break;
		case MSRender::eCmd_ClearColour:
		{
			Colour rgba = MSCmdBuf::GetBuffer()->buffer[idx++];
			ClearColour( rgba );
		} break;
		case MSRender::eCmd_SetTexture:
		{
			// Skip over a pointer
			idx += sizeof( void* ) / sizeof( u_int );
		} break;
		case MSRender::eCmd_Quad:
		{
			// Skip over the quad
			idx += 7;
		} break;
		default:
		{
			MASSERT( 0, "Unexpected render command." );
		} break;
	}

	return true;
}

void MSRenderThread::ProcessCommands()
{
	// ProcessCommands
	while ( ProcessCommand() );

	// TESTESTEST
	MSRender::ClearColour( 0xff00ffff );;
	MSRender::BeginScene();
	MSRender::EndScene();

	MSLauncher::RequestRedisplay();
}

