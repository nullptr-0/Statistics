// StatisticsSubject.cpp: implementation of the StatisticsSubject class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "SHARED.H"
#include "StatisticsSubject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StatisticsSubject::StatisticsSubject()
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

StatisticsSubject::~StatisticsSubject()
{
	delete[]name;
}

void StatisticsSubject::setscrline(float full, float exellent, float pass, float bad)
{
	fullscr = full;
	exellentscr = exellent;
	passscr = pass;
	badscr = bad;
}

void StatisticsSubject::setmaxsize(long max)
{
	if (max == 0)
	{
		maxsize = ObjectSize;
	}
}
