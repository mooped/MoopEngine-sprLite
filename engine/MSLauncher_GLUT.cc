/*
 *  MSLauncher_GLUT.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#include "MSLauncher.h"

// System libraries
#if defined(WIN32)
#define _WCHAR_T_DEFINED
#include "glut.h"
#include <stdlib.h>
#elif defined(FREEBSD) || defined(LINUX)
#include <GL/glut.h>
#elif defined(MACOSX)
#include <GLUT/glut.h>
#endif

#include <stdio.h>

namespace MSLauncher
{
	const char* s_pBasePath;

	Callback* s_pfnDisplayCallback = NULL;
	Callback* s_pfnExitCallback = NULL;

	void ReshapeCallback( int width, int height );
	void DisplayCallback();
	void ExitCallback();

	KeyCallback* s_pfnKeyUp = NULL;
	KeyCallback* s_pfnKeyDown = NULL;

	void KeyDownCallback( unsigned char key, int x, int y );
	void KeyUpCallback( unsigned char key, int x, int y );
	void KeySpecialCallback( int key, int x, int y );
};

using namespace MSLauncher;

void MSLauncher::RequestRedisplay()
{
	glutPostRedisplay();
}

void MSLauncher::SwapBuffers()
{
	glutSwapBuffers();
}

void MSLauncher::SetDisplayCallback( Callback* pfnCallback )
{
	s_pfnDisplayCallback = pfnCallback;
}

void MSLauncher::SetExitCallback( Callback* pfnCallback )
{
	s_pfnExitCallback = pfnCallback;
}

void MSLauncher::SetKeyUpCallback( KeyCallback* pfnCallback )
{
	s_pfnKeyUp = pfnCallback;
}

void MSLauncher::SetKeyDownCallback( KeyCallback* pfnCallback )
{
	s_pfnKeyDown = pfnCallback;
}

void MSLauncher::ReshapeCallback( int width, int height )
{

}

void MSLauncher::DisplayCallback()
{
	if ( s_pfnDisplayCallback )
	{
		s_pfnDisplayCallback();
	}
}

void MSLauncher::ExitCallback()
{
	if ( s_pfnExitCallback )
	{
		s_pfnExitCallback();
	}
}

void MSLauncher::KeyDownCallback( unsigned char key, int x, int y )
{
	if ( s_pfnKeyDown )
	{
		s_pfnKeyDown( key );
	}
}

void MSLauncher::KeyUpCallback( unsigned char key, int x, int y )
{
	if ( s_pfnKeyUp )
	{
		s_pfnKeyUp( key );
	}
}

void MSLauncher::KeySpecialCallback( int key, int x, int y )
{
	if ( s_pfnKeyDown )
	{
		switch ( key )
		{
			case GLUT_KEY_UP:
			{
				s_pfnKeyDown( eSk_Up );
			} break;
			case GLUT_KEY_DOWN:
			{
				s_pfnKeyDown( eSk_Down );
			} break;
			case GLUT_KEY_LEFT:
			{
				s_pfnKeyDown( eSk_Left );
			} break;
			case GLUT_KEY_RIGHT:
			{
				s_pfnKeyDown( eSk_Right );
			} break;
			default: break;
		}
	}
}

void MSLauncher::Launch( MSLauncher::TickFn* pfnTick )
{
	glutIdleFunc( pfnTick );
	glutMainLoop();
}

void MSLauncher::SetTitle( const char* const title )
{
	glutSetWindowTitle( title );
}

int main( int argc, char** const argv )
{
	printf( "Path: %s\n", argv[0] );
	s_pBasePath = argv[0];

	// Initialise GLUT...

	// Make us a window
	glutInit( &argc, argv );
	glutInitWindowSize( 800, 600 );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutCreateWindow( "moopengine2 window" );

	// Rendering
	glutReshapeFunc( &MSLauncher::ReshapeCallback );
	glutDisplayFunc( &MSLauncher::DisplayCallback );

	// Input
	glutKeyboardFunc( &KeyDownCallback );
	glutKeyboardUpFunc( &KeyUpCallback );
	glutSpecialFunc( &KeySpecialCallback );

	// Now kick off the game threads
	atexit( &MSLauncher::ExitCallback );

	// Initialise the game
	MSMain::Main();

	// We'll never reach this line
	return 0;
}

