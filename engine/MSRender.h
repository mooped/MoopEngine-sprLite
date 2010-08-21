/*
 *  MSRender.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MSRENDER_DEF
#define MSRENDER_DEF

#include "MSEngine.h"
#include "MSPoint.h"

namespace MSRender
{
	// Render commands
	void BeginScene();
	void EndScene();
	void Sync();

	void ClearColour( Colour rgba );

	void SetTexture( class MSImage* pImage );
	void Quad( MSVec verts[4], u_int layer, MSVec uvs[2] );
};

#endif

