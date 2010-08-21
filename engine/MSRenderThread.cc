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
#include "MSImage.h"

// System libraries
#if defined(WIN32)
#include "GL.h"
#include "GLU.h"
#include "GLEXT.h"  // For GL_BGRA among other things
#elif defined(MACOSX)
#include "OpenGL/GL.h"
#include "OpenGL/GLU.h"
#endif

#include <string.h>

namespace MSRenderThread
{
	// Command implementations
	void BeginScene();
	void EndScene();
	void Sync();

	void ClearColour( Colour rgba );

	void SetTexture( class MSImage* pImage );
	void Quad( MSVec verts[4], int layer, MSVec uvs[2], Colour rgba );

	bool ProcessCommand();

	u_int s_curCmd = 0;
	Colour s_clearcolour = 0x000000ff;
	unsigned int s_width = 640;	// Virtual pixel dimensions
	unsigned int s_height = 480;
	MSImage* s_pImage = NULL;	// Currently bound image
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

	glEnable( GL_TEXTURE_2D );

	// Fixed function vertex state
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 256.0f );
}

void MSRenderThread::EndScene()
{
	glFlush();
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
	if ( pImage )
	{
		if ( pImage->GetRef() != ~0 )
		{
			glBindTexture( GL_TEXTURE_2D, pImage->GetRef() );
		}
		else
		{
			GLuint texName;
			glGenTextures( 1, &texName );

			glBindTexture( GL_TEXTURE_2D, texName );
			glTexImage2D( GL_TEXTURE_2D, 0, 4, pImage->GetWidth(), pImage->GetHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, pImage->GetData() );

			glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

			pImage->SetRef( texName );
		}
	}
	s_pImage = pImage;
}

void MSRenderThread::Quad( MSVec verts[4], int layer, MSVec uvs[2], Colour rgba )
{
	if ( s_pImage )
	{
		// Set colour
		const unsigned char ucRed = ( (rgba & 0xff000000) >> 24 );
		const unsigned char ucGreen = ( (rgba & 0x00ff0000) >> 16 );
		const unsigned char ucBlue = ( (rgba & 0x0000ff00) >> 8 );
		const unsigned char ucAlpha = ( rgba & 0x000000ff );
		glColor4ub( ucRed, ucGreen, ucBlue, ucAlpha );
	
		// Convert coordinates
		float x[4];
		float y[4];
		float z = -layer;
		float u[2];
		float v[2];
		float xStep = 1.0f / static_cast<float>( s_width );
		float yStep = 1.0f / static_cast<float>( s_height );
		float uStep = 1.0f / static_cast<float>( s_pImage->GetWidth() );
		float vStep = 1.0f / static_cast<float>( s_pImage->GetHeight() );
		for ( int i = 0; i < 4; ++i )
		{
			x[i] = verts[i].x * xStep;
			y[i] = verts[i].y * yStep;
		}
		for ( int i = 0; i < 2; ++i )
		{
			u[i] = uvs[i].x * uStep;
			v[i] = 1.0f - ( uvs[i].y * vStep );
		}
	
		// Draw the quad - expect clockwise winding
		glBegin( GL_QUADS );
		glTexCoord2f( u[0], v[0] );
		glVertex3f( x[0], y[0], z );
		glTexCoord2f( u[1], v[0] );
		glVertex3f( x[1], y[1], z );
		glTexCoord2f( u[1], v[1] );
		glVertex3f( x[2], y[2], z );
		glTexCoord2f( u[0], v[1] );
		glVertex3f( x[3], y[3], z );
		glEnd();
	}
	else
	{
		MASSERT( 0, "Drawing a quad with no image bound." );
	}
}

void MSRenderThread::CreateRT()
{
	MSLauncher::Launch( &ProcessCommands );
}

bool MSRenderThread::ProcessCommand()
{
	unsigned int &idx = s_curCmd;
	MSCmdBuf::CmdBuffer* buffer = MSCmdBuf::GetBuffer();
	buffer->lock.Lock();
	const bool rendering = buffer->rendering;
	if ( !rendering )
	{
		buffer->lock.Unlock();
		MSCmdBuf::SwapRenderingBuffer();
		return false;
	}
	if ( idx >= buffer->valid_length )
	{
		MSCmdBuf::Clear();
		idx = 0;
		buffer->lock.Unlock();
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
			MSImage* pImage = *( reinterpret_cast<MSImage**>( &buffer->buffer[idx] ) );
			idx += sizeof( MSImage* ) / sizeof( u_int );
			SetTexture( pImage );
		} break;
		case MSRender::eCmd_Quad:
		{
			MSVec verts[4];
			memcpy( verts, &MSCmdBuf::GetBuffer()->buffer[idx], sizeof( verts ) );
			idx += sizeof( verts ) / sizeof( u_int );
			int layer = static_cast<int>( buffer->buffer[idx++] );
			MSVec uvs[2];
			memcpy( uvs, &buffer->buffer[idx], sizeof( uvs ) );
			idx += sizeof( uvs ) / sizeof( u_int );
			Colour rgba = buffer->buffer[idx++];
			Quad( verts, layer, uvs, rgba );
		} break;
		default:
		{
			MASSERT( 0, "Unexpected render command." );
		} break;
	}

	buffer->lock.Unlock();
	return true;
}

void MSRenderThread::ProcessCommands()
{
	// ProcessCommands
	while ( ProcessCommand() );
	MSCmdBuf::FinishedRendering();

	MSLauncher::RequestRedisplay();
}

