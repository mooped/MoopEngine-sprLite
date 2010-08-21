/*
 *  MSImage.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
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
	const char* m_data;

	void LoadTGA( const char* const filename );

public:
	MSImage( const char* const filename );
	~MSImage();

	u_int GetRef() { return m_ref; }
};

#endif

