/*
 *  MSLauncher.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MSGLUT_DEF
#define MSGLUT_DEF

#include "MSEngine.h"

namespace MSMain
{
	void Main();
};

namespace MSLauncher
{
	typedef void (TickFn)();

	const char* GetBasePath();

	void RequestRedisplay();
	void SwapBuffers();
	void ReshapeCallback( int width, int height );
	void DisplayCallback();
	void ExitCallback();

	void Launch( TickFn* pfnTick );

	void SetTitle( const char* const title );
};

#endif

