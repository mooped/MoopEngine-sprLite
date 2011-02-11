/*
 *  MSTile.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#include "MSTile.h"

#include <vector>
#include <stdio.h>

namespace MSTile
{
	enum EFileVersion
	{
		eFV_INITIAL = 0,

		eFV_CURRENT_VERSION = eFV_INITIAL,
	};

	struct SLayer_Int
	{
		int width;
		int height;
		MSSprite::SheetID sheet;
		MSSprite::SpriteID* data;
	};

	typedef std::vector<SLayer_Int*> LayerList;
	LayerList s_layers;
};

MSTile::LayerID MSTile::AddLayer( MSSprite::SheetID sheet, int width, int height )
{
	SLayer_Int* layer = new SLayer_Int;
	layer->width = width;
	layer->height = height;
	layer->sheet = sheet;
	layer->data = new MSSprite::SpriteID[width * height];
	const int id = s_layers.size();
	s_layers.push_back( layer );
	return id;
}

MSTile::LayerID MSTile::AddLayer( MSSprite::SheetID sheet, const char* const filename )
{
	LayerID layerId = ~0;
	FILE* f = fopen( filename, "rb" );
	if ( f )
	{
		u_int version;
		int read;
		read = fread( &version, sizeof( version ), 1, f );
		MASSERT( read == 1, "Failed to read MSTile data." );
		version = swap_endian32( version );
		switch ( version )
		{
			case eFV_INITIAL:
			{
				int width;
				int height;
				read = fread( &width, sizeof( width ), 1, f );
				MASSERT( read == 1, "Failed to read MSTile data." );
				read = fread( &height, sizeof( height ), 1, f );
				MASSERT( read == 1, "Failed to read MSTile data." );
#ifdef BIGENDIAN
				width = swap_endian32( width );
				height = swap_endian32( height );
#endif
				layerId = AddLayer( sheet, width, height );
				SLayer& layer = GetLayer( layerId );
				unsigned int count = width * height;
				read = fread( layer.data, sizeof( MSSprite::SpriteID ), count, f );
				MASSERT( read == count, "Failed to read MSTileData." );
#ifdef BIGENDIAN
				for ( unsigned int u = 0; u < count; ++u )
				{
					layer.data[u] = swap_endian32( layer.data[u] );
				}
#endif
			} break;
			default:
			{
				MASSERT( 0, "Unsupported MSTile version." );
			} break; 
		}
	}
	return layerId;
}

void MSTile::WriteLayer( MSTile::LayerID id, const char* const filename )
{
	FILE* f = fopen( filename, "wb" );
	int written;
#ifdef BIGENDIAN
	SLayer& original = GetLayer( id );
	SLayer_Int layer;
	unsigned count = original.width * original.height;
	layer.width = swap_endian32( original.width );
	layer.height = swap_endian32( original.height );
	MSSprite::SpriteID data[count];
	for ( unsigned int u = 0; u < count; ++u )
	{
		data[u] = swap_endian32( original.data[u] );
	}
	layer.data = &data[0];
#else
	SLayer& layer = GetLayer( id );
#endif
	const u_int version = swap_endian32( eFV_CURRENT_VERSION );
	written = fwrite( &version, sizeof( version ), 1, f );
	MASSERT( written == 1, "Failed to write MSTile data." );
	written = fwrite( &layer.width, sizeof( layer.width ), 1, f );
	MASSERT( written == 1, "Failed to write MSTile data." );
	written = fwrite( &layer.height, sizeof( layer.height ), 1, f );
	MASSERT( written == 1, "Failed to write MSTile data." );
	written = fwrite( layer.data, sizeof( MSSprite::SpriteID ), layer.width * layer.height, f );
	MASSERT( written == ( layer.width * layer.height ), "Failed to write MSTileData." );
}

void MSTile::Shutdown()
{
	for ( LayerList::iterator it = s_layers.begin(), end = s_layers.end(); it != end; ++it )
	{
		if ( *it )
		{
			delete[] (*it)->data;
			memset( (*it), sizeof( SLayer ), 0 );
			delete *it;
			*it = NULL;
		}
	}
	s_layers.clear();
}

MSTile::SLayer& MSTile::GetLayer( MSTile::LayerID id )
{
	MASSERT( id < s_layers.size(), "Invalid layer ID." );
	MASSERT( sizeof( SLayer ) == sizeof( SLayer_Int ), "SLayer and SLayer_Int structures do not match!" );
	return *reinterpret_cast<SLayer*>( s_layers[id] );
}

void MSTile::RenderLayer( MSTile::LayerID layerId, const MSVec& pos, int layer, const MSVec& tileSize, const Colour rgba/* = 0xffffffff*/ )
{
	SLayer& tileLayer = GetLayer( layerId );
	// Sprite positions are calculated from the centre so subtract 1
	const MSVec layerSize( tileLayer.width - 1, tileLayer.height - 1 );
	MSVec tlPos = pos - ( ( tileSize * layerSize ) / MSVec( 2, 2 ) );
	const int width = tileLayer.width;
	const int height = tileLayer.height;
	const MSSprite::SheetID sheet = tileLayer.sheet;
	const MSSprite::SpriteID* const data = tileLayer.data;
	for ( int y = 0; y < height; ++y )
	{
		const int rowIdx = width * y;
		for ( int x = 0; x < width; ++x )
		{
			const MSSprite::SpriteID sprite = data[rowIdx + x];
			const MSVec sprpos = tlPos + ( tileSize * MSVec( x, y ) );
			MSSprite::RenderSprite( sheet, sprite, sprpos, layer, tileSize, rgba );
		}
	}
}

