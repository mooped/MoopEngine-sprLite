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

void MSRender::BeginScene()
{
	MSCmdBuf::AddCommand( eCmd_BeginScene );
	MSCmdBuf::Dispatch();
}

void MSRender::EndScene()
{
	MSCmdBuf::AddCommand( eCmd_EndScene );
	MSCmdBuf::Dispatch();
}

void MSRender::Sync()
{
	MSCmdBuf::AddCommand( eCmd_Sync );
	MSCmdBuf::Dispatch();
}

void MSRender::ClearColour( Colour rgba )
{
	MSCmdBuf::AddCommand( eCmd_ClearColour );
	MSCmdBuf::AddUInt( rgba );
	MSCmdBuf::Dispatch();
}

void MSRender::SetTexture( class MSImage* pImage )
{
	MSCmdBuf::AddCommand( eCmd_SetTexture );
	MSCmdBuf::AddPointer( pImage );
	MSCmdBuf::Dispatch();
}

void MSRender::Quad( MSVec verts[4], int layer, MSVec uvs[2] )
{
	MSCmdBuf::AddCommand( eCmd_Quad );
	MSCmdBuf::AddVec( verts[0] );
	MSCmdBuf::AddVec( verts[1] );
	MSCmdBuf::AddVec( verts[2] );
	MSCmdBuf::AddVec( verts[3] );
	MSCmdBuf::AddInt( layer );
	MSCmdBuf::AddVec( uvs[0] );
	MSCmdBuf::AddVec( uvs[1] );
}

