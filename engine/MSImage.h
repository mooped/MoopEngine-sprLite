/*
 *  MSImage.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MSIMAGE_DEF
#define MSIMAGE_DEF

#include "MSEngine.h"

class MSImage
{
private:
	const MHashID m_hash;
	u_int m_ref;
	int m_width;
	int m_height;
	u_char* m_data;

	void LoadTGA( const char* const filename );

public:
	MSImage( const char* const filename );
	MSImage( const MHashID hash, const int width, const int height );
	~MSImage();

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	u_char* GetData() { return m_data; }

	u_int GetRef() { return m_ref; }
	void SetRef( u_int ref ) { m_ref = ref; }
};

#endif

