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
	// Command implementations
	void BeginScene();
	void EndScene();
	void Sync();

	void ClearColour( Colour rgba );

	void SetTexture( class MSImage* pImage );
	void Quad( MSVec verts[4], u_int layer, MSVec uvs[2] );

	// Management
	void CreateRT();	// This does not return! Stupid GLUT
	void ProcessCommands();
};

#endif

