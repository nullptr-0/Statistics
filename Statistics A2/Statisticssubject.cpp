// Statisticssubject.cpp: implementation of the Statisticssubject class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "SHARED.H"
#include "Statisticssubject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Statisticssubject::Statisticssubject()
{
	type = 0;
	existst = 0;
	size = 0;
	maxsize = 0;
	name = new char[60];
	firstobjectscr = 0.f;
	lastobjectscr = 0.f;
	fullscr = 0.f;
	exellentscr = 0.f;
	passscr = 0.f;
	badscr = 0.f;
	fullpct = 0.f;
	exellentpct = 0.f;
	passpct = 0.f;
	badpct = 0.f;
	averagescr = 0.f;
	rmsscr = 0.f;
	allobjectscr = 0.f;
}

Statisticssubject::~Statisticssubject()
{
	delete[]name;
}

void Statisticssubject::setscrline(float full, float exellent, float pass, float bad)
{
	fullscr = full;
	exellentscr = exellent;
	passscr = pass;
	badscr = bad;
}

void Statisticssubject::setmaxsize(long max)
{
	if (max == 0)
	{
		maxsize = OBJECTSIZE;
	}
}
