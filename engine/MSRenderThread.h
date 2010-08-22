/*
 *  MSRenderThread.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MRENDERTHREAD_DEF
#define MRENDERTHREAD_DEF

#include "MSEngine.h"
#include "MSPoint.h"

namespace MSRenderThread
{
	typedef void (UpdateFunc)();

	// Management
	void CreateRT();	// This does not return! Stupid GLUT
	void ProcessCommands();

	// For single threaded games
	void SetGameUpdateFunc( UpdateFunc* pfnUpdate );
};

#endif

