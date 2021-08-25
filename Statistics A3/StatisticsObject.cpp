// StatisticsObject.cpp: implementation of the StatisticsObject class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "Constants.h"
#include "StatisticsGroup.h"
#include "StatisticsSubject.h"
#include "StatisticsObject.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StatisticsObject::StatisticsObject()
{
	groups = new StatisticsGroup * [GroupSize];
	subjects = new StatisticsSubject * [SubjectSize];
	groupsexistst = new short[GroupSize];
	groupranks = new long[GroupSize];
	subjectsexistst = new short[SubjectSize];
	subjectscores = new float[SubjectSize];
	subjectranks = new long[SubjectSize];
	name = new char[30];
	number = new char[60];
	sex = 0;
	existst = 0;
	marked = 0;
	rank = 0;
	score = 0;
	long tpos;
	for (tpos = 0; tpos < GroupSize; tpos++)
	{
		groups[tpos] = NULL;
		groupsexistst[tpos] = 0;
		groupranks[tpos] = 0;
	}
	for (tpos = 0; tpos < SubjectSize; tpos++)
	{
		subjects[tpos] = NULL;
		subjectsexistst[tpos] = 0;
		subjectscores[tpos] = -1.f;
		subjectranks[tpos] = 0;
	}
}

StatisticsObject::~StatisticsObject()
{
	delete[]groups;
	delete[]subjects;
	delete[]groupsexistst;
	delete[]subjectsexistst;
	delete[]subjectscores;
	delete[]name;
	delete[]number;
}

bool StatisticsObject::IsInGroup(void* item, short type)
{
	long tpos;
	bool state = false;
	switch (type)
	{
	case ByName:
		for (tpos = 0; tpos < GroupSize && ((groupsexistst[tpos] != 1) || strcmp(groups[tpos]->name, (const char*)item) != 0); tpos++)
		{
		}
		if (tpos < GroupSize)
			state = true;
		break;
	case ByObj:
		for (tpos = 0; tpos < GroupSize && ((groupsexistst[tpos] != 1) || (groups[tpos] != (StatisticsGroup*)item)); tpos++)
		{
		}
		if (tpos < GroupSize)
			state = true;
		break;
	default:
		break;
	}
	return state;
}

bool StatisticsObject::IsInSubject(void* item, short type)
{
	long tpos;
	bool state = false;
	switch (type)
	{
	case ByName:
		for (tpos = 0; tpos < SubjectSize && ((subjectsexistst[tpos] != 1) || strcmp(subjects[tpos]->name, (const char*)item) != 0); tpos++)
		{
		}
		if (tpos < SubjectSize)
			state = true;
		break;
	case ByObj:
		for (tpos = 0; tpos < SubjectSize && ((subjectsexistst[tpos] != 1) || (subjects[tpos] != (StatisticsSubject*)item)); tpos++)
		{
		}
		if (tpos < SubjectSize)
			state = true;
		break;
	default:
		break;
	}
	return state;
}

bool StatisticsObject::IsInSubject(void* item, short type, long& pos)
{
	pos = 0;
	long tpos;
	bool state = false;
	switch (type)
	{
	case ByName:
		for (tpos = 0; tpos < SubjectSize && ((subjectsexistst[tpos] != 1) || strcmp(subjects[tpos]->name, (const char*)item) != 0); tpos++)
		{
		}
		if (tpos < SubjectSize)
		{
			state = true;
			pos = tpos;
		}
		break;
	case ByObj:
		for (tpos = 0; tpos < SubjectSize && ((subjectsexistst[tpos] != 1) || (subjects[tpos] != (StatisticsSubject*)item)); tpos++)
		{
		}
		if (tpos < SubjectSize)
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
