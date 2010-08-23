/*
 *  MSEngine.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MSENGINE_DEF
#define MSENGINE_DEF

#include <assert.h>

// Useful types
typedef unsigned char byte;
typedef unsigned int u_int;
typedef unsigned short u_short;
typedef unsigned char u_char;

typedef unsigned int Colour;

// Hashing
#define MHash djb2
typedef unsigned int MHashID;

MHashID MHash( const char* pszString );

// Magic numbers
enum ESpecialKeys
{
	eSk_Esc = 27,
	eSk_Up = 128,
	eSk_Down = 129,
	eSk_Left = 130,
	eSk_Right = 131,
};

// Useful macros

#ifndef NULL
// Eww
#define NULL 0
#endif

#ifdef WIN32
// Eeeeurgh
#define snprintf _snprintf_s
#endif


// Align (align parameter must be a power of 2)
#define ALIGNP2( offset, align ) \
	((offset - 1) & ~(align - 1))

// Calculate padding to align (align parameter must be a power of 2)
#define PADDINGP2( offset, align ) \
	(align + ((offset - 1) & ~(align - 1))) - offset

// Settings
#ifdef WIN32
#define USE_WIN32THREADS 1
#elif MACOSX
#define USE_PTHREADS 1
#endif

#define MASSERT( cond, msg ) assert( cond && msg );

#endif

