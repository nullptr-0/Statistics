// Statisticsinfo.cpp: implementation of the Statisticsinfo class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "Shared.h"
#include "Statisticsgroup.h"
#include "Statisticssubject.h"
#include "Statisticsinfo.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Statisticsinfo::Statisticsinfo()
{
	groups = new Statisticsgroup * [GROUPSIZE];
	subjects = new Statisticssubject * [SUBJECTSIZE];
	groupsexistst = new short[GROUPSIZE];
	groupranks = new long[GROUPSIZE];
	subjectsexistst = new short[SUBJECTSIZE];
	subjectscores = new float[SUBJECTSIZE];
	subjectranks = new long[SUBJECTSIZE];
	name = new char[30];
	number = new char[60];
	sex = 0;
	existst = 0;
	marked = 0;
	rank = 0;
	score = 0;
	long tpos;
	for (tpos = 0; tpos < GROUPSIZE; tpos++)
	{
		groups[tpos] = NULL;
		groupsexistst[tpos] = 0;
		groupranks[tpos] = 0;
	}
	for (tpos = 0; tpos < SUBJECTSIZE; tpos++)
	{
		subjects[tpos] = NULL;
		subjectsexistst[tpos] = 0;
		subjectscores[tpos] = -1.f;
		subjectranks[tpos] = 0;
	}
}

Statisticsinfo::~Statisticsinfo()
{
	delete[]groups;
	delete[]subjects;
	delete[]groupsexistst;
	delete[]subjectsexistst;
	delete[]subjectscores;
	delete[]name;
	delete[]number;
}

bool Statisticsinfo::IsInGroup(void* item, short type)
{
	long tpos;
	bool state = false;
	switch (type)
	{
	case BYNAME:
		for (tpos = 0; tpos < GROUPSIZE && ((groupsexistst[tpos] != 1) || strcmp(groups[tpos]->name, (const char*)item) != 0); tpos++)
		{
		}
		if (tpos < GROUPSIZE)
			state = true;
		break;
	case BYOBJ:
		for (tpos = 0; tpos < GROUPSIZE && ((groupsexistst[tpos] != 1) || (groups[tpos] != (Statisticsgroup*)item)); tpos++)
		{
		}
		if (tpos < GROUPSIZE)
			state = true;
		break;
	default:
		break;
	}
	return state;
}

bool Statisticsinfo::IsInSubject(void* item, short type)
{
	long tpos;
	bool state = false;
	switch (type)
	{
	case BYNAME:
		for (tpos = 0; tpos < SUBJECTSIZE && ((subjectsexistst[tpos] != 1) || strcmp(subjects[tpos]->name, (const char*)item) != 0); tpos++)
		{
		}
		if (tpos < SUBJECTSIZE)
			state = true;
		break;
	case BYOBJ:
		for (tpos = 0; tpos < SUBJECTSIZE && ((subjectsexistst[tpos] != 1) || (subjects[tpos] != (Statisticssubject*)item)); tpos++)
		{
		}
		if (tpos < SUBJECTSIZE)
			state = true;
		break;
	default:
		break;
	}
	return state;
}

bool Statisticsinfo::IsInSubject(void* item, short type, long& pos)
{
	pos = 0;
	long tpos;
	bool state = false;
	switch (type)
	{
	case BYNAME:
		for (tpos = 0; tpos < SUBJECTSIZE && ((subjectsexistst[tpos] != 1) || strcmp(subjects[tpos]->name, (const char*)item) != 0); tpos++)
		{
		}
		if (tpos < SUBJECTSIZE)
		{
			state = true;
			pos = tpos;
		}
		break;
	case BYOBJ:
		for (tpos = 0; tpos < SUBJECTSIZE && ((subjectsexistst[tpos] != 1) || (subjects[tpos] != (Statisticssubject*)item)); tpos++)
		{
		}
		if (tpos < SUBJECTSIZE)
		{
			state = true;
			pos = tpos;
		}
		break;
	default:
		break;
	}
	return state;
}
