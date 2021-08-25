// Statisticsgroup.cpp: implementation of the Statisticsgroup class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "Statisticsgroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Statisticsgroup::Statisticsgroup()
{
	name=new char[60];
	type=0;
	existst=0;
	size=0;
	firstobjectscr = 0.f;
	lastobjectscr = 0.f;
	allobjectscr = 0.f;
	averagescr = 0.f;
	rmsscr = 0.f;
	exellentpct = 0.f;
	passpct = 0.f;
	badpct = 0.f;
}

Statisticsgroup::~Statisticsgroup()
{
	delete []name;
}

