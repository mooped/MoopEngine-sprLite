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

namespace MSRender
{
	typedef unsigned int Colour;

	void CreateRT();	// This does not return! Stupid GLUT

	void BeginScene();
	void EndScene();
	void Sync();

	void Clear( Colour rgb ); 
};

#endif

