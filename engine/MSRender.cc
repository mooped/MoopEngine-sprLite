/*
 *  MSRender.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MSRender.h"
#include "MSLauncher.h"

#include "MSCmdBuf.h"

// System libraries
#if defined(WIN32)
#include "GL.h"
#include "GLU.h"
#include "GLEXT.h"  // For GL_BGRA among other things
#elif defined(MACOSX)
#include "OpenGL/GL.h"
#include "OpenGL/GLU.h"
#endif

namespace MSRender
{
	Colour m_clearcolour;
	unsigned int m_width;
	unsigned int m_height;
};

void MSRender::BeginScene()
{
	// Clear screen
	const unsigned char ucRed = ( (m_clearcolour & 0xff000000) >> 24 );
	const unsigned char ucGreen = ( (m_clearcolour & 0x00ff0000) >> 16 );
	const unsigned char ucBlue = ( (m_clearcolour & 0x0000ff00) >> 8 );
	const unsigned char ucAlpha = ( m_clearcolour & 0x000000ff );
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

void MSRender::EndScene()
{
	MSLauncher::SwapBuffers();
}

void MSRender::Sync()
{

}

void MSRender::ClearColour( Colour rgba )
{
	m_clearcolour = rgba;
}

void MSRender::SetTexture( class MSImage* pImage )
{

}

void MSRender::Quad( MSVec verts[4], u_int layer, MSVec uvs[2] )
{

}

void MSRender::CreateRT()
{
	MSLauncher::Launch( &Tick );
}

void MSRender::Tick()
{
	// TESTING
	ClearColour( 0xff00ffff );
	BeginScene();
	EndScene();

	// ProcessCommands
	MSLauncher::RequestRedisplay();
}

