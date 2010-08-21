/*
 *  MSLauncher_GLUT.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MSLauncher.h"

// System libraries
#if defined(WIN32)
#include <glut.h>
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
	//glutKeyboardFunc( &keyboard_callback );
	//glutKeyboardUpFunc( &keyboard_up_callback );
	//glutSpecialFunc( &special_key_callback );
	//glutMouseFunc( &mouse_callback );
	//glutPassiveMotionFunc( &motion_callback );
	//glutJoystickFunc( &joystick_callback, 16 );	// Polling interval 16ms

	// Now kick off the game threads
	atexit( &MSLauncher::ExitCallback );

	// Initialise the game
	MSMain::Main();

	// We'll never reach this line
	return 0;
}

