/*
 *  MSTile.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MSTILE_DEF
#define MSTILE_DEF

#include "MSSprite.h"

namespace MSTile
{
	typedef u_int LayerID;

	struct SLayer
	{
		const int width;
		const int height;
		const MSSprite::SheetID sheet;
		MSSprite::SpriteID* const data;
	};

	LayerID AddLayer( MSSprite::SheetID sheet, int width, int height );
	LayerID AddLayer( MSSprite::SheetID sheet, const char* const filename );
	void WriteLayer( LayerID id, const char* const filename );
	void Shutdown();

	SLayer& GetLayer( LayerID id );

	void RenderLayer( LayerID layerId, const MSVec& pos, int layer, const MSVec& tileSize, const Colour rgba = 0xffffffff );
};

#endif

