//------------------------------------------------------------------------------
// Desc:	Random number routines
//
// Tabs:	3
//
//		Copyright (c) 1995-1998, 2000, 2003,2005-2006 Novell, Inc. All Rights Reserved.
//
//		This program is free software; you can redistribute it and/or
//		modify it under the terms of version 2 of the GNU General Public
//		License as published by the Free Software Foundation.
//
//		This program is distributed in the hope that it will be useful,
//		but WITHOUT ANY WARRANTY; without even the implied warranty of
//		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//		GNU General Public License for more details.
//
//		You should have received a copy of the GNU General Public License
//		along with this program; if not, contact Novell, Inc.
//
//		To contact Novell about this file by physical or electronic mail,
//		you may find current contact information at www.novell.com
//
// $Id: ftkrand.cpp 3115 2006-01-19 13:24:39 -0700 (Thu, 19 Jan 2006) dsanders $
//------------------------------------------------------------------------------

#include "ftksys.h"

/****************************************************************************
This random number generator is based on Park & Miller's "suggested
minimal standard" for random number generation, pp 1192-1201 of the Oct 1988
issue of _Communications_of_the_ACM_ (vol 31 number 10).  It is a Lehmer
generator, which are of the form x[n] = A * x[n-1] % M, with A and M being
judiciously chosen constants.  More formally, this is a "prime modulus
multiplicative linear congruential generator," or PMMLCG.

Park & Miller originally suggested A==16807 and M==2**31-1, but an update
in CACM Vol 36, No. 7 pp 108-110 (July 1993) indicates that they have found
a better multiplier (A == 48271) to use with the same modulus (2**31-1).
This implementation uses the updated multiplier.

To quote Park & Miller 1988, "We believe that this is the generator that
should always be used--unless one has access to a random number generator
KNOWN to be better."

This algorithm produces a full-period generator; that is, starting from
any seed between 1 and 2**31-2, it generates all other values between 1
and 2**31-2 before it returns to the starting point -- whereupon it repeats
the same sequence of 31-bit values.  This is true for either choice of A
(16807 or 48271).

The July 1993 article includes criticism by George Marsaglia of the Park
and Miller generator.  Marsaglia feels that longer periods are needed.  For
a description of his "subtract-with-borrow" (SWB) generators, see "A New
Class of Random Number Generators", The Annals of Applied Probability,
(1991) Vol. 1, No. 3, pp. 462-480.  These generators require more state
information (~48 longwords) but produce generators with periods on the
order of 10**445. They also pass more stringent tests than the congruential
generators, and so might be considered 'a random number generator KNOWN to
be better.' However, Marsaglia does not spell out all the details needed to
implement SWB, nor does he give any simple test to determine whether an SWB
implementation is correct.
****************************************************************************/

#define MAX_RANDOM  2147483646L

/****************************************************************************
Desc:
****************************************************************************/
class F_RandomGenerator : public IF_RandomGenerator
{
public:

	void FLMAPI randomize( void);

	void FLMAPI setSeed(
		FLMINT32		i32seed);

	FLMINT32 FLMAPI getINT32( void);
		
	FLMINT32 FLMAPI getINT32(
		FLMINT32 	i32Low,
		FLMINT32 	i32High);

	FLMBOOL FLMAPI getBoolean( void);

	FLMINT32 FLMAPI getSeed( void)
	{
		return( m_i32Seed);
	}

private:

	FLMINT32			m_i32Seed;
};

/****************************************************************************
Desc:
****************************************************************************/
RCODE FLMAPI FlmAllocRandomGenerator(
	IF_RandomGenerator **	ppRandomGenerator)
{
	if( (*ppRandomGenerator = f_new F_RandomGenerator) == NULL)
	{
		return( RC_SET( NE_FLM_MEM));
	}
	
	return( NE_FLM_OK);
}

/*************************************************************************
Desc:	Set the seed from the date and time
*************************************************************************/
void	F_RandomGenerator::randomize( void)
{
	FLMUINT	uiTime;

	f_timeGetSeconds( &uiTime );
	setSeed( (FLMUINT32)(((FLMUINT32)uiTime % MAX_RANDOM) + 1));
}

/*************************************************************************
Desc:	initialize the seed to a known value
*************************************************************************/
void F_RandomGenerator::setSeed(
	FLMINT32				i32Seed)
{
	if( i32Seed > 0 && i32Seed <= MAX_RANDOM)
	{
		m_i32Seed = i32Seed;
	}
	else
	{
		setSeed( (FLMUINT32) 
			(i32Seed < 1
				? i32Seed + MAX_RANDOM
				: i32Seed - MAX_RANDOM));
	}
}

/*************************************************************************
Desc:	Generate the next number in the pseudo-random sequence
		i.e.,	"f_randomLong( &r) > MAX_RANDOM/2" will be true half the
		time,	on average.  Likewise, "f_randomLong( &r) & 0x1" has a 50-50
		chance of being true.
*************************************************************************/
FLMINT32 F_RandomGenerator::getINT32( void)
{
#define M		2147483647
#define A		48271
#define CHECK	399268537

	FLMUINT32 	ui32High;
	FLMUINT32 	ui32Low;
	FLMUINT32 	ui32Seed = m_i32Seed;

	ui32High = (ui32Seed >> 16);
	ui32Low = ui32Seed & 0xFFFF;
	ui32Low *= A;
	ui32High *= A;

	ui32High += (ui32Low >> 16) & 0xFFFF;
	ui32Low &= 0xFFFF;

	ui32Low |= (ui32High & 0x7FFF) << 16;
	ui32High >>= 15;
	ui32Low += ui32High;

	if( ui32Low & 0x80000000L)
	{
		ui32Low &= 0x7FFFFFFF;
		ui32Low++;
	}

	return( m_i32Seed = ui32Low);
}

/*************************************************************************
Desc:	Returns a random integer between i32Low and i32High, inclusive.
*************************************************************************/
FLMINT32 F_RandomGenerator::getINT32(
	FLMINT32				i32Low,
	FLMINT32				i32High)
{
	FLMINT32 	i32Range = i32High - i32Low + 1;

	if( i32Range < (1L << 20))
	{
		return( i32Low + getINT32() % i32Range);
	}
	else
	{
		FLMINT32 	i32Mask = 0;
		FLMINT32 	i32Val;

		i32Range--;
		for( i32Val = i32Range; i32Val > 0; i32Val >>= 1)
		{
			i32Mask = (i32Mask << 1) | 1;
		}

		do
		{
			i32Val = getINT32() & i32Mask;
		}	while( i32Val > i32Range);

		return( i32Low + i32Val);
	}
}

/*************************************************************************
Desc:
*************************************************************************/
FLMBOOL F_RandomGenerator::getBoolean( void)
{
	return( (getINT32( 1, 100) <= 50 ? TRUE : FALSE));
}