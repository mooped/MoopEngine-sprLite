/*
 *  MSRender.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MSRENDER_DEF
#define MSRENDER_DEF

#include "MSEngine.h"
#include "MSPoint.h"
#include "MSCmdBuf.h"
#include "MSEffect.h"

class MSImage;

namespace MSRender
{
	// Render command enums
	enum ECmd
	{
		eCmd_NOP = MSCmdBuf::eCmd_NOP,
		eCmd_SKP = MSCmdBuf::eCmd_SKP,
		eCmd_BeginScene,
		eCmd_EndScene,
		eCmd_Sync,
		eCmd_ClearColour,
		eCmd_SetTexture,
		eCmd_Quad,
		eCmd_DebugLine,

		eCmd_Invalid,
	};

	// Render commands
	void BeginScene();
	void EndScene();
	void Sync();

	void ClearColour( Colour rgba );

	void SetTexture( MSImage* pImage );
	void Quad( MSVec verts[4], int layer, MSVec uvs[2], Colour rgba );

	void DebugLine( MSVec a, MSVec b, Colour rgba );
};

#endif

