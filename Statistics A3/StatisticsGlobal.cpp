// StatisticsGlobal.cpp: implementation of the StatisticsGlobal class.
//
//////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "math.h"
#include "Constants.h"
#include "StatisticsGroup.h"
#include "StatisticsSubject.h"
#include "StatisticsObject.h"
#include "StatisticsGlobal.h"
#include <string.h>

#pragma warning(disable : 4996)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StatisticsGlobal::StatisticsGlobal()
{
	objects = new StatisticsObject[ObjectSize];
	groups = new StatisticsGroup[GroupSize];
	subjects = new StatisticsSubject[SubjectSize];
	for (int i = 0; i < ObjectSize; i++)
	{
		marked[i] = -1;
	}
	for (int i = 0; i < MSGSIZE; i++)
	{
		msg_str[i] = new char[100];
		msg_stc[i].objsrc = NULL;
		msg_stc[i].grpsrc = NULL;
		msg_stc[i].subsrc = NULL;
		msg_stc[i].msgflag = 0;
		strcpy(msg_str[i], "");
	}
	size = 0;
	objectIndex = 0;
	groupIndex = 0;
	subjectIndex = 0;
	averagescr = 0.0;
	allobjectscr = 0.0;
	firstobjectscr = 0.0;
	lastobjectscr = 0.0;
	rmsscr = 0.0;
	exellentscr = 0.0;
	badscr = 0.0;
	passscr = 0.0;
	exellentpct = 0.0;
	badpct = 0.0;
	passpct = 0.0;
	errcode = 0;
}

StatisticsGlobal::~StatisticsGlobal()
{
	delete[]objects;
	delete[]groups;
	delete[]subjects;
	for (int i = 0; i < MSGSIZE; i++)
	{
		delete[]msg_str[i];
	}
}

void StatisticsGlobal::Setscrline(float esc, float bsc, float psc)
{
	exellentscr = esc;
	badscr = bsc;
	passscr = psc;
}

bool StatisticsGlobal::SearchObject(void* si, short type, short stype = Search_Single)
{
	long tpos = 0;
	switch (type)
	{
	case 1:
		if (stype != Search_Fuzzy)
		{
			for (; (strcmp(objects[tpos].name, (const char*)si) != 0 || objects[tpos].existst != 1) && tpos < ObjectSize; tpos++) {}
		}
		else
		{
			for (; tpos < ObjectSize; tpos++)
			{
				if (strstr(objects[tpos].name, (const char*)si) != 0 && objects[tpos].existst == 1)
				{
					objects[tpos].marked = 1;
				}
			}
			return true;
		}
		break;
	case 2:
		if (stype != Search_Fuzzy)
		{
			for (; (strcmp(objects[tpos].number, (const char*)si) != 0 || objects[tpos].existst != 1) && tpos < ObjectSize; tpos++) {}
		}
		else
		{
			for (; tpos < ObjectSize; tpos++)
			{
				if (strstr(objects[tpos].number, (const char*)si) != 0 && objects[tpos].existst == 1)
				{
					objects[tpos].marked = 1;
				}
			}
			return true;
		}
		break;
	case 3:
		if (stype == Search_Single)
		{
			long tpos2 = 0;
			SUB_SCR* tsi;
			tsi = (SUB_SCR*)si;
			if (tsi->sub == NULL)
			{
				for (; (objects[tpos].score != tsi->scr || objects[tpos].existst != 1) && tpos < ObjectSize; tpos++) {}
			}
			else
			{
				for (; tpos < ObjectSize; tpos++)
				{
					if (objects[tpos].existst == 1)
					{
						for (tpos2 = 0; (objects[tpos].subjects[tpos2] != tsi->sub || objects[tpos].subjectsexistst[tpos2] != 1 || objects[tpos].subjectscores[tpos2] != tsi->scr) && tpos2 < SubjectSize; tpos2++) {}
						if (tpos2 < SubjectSize)
						{
							objectIndex = tpos;
							return true;
						}
					}
				}
			}
		}
		else
		{
			long tpos2 = 0;
			SUB_SCR* tsi;
			tsi = (SUB_SCR*)si;
			if (tsi->sub == NULL)
			{
				for (; tpos < ObjectSize; tpos++)
				{
					if (objects[tpos].score == tsi->scr && objects[tpos].existst == 1)
					{
						objects[tpos].marked = 1;
					}
				}
			}
			else
			{
				for (; tpos < ObjectSize; tpos++)
				{
					if (objects[tpos].existst == 1 && objects[tpos].IsInSubject(tsi->sub, ByObj, tpos2) == true && objects[tpos].subjectscores[tpos2] == tsi->scr)
					{
						objects[tpos].marked = 1;
					}
				}
			}
			return true;
		}
		break;
	case 4:
		if (stype == Search_Single)
		{
			for (; (objects[tpos].rank != *((long*)si) || objects[tpos].existst != 1) && tpos < ObjectSize; tpos++) {}
		}
		else
		{
			for (; tpos < ObjectSize; tpos++)
			{
				if (objects[tpos].existst == 1 && objects[tpos].rank == *((long*)si))
				{
					objects[tpos].marked = 1;
				}
			}
			return true;
		}
		break;
	}
	if (tpos == ObjectSize)
	{
		errcode = Error_NoSuchItem;
		return false;
	}
	objectIndex = tpos;
	return true;
}

void StatisticsGlobal::GetAll(long& sz, float& avg, float& gap, float& rms)
{
	sz = size;
	avg = averagescr;
	gap = firstobjectscr - lastobjectscr;
	rms = rmsscr;
}

bool StatisticsGlobal::AddObject(char* onm, char* onb, short osx, StatisticsObject** reto)
{
	if (size == ObjectSize)
	{
		errcode = Error_FullMemory;;
		return false;
	}
	long tpos;
	long tpos2 = -1;
	bool retv = true;
	for (tpos = 0; tpos < ObjectSize; tpos++)
	{
		if (objects[tpos].existst != 1 && tpos2 < 0)
		{
			tpos2 = tpos;
		}
		if (objects[tpos].existst == 1)
		{
			if (strcmp(objects[tpos].name, onm) == 0 || strcmp(objects[tpos].number, onb) == 0)
			{
				errcode = Error_ItemExists;;
				return false;
			}
		}
	}
	tpos = tpos2;
	if (reto != NULL)
	{
		*reto = &(objects[tpos]);
	}
	strcpy(objects[tpos].name, onm);
	strcpy(objects[tpos].number, onb);
	objects[tpos].sex = osx;
	objects[tpos].score = 0.f;
	objects[tpos].existst = 1;
	size++;
	averagescr = allobjectscr / size;
	rmsscr = 0;
	for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
	{
		if (objects[tpos2].existst == 1)
		{
			rmsscr += pow((objects[tpos2].score - averagescr), 2);
		}
	}
	rmsscr = pow((rmsscr / size), 0.5);
	lastobjectscr = 0;
	if (size == 1)
	{
		firstobjectscr = 0;
		objectIndex = 0;
		objects[tpos].rank = 1;
	}
	else
	{
		objects[tpos].rank = size;
		for (tpos2 = 0; (objects[tpos2].existst != 1 || lastobjectscr != objects[tpos2].score) && tpos2 < ObjectSize; tpos2++) {}
		if (tpos2 < ObjectSize)
		{
			objects[tpos].rank = objects[tpos2].rank;
		}
	}

	tpos2 = 0;
	long tpos3 = 0;
	long tpos4 = 0;
	long tpos5 = 0;
	for (tpos2 = 0; tpos2 < GroupSize; tpos2++)
	{
		if (groups[tpos2].existst == 1 && groups[tpos2].type == CompulsoryGroups)
		{
			objects[tpos].groups[tpos3] = &groups[tpos2];
			objects[tpos].groupsexistst[tpos3] = 1;
			objects[tpos].groups[tpos3]->size++;
			objects[tpos].groups[tpos3]->averagescr = (objects[tpos].groups[tpos3]->allobjectscr) / (objects[tpos].groups[tpos3]->size);
			if (objects[tpos].groups[tpos3]->size == 1)
			{
				objects[tpos].groups[tpos3]->firstobjectscr = 0;
			}
			objects[tpos].groups[tpos3]->lastobjectscr = 0;
			if (objects[tpos].score == exellentscr)
			{
				objects[tpos].groups[tpos3]->exellentpct = (objects[tpos].groups[tpos3]->exellentpct * (objects[tpos].groups[tpos3]->size - 1) + 100) / (objects[tpos].groups[tpos3]->size);
			}
			if (objects[tpos].score == passscr)
			{
				objects[tpos].groups[tpos3]->passpct = (objects[tpos].groups[tpos3]->passpct * (objects[tpos].groups[tpos3]->size - 1) + 100) / (objects[tpos].groups[tpos3]->size);
			}
			if (objects[tpos].score <= badscr)
			{
				objects[tpos].groups[tpos3]->badpct = (objects[tpos].groups[tpos3]->badpct * (objects[tpos].groups[tpos3]->size - 1) + 100) / (objects[tpos].groups[tpos3]->size);
			}
			objects[tpos].groups[tpos3]->rmsscr = 0;
			for (tpos4 = 0; tpos4 < ObjectSize; tpos4++)
			{
				if (objects[tpos4].existst == 1 && objects[tpos4].IsInGroup(objects[tpos].groups[tpos3], ByObj) == true)
				{
					objects[tpos].groups[tpos3]->rmsscr += pow(((objects[tpos4].score) - (objects[tpos].groups[tpos3]->averagescr)), 2);
				}
			}
			objects[tpos].groups[tpos3]->rmsscr = pow((objects[tpos].groups[tpos3]->rmsscr / objects[tpos].groups[tpos3]->size), 0.5);
			tpos3++;
		}
	}
	tpos3 = 0;
	tpos4 = 0;
	for (tpos2 = 0; tpos2 < SubjectSize; tpos2++)
	{
		if (subjects[tpos2].existst == 1 && subjects[tpos2].type == CompulsorySubjects)
		{
			objects[tpos].subjects[tpos3] = &subjects[tpos2];
			objects[tpos].subjectsexistst[tpos3] = 1;
			objects[tpos].subjectscores[tpos3] = -1.f;
			objects[tpos].subjects[tpos3]->size++;
			objects[tpos].subjects[tpos3]->averagescr = (objects[tpos].subjects[tpos3]->allobjectscr) / (objects[tpos].subjects[tpos3]->size);
			if (objects[tpos].subjects[tpos3]->size == 1)
			{
				objects[tpos].subjects[tpos3]->firstobjectscr = 0;
			}
			objects[tpos].subjects[tpos3]->lastobjectscr = 0;
			if (0 == objects[tpos].subjects[tpos3]->fullscr)
			{
				objects[tpos].subjects[tpos3]->fullpct = (objects[tpos].subjects[tpos3]->fullpct * (objects[tpos].subjects[tpos3]->size - 1) + 100) / (objects[tpos].subjects[tpos3]->size);
			}
			if (0 == objects[tpos].subjects[tpos3]->exellentscr)
			{
				objects[tpos].subjects[tpos3]->exellentpct = (objects[tpos].subjects[tpos3]->exellentpct * (objects[tpos].subjects[tpos3]->size - 1) + 100) / (objects[tpos].subjects[tpos3]->size);
			}
			if (0 == objects[tpos].subjects[tpos3]->passscr)
			{
				objects[tpos].subjects[tpos3]->passpct = (objects[tpos].subjects[tpos3]->passpct * (objects[tpos].subjects[tpos3]->size - 1) + 100) / (objects[tpos].subjects[tpos3]->size);
			}
			if (0 <= objects[tpos].subjects[tpos3]->badscr)
			{
				objects[tpos].subjects[tpos3]->badpct = (objects[tpos].subjects[tpos3]->badpct * (objects[tpos].subjects[tpos3]->size - 1) + 100) / (objects[tpos].subjects[tpos3]->size);
			}
			objects[tpos].subjects[tpos3]->rmsscr = 0;
			for (tpos4 = 0; tpos4 < ObjectSize; tpos4++)
			{
				if (objects[tpos4].existst == 1 && objects[tpos4].IsInSubject(objects[tpos].subjects[tpos3], ByObj, tpos5) == true)
				{
					if (objects[tpos4].subjectscores[tpos5] <= 0)
					{
						objects[tpos].subjects[tpos3]->rmsscr += pow(objects[tpos].subjects[tpos3]->averagescr, 2);
					}
					else
					{
						objects[tpos].subjects[tpos3]->rmsscr += pow(((objects[tpos4].subjectscores[tpos5]) - (objects[tpos].subjects[tpos3]->averagescr)), 2);
					}
				}
			}
			objects[tpos].subjects[tpos3]->rmsscr = pow((objects[tpos].subjects[tpos3]->rmsscr / objects[tpos].subjects[tpos3]->size), 0.5);
			tpos3++;
		}
	}
	return retv;
}

bool StatisticsGlobal::DeleteObject(char*& onm, char*& onb, short& osx)
{
	if (size == 0)
	{
		errcode = Error_EmptyMemory;
		return false;
	}
	long tpos;
	long tpos2;
	long plus;
	tpos = objectIndex;
	strcpy(onm, objects[tpos].name);
	strcpy(onb, objects[tpos].number);
	osx = objects[tpos].sex;
	objects[tpos].existst = 0;
	size--;
	if (size == 0)
	{
		objectIndex = 0;
		allobjectscr = 0;
		averagescr = 0;
		rmsscr = 0;
		firstobjectscr = 0;
		lastobjectscr = 0;
		exellentpct = 0.0;
		badpct = 0.0;
		passpct = 0.0;
		return true;
	}
	allobjectscr -= objects[tpos].score;
	averagescr = allobjectscr / size;
	rmsscr = 0;
	for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
	{
		if (objects[tpos2].existst == 1)
		{
			rmsscr += pow((objects[tpos2].score - averagescr), 2);
		}
	}
	rmsscr = pow((rmsscr / size), 0.5);
	if (objects[tpos].score == firstobjectscr)
	{
		plus = 1;
	retry4:
		for (tpos2 = 0; (objects[tpos2].rank != plus || objects[tpos2].existst != 1) && tpos2 < ObjectSize; tpos2++) {}
		if (tpos2 == ObjectSize)
		{
			plus++;
			goto retry4;
		}
		firstobjectscr = objects[tpos2].score;
	}
	if (objects[tpos].score == lastobjectscr)
	{
		plus = size;
	retry5:
		for (tpos2 = 0; (objects[tpos2].rank != plus || objects[tpos2].existst != 1) && tpos2 < ObjectSize; tpos2++) {}
		if (tpos2 == ObjectSize)
		{
			plus--;
			goto retry5;
		}
		lastobjectscr = objects[tpos2].score;
	}
	for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
	{
		if (objects[tpos].score > objects[tpos2].score && objects[tpos2].existst == 1)
		{
			objects[tpos2].rank--;
		}
	}

	long tpos3;
	long tpos4 = 0;
	long tpos5 = 0;
	for (tpos3 = 0; tpos3 < GroupSize; tpos3++)
	{
		if (objects[tpos].groupsexistst[tpos3] == 1)
		{
			objects[tpos].groups[tpos3]->size--;
			objects[tpos].groups[tpos3]->allobjectscr -= objects[tpos].score;
			objects[tpos].groups[tpos3]->averagescr = (objects[tpos].groups[tpos3]->allobjectscr) / (objects[tpos].groups[tpos3]->size);
			if (objects[tpos].score == objects[tpos].groups[tpos3]->firstobjectscr)
			{
				objects[tpos].groups[tpos3]->firstobjectscr = 0;
				for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
				{
					if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(objects[tpos].groups[tpos3], ByObj) == true && objects[tpos2].score > objects[tpos].groups[tpos3]->firstobjectscr)
					{
						objects[tpos].groups[tpos3]->firstobjectscr = objects[tpos2].score;
					}
				}
			}
			if (objects[tpos].score == objects[tpos].groups[tpos3]->lastobjectscr)
			{
				if (objects[tpos].groups[tpos3]->size == 0)
				{
					objects[tpos].groups[tpos3]->lastobjectscr = 0;
				}
				else
				{
					objects[tpos].groups[tpos3]->lastobjectscr = objects[tpos].groups[tpos3]->firstobjectscr;
					for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
					{
						if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(objects[tpos].groups[tpos3], ByObj) == true && objects[tpos2].score < objects[tpos].groups[tpos3]->lastobjectscr)
						{
							objects[tpos].groups[tpos3]->lastobjectscr = objects[tpos2].score;
						}
					}
				}
			}
			if (objects[tpos].groups[tpos3]->size != 0)
			{
				if (objects[tpos].score >= exellentscr)
				{
					objects[tpos].groups[tpos3]->exellentpct = (objects[tpos].groups[tpos3]->exellentpct * (objects[tpos].groups[tpos3]->size + 1) - 100) / (objects[tpos].groups[tpos3]->size);
				}
				if (objects[tpos].score >= passscr)
				{
					objects[tpos].groups[tpos3]->passpct = (objects[tpos].groups[tpos3]->passpct * (objects[tpos].groups[tpos3]->size + 1) - 100) / (objects[tpos].groups[tpos3]->size);
				}
				if (objects[tpos].score <= badscr)
				{
					objects[tpos].groups[tpos3]->badpct = (objects[tpos].groups[tpos3]->badpct * (objects[tpos].groups[tpos3]->size + 1) - 100) / (objects[tpos].groups[tpos3]->size);
				}
			}
			else
			{
				objects[tpos].groups[tpos3]->exellentpct = 0;
				objects[tpos].groups[tpos3]->passpct = 0;
				objects[tpos].groups[tpos3]->badpct = 0;
			}
			objects[tpos].groups[tpos3]->rmsscr = 0;
			for (tpos4 = 0; tpos4 < ObjectSize; tpos4++)
			{
				if (objects[tpos4].existst == 1 && objects[tpos4].IsInGroup(objects[tpos].groups[tpos3], ByObj) == true)
				{
					objects[tpos].groups[tpos3]->rmsscr += pow(((objects[tpos4].score) - (objects[tpos].groups[tpos3]->averagescr)), 2);
				}
			}
			objects[tpos].groups[tpos3]->rmsscr = pow((objects[tpos].groups[tpos3]->rmsscr / objects[tpos].groups[tpos3]->size), 0.5);
		}
	}
	tpos4 = 0;
	for (tpos3 = 0; tpos3 < SubjectSize; tpos3++)
	{
		if (objects[tpos].subjectsexistst[tpos3] == 1)
		{
			objects[tpos].subjects[tpos3]->size--;
			objects[tpos].subjects[tpos3]->allobjectscr -= objects[tpos].subjectscores[tpos3];
			objects[tpos].subjects[tpos3]->averagescr = (objects[tpos].subjects[tpos3]->allobjectscr) / (objects[tpos].subjects[tpos3]->size);
			if (objects[tpos].subjectscores[tpos3] == objects[tpos].subjects[tpos3]->firstobjectscr)
			{
				objects[tpos].subjects[tpos3]->firstobjectscr = 0;
				for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
				{
					if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(objects[tpos].subjects[tpos3], ByObj, tpos5) == true && objects[tpos2].subjectscores[tpos5] > objects[tpos].subjects[tpos3]->firstobjectscr)
					{
						objects[tpos].subjects[tpos3]->firstobjectscr = objects[tpos2].subjectscores[tpos5];
					}
				}
			}
			if (objects[tpos].subjectscores[tpos3] == objects[tpos].subjects[tpos3]->lastobjectscr)
			{
				if (objects[tpos].subjects[tpos3]->size == 0)
				{
					objects[tpos].subjects[tpos3]->lastobjectscr = 0;
				}
				else
				{
					objects[tpos].subjects[tpos3]->lastobjectscr = objects[tpos].subjects[tpos3]->firstobjectscr;
					for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
					{
						if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(objects[tpos].subjects[tpos3], ByObj, tpos5) == true && objects[tpos2].subjectscores[tpos5] < objects[tpos].subjects[tpos3]->lastobjectscr)
						{
							objects[tpos].subjects[tpos3]->lastobjectscr = objects[tpos2].subjectscores[tpos5];
						}
					}
				}
			}
			if (objects[tpos].subjects[tpos3]->size != 0)
			{
				if (objects[tpos].subjectscores[tpos3] == objects[tpos].subjects[tpos3]->fullscr)
				{
					objects[tpos].subjects[tpos3]->fullpct = (objects[tpos].subjects[tpos3]->fullpct * (objects[tpos].subjects[tpos3]->size + 1) - 100) / (objects[tpos].subjects[tpos3]->size);
				}
				if (objects[tpos].subjectscores[tpos3] >= objects[tpos].subjects[tpos3]->exellentscr)
				{
					objects[tpos].subjects[tpos3]->exellentpct = (objects[tpos].subjects[tpos3]->exellentpct * (objects[tpos].subjects[tpos3]->size + 1) - 100) / (objects[tpos].subjects[tpos3]->size);
				}
				if (objects[tpos].subjectscores[tpos3] >= objects[tpos].subjects[tpos3]->passscr)
				{
					objects[tpos].subjects[tpos3]->passpct = (objects[tpos].subjects[tpos3]->passpct * (objects[tpos].subjects[tpos3]->size + 1) - 100) / (objects[tpos].subjects[tpos3]->size);
				}
				if (objects[tpos].subjectscores[tpos3] <= objects[tpos].subjects[tpos3]->badscr)
				{
					objects[tpos].subjects[tpos3]->badpct = (objects[tpos].subjects[tpos3]->badpct * (objects[tpos].subjects[tpos3]->size + 1) - 100) / (objects[tpos].subjects[tpos3]->size);
				}
			}
			else
			{
				objects[tpos].subjects[tpos3]->fullpct = 0;
				objects[tpos].subjects[tpos3]->exellentpct = 0;
				objects[tpos].subjects[tpos3]->passpct = 0;
				objects[tpos].subjects[tpos3]->badpct = 0;
			}
			objects[tpos].subjects[tpos3]->rmsscr = 0;
			for (tpos4 = 0; tpos4 < ObjectSize; tpos4++)
			{
				if (objects[tpos4].existst == 1 && objects[tpos4].IsInSubject(objects[tpos].subjects[tpos3], ByObj, tpos5) == true)
				{
					objects[tpos].subjects[tpos3]->rmsscr += pow(((objects[tpos4].subjectscores[tpos5]) - (objects[tpos].subjects[tpos3]->averagescr)), 2);
				}
			}
			objects[tpos].subjects[tpos3]->rmsscr = pow((objects[tpos].subjects[tpos3]->rmsscr / objects[tpos].subjects[tpos3]->size), 0.5);
		}
	}
	return true;
}

void StatisticsGlobal::GetObject(char*& tnm, char*& tnb, short& tsx, float& tsc, long& tps)
{
	strcpy(tnm, objects[objectIndex].name);
	strcpy(tnb, objects[objectIndex].number);
	tsx = objects[objectIndex].sex;
	tsc = objects[objectIndex].score;
	tps = objects[objectIndex].rank;
}

bool StatisticsGlobal::NextObject(long times)
{
	if (times == 0)
		return true;
	long tpos = objects[objectIndex].rank + times;
	if (tpos > size)
	{
		errcode = Error_NoSuchItem;
		return false;
	}
	long tpos2;
retry2:
	for (tpos2 = 0; ((tpos != objects[tpos2].rank || objects[tpos2].existst != 1) && tpos2 < ObjectSize) || tpos2 == objectIndex; tpos2++)
	{
	}
	if (tpos2 == ObjectSize)
	{
		tpos++;
		if (tpos > size)
		{
			errcode = Error_NoSuchItem;
			return false;
		}
		goto retry2;
	}
	if ((objects[objectIndex].rank + times) != tpos)
	{
		errcode = Error_NoSuchItem_RankInc;
		objectIndex = tpos2;
		return false;
	}
	objectIndex = tpos2;
	return true;
}

bool StatisticsGlobal::NastObject(long times)
{
	if (times == 0)
		return true;
	long tpos = objects[objectIndex].rank - times;
	if (tpos <= 0)
	{
		errcode = Error_NoSuchItem;
		return false;
	}
	long tpos2;
retry3:
	for (tpos2 = 0; ((tpos != objects[tpos2].rank || objects[tpos2].existst != 1) && tpos2 < ObjectSize) || tpos2 == objectIndex; tpos2++)
	{
	}
	if (tpos2 == ObjectSize)
	{
		tpos--;
		if (tpos < 1)
		{
			errcode = Error_NoSuchItem;
			return false;
		}
		goto retry3;
	}
	if ((objects[objectIndex].rank - times) != tpos)
	{
		errcode = Error_NoSuchItem_RankDec;
		objectIndex = tpos2;
		return false;
	}
	objectIndex = tpos2;
	return true;
}

void StatisticsGlobal::getscrline(StatisticsSubject* sub, float& esc, float& bsc, float& psc, float& fsc)
{
	if (sub == NULL)
	{
		fsc = subjects[subjectIndex].fullscr;
		esc = subjects[subjectIndex].exellentscr;
		bsc = subjects[subjectIndex].badscr;
		psc = subjects[subjectIndex].passscr;
	}
	else
	{
		fsc = sub->fullscr;
		esc = sub->exellentscr;
		bsc = sub->badscr;
		psc = sub->passscr;
	}
}

bool StatisticsGlobal::GoToObject(unsigned long pos, short errorHandling = ErrorHandling_IncRank)
{
	if (pos > ObjectSize)
	{
		errcode = Error_NoSuchItem;
		return false;
	}
	long tpos;
	long tpos2 = pos;
retry6:
	for (tpos = 0; (objects[tpos].rank != pos || objects[tpos].existst != 1) && tpos < ObjectSize; tpos++) {}
	if (tpos == ObjectSize)
	{
		if (errorHandling == ErrorHandling_IncRank)
			pos++;
		else
			pos--;
		if (pos > ObjectSize || pos <= 0)
		{
			errcode = Error_NoSuchItem;
			return false;
		}
		goto retry6;
	}
	if (tpos2 != pos)
	{
		errcode = errorHandling + 6;
		objectIndex = tpos;
		return false;
	}
	objectIndex = tpos;
	return true;
}

void StatisticsGlobal::setscrline(StatisticsSubject* sub, float esc, float bsc, float psc, float fsc)
{
	if (sub == NULL)
	{
		subjects[subjectIndex].fullscr = fsc;
		subjects[subjectIndex].exellentscr = esc;
		subjects[subjectIndex].badscr = bsc;
		subjects[subjectIndex].passscr = psc;
	}
	else
	{
		sub->fullscr = fsc;
		sub->exellentscr = esc;
		sub->badscr = bsc;
		sub->passscr = psc;
	}
}

void StatisticsGlobal::GetPercents(float& epct, float& tpct, float& ppct)
{
	long tpos;
	exellentpct = 0;
	for (tpos = 0; tpos < ObjectSize; tpos++)
	{
		if (objects[tpos].score >= exellentscr && objects[tpos].existst == 1)
		{
			exellentpct++;
		}
	}
	if (exellentpct > 0 && size > 0)
	{
		exellentpct = exellentpct / size * 100;
	}
	else
	{
		exellentpct = 0;
	}
	badpct = 0;
	for (tpos = 0; tpos < ObjectSize; tpos++)
	{
		if (objects[tpos].score <= badscr && objects[tpos].existst == 1)
		{
			badpct++;
		}
	}
	if (badpct > 0 && size > 0)
	{
		badpct = badpct / size * 100;
	}
	else
	{
		badpct = 0;
	}
	passpct = 0;
	for (tpos = 0; tpos < ObjectSize; tpos++)
	{
		if (objects[tpos].score >= passscr && objects[tpos].existst == 1)
		{
			passpct++;
		}
	}
	if (passpct > 0 && size > 0)
	{
		passpct = passpct / size * 100;
	}
	else
	{
		passpct = 0;
	}
	epct = exellentpct;
	tpct = badpct;
	ppct = passpct;
}

bool StatisticsGlobal::NextObject(char*& onm, char*& onb, short& osx, float& osc, long& ops)
{
	long tpos;
	for (tpos = objectIndex + 1; objects[tpos].existst != 1 && tpos < ObjectSize; tpos++)
	{
	}
	if (tpos == ObjectSize)
	{
		strcpy(onm, "NULLOBJECT");
		errcode = Error_NoSuchItem;
		return false;
	}
	else
	{
		strcpy(onm, objects[tpos].name);
		onb = objects[tpos].number;
		osx = objects[tpos].sex;
		osc = objects[tpos].score;
		ops = objects[tpos].rank;
		objectIndex = tpos;
	}
	return true;
}

bool StatisticsGlobal::SetObjectIndex(long index, bool forced = false)
{
	if ((forced == false && index < ObjectSize && index >= 0) || forced = true)
	{
		objectIndex = index;
		if ((forced == false && objects[index].existst == 1) || forced == true)
		{
			return true;
		}
	}
	errcode = Error_NoSuchItem;
	return false;
}

void StatisticsGlobal::GetObjectIndex(long& index)
{
	index = objectIndex;
}

bool StatisticsGlobal::AddGroup(char* gnm, short gtp)
{
	long tpos;
	long tpos2 = -1;
	long tpos3;
	for (tpos = 0; tpos < GroupSize; tpos++)
	{
		if (groups[tpos].existst != 1 && tpos2 < 0)
		{
			tpos2 = tpos;
		}
		if (groups[tpos].existst == 1)
		{
			if (strcmp(groups[tpos].name, gnm) == 0)
			{
				errcode = Error_ItemExists;;
				return false;
			}
		}
	}
	tpos = tpos2;
	if (tpos == GroupSize)
	{
		errcode = Error_FullMemory;
		return false;
	}
	strcpy(groups[tpos].name, gnm);
	groups[tpos].type = gtp;
	groups[tpos].existst = 1;
	if (gtp == 1)
	{
		for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
		{
			if (objects[tpos2].existst == 1)
			{
				for (tpos3 = 0; objects[tpos2].groupsexistst[tpos3] == 1 && tpos3 < GroupSize; tpos3++)
				{
				}
				if (tpos3 < GroupSize)
				{
					objects[tpos2].groups[tpos3] = groups + tpos;
					objects[tpos2].groupsexistst[tpos3] = 1;
				}
				else
				{
					errcode = Error_FullMemory;
					return false;
				}
			}
		}
		groups[tpos].size = size;
		groups[tpos].allobjectscr = allobjectscr;
		groups[tpos].averagescr = averagescr;
		groups[tpos].rmsscr = rmsscr;
		groups[tpos].firstobjectscr = firstobjectscr;
		groups[tpos].lastobjectscr = lastobjectscr;
		groups[tpos].exellentpct = exellentpct;
		groups[tpos].passpct = passpct;
		groups[tpos].badpct = badpct;
	}
	else
	{
		groups[tpos].size = 0;
		groups[tpos].allobjectscr = 0;
		groups[tpos].averagescr = 0;
		groups[tpos].rmsscr = 0;
		groups[tpos].firstobjectscr = 0;
		groups[tpos].lastobjectscr = 0;
		groups[tpos].exellentpct = 0;
		groups[tpos].passpct = 0;
		groups[tpos].badpct = 0;
	}
	for (tpos2 = 0; (groups[tpos2].existst != 1 || &groups[tpos2] == &groups[tpos]) && tpos2 < GroupSize; tpos2++) {}
	if (tpos == GroupSize)
	{
		groupIndex = tpos;
	}
	return true;
}

void StatisticsGlobal::GetGroup(char*& gnm, short& gtp, long& gsz, float& gavg, float& ggap, float& grms)
{
	strcpy(gnm, groups[groupIndex].name);
	gtp = groups[groupIndex].type;
	gsz = groups[groupIndex].size;
	gavg = groups[groupIndex].averagescr;
	ggap = groups[groupIndex].firstobjectscr - groups[groupIndex].lastobjectscr;
	grms = groups[groupIndex].rmsscr;
}

bool StatisticsGlobal::SearchGroup(void* si, short type)
{
	long tpos = 0;
	switch (type)
	{
	case ByName:
		for (; (strcmp(groups[tpos].name, (const char*)si) != 0 || groups[tpos].existst != 1) && tpos < GroupSize; tpos++) {}
		break;
	case ByObj:
		for (; (&groups[tpos] != (StatisticsGroup*)si || groups[tpos].existst != 1) && tpos < GroupSize; tpos++) {}
		break;
	default:
		break;
	}
	if (tpos == GroupSize)
	{
		errcode = Error_NoSuchItem;
		return false;
	}
	groupIndex = tpos;
	return true;
}

bool StatisticsGlobal::AddToGroup(long gpos, short gtp)
{
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	for (; tpos2 < gpos && tpos < GroupSize; tpos++)
	{
		if (groups[tpos].existst == 1 && groups[tpos].type == gtp)
		{
			tpos2++;
		}
	}
	tpos2 = -1;
	for (tpos3 = 0; tpos3 < GroupSize; tpos3++)
	{
		if (objects[objectIndex].groupsexistst[tpos3] != 1 && tpos2 < 0)
		{
			tpos2 = tpos3;
		}
		if (objects[objectIndex].groupsexistst[tpos3] == 1)
		{
			if (strcmp(groups[tpos].name, objects[objectIndex].groups[tpos3]->name) == 0)
			{
				errcode = Error_ItemExists;;
				return false;
			}
		}
	}
	tpos3 = tpos2;
	if (tpos3 < 0)
	{
		errcode = Error_FullMemory;
		return false;
	}
	objects[objectIndex].groups[tpos3] = &groups[tpos];
	objects[objectIndex].groupsexistst[tpos3] = 1;
	groups[tpos].size++;
	groups[tpos].allobjectscr += objects[objectIndex].score;
	groups[tpos].averagescr = (groups[tpos].allobjectscr) / (groups[tpos].size);
	if (objects[objectIndex].score > groups[tpos].firstobjectscr)
	{
		groups[tpos].firstobjectscr = objects[objectIndex].score;
	}
	if (objects[objectIndex].score < groups[tpos].lastobjectscr || groups[tpos].size == 1)
	{
		groups[tpos].lastobjectscr = objects[objectIndex].score;
	}
	groups[tpos].rmsscr = 0;
	for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
	{
		if (objects[tpos2].existst == 1)
		{
			for (tpos3 = 0; ((objects[tpos2].groups[tpos3]) != groups + tpos || (objects[tpos2].groupsexistst[tpos3]) != 1) && tpos3 < GroupSize; tpos3++) {}
			if (tpos3 < GroupSize)
			{
				groups[tpos].rmsscr += pow(((objects[tpos2].score) - (groups[tpos].averagescr)), 2);
			}
		}
	}
	groups[tpos].rmsscr = pow((groups[tpos].rmsscr / groups[tpos].size), 0.5);
	return true;
}

bool StatisticsGlobal::AddToGroup(const char* gnm, long opos)
{
	if (opos == -1)
	{
		opos = objectIndex;
	}
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	for (; tpos < GroupSize && !(groups[tpos].existst == 1 && strcmp(groups[tpos].name, gnm) == 0); tpos++)
	{
	}
	tpos2 = -1;
	for (tpos3 = 0; tpos3 < GroupSize; tpos3++)
	{
		if (objects[opos].groupsexistst[tpos3] != 1 && tpos2 < 0)
		{
			tpos2 = tpos3;
		}
		if (objects[opos].groupsexistst[tpos3] == 1)
		{
			if (strcmp(groups[tpos].name, objects[opos].groups[tpos3]->name) == 0)
			{
				errcode = Error_ItemExists;;
				return false;
			}
		}
	}
	tpos3 = tpos2;
	if (tpos3 < 0)
	{
		errcode = Error_FullMemory;
		return false;
	}
	objects[opos].groups[tpos3] = &groups[tpos];
	objects[opos].groupsexistst[tpos3] = 1;
	groups[tpos].size++;
	groups[tpos].allobjectscr += objects[opos].score;
	groups[tpos].averagescr = (groups[tpos].allobjectscr) / (groups[tpos].size);
	if (objects[opos].score > groups[tpos].firstobjectscr)
	{
		groups[tpos].firstobjectscr = objects[opos].score;
	}
	if (objects[opos].score < groups[tpos].lastobjectscr || groups[tpos].size == 1)
	{
		groups[tpos].lastobjectscr = objects[opos].score;
	}
	groups[tpos].rmsscr = 0;
	for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
	{
		if (objects[tpos2].existst == 1)
		{
			for (tpos3 = 0; ((objects[tpos2].groups[tpos3]) != groups + tpos || (objects[tpos2].groupsexistst[tpos3]) != 1) && tpos3 < GroupSize; tpos3++) {}
			if (tpos3 < GroupSize)
			{
				groups[tpos].rmsscr += pow(((objects[tpos2].score) - (groups[tpos].averagescr)), 2);
			}
		}
	}
	groups[tpos].rmsscr = pow((groups[tpos].rmsscr / groups[tpos].size), 0.5);
	return true;
}

void StatisticsGlobal::NextGroup(char*& gnm, short gtp)
{
	long tpos;
	if (gtp != AllGroups)
	{
		for (tpos = groupIndex + 1; (groups[tpos].existst != 1 || groups[tpos].type != gtp) && tpos < GroupSize; tpos++)
		{
		}
	}
	else
	{
		for (tpos = groupIndex + 1; groups[tpos].existst != 1 && tpos < GroupSize; tpos++)
		{
		}
	}
	if (tpos == GroupSize)
	{
		strcpy(gnm, "NULLGROUP");
		return;
	}
	else
	{
		strcpy(gnm, groups[tpos].name);
		groupIndex = tpos;
	}
}

bool StatisticsGlobal::SetGroupIndex(long index, bool forced = false)
{
	if ((forced == false && index < GroupSize && index >= 0) || forced = true)
	{
		groupIndex = index;
		if ((forced == false && groups[index].existst == 1) || forced == true)
		{
			return true;
		}
	}
	errcode = Error_NoSuchItem;
	return false;
}

void StatisticsGlobal::GetGroupIndex(long& index)
{
	index = groupIndex;
}

void StatisticsGlobal::getgrouppercents(float& epct, float& tpct, float& ppct)
{
	long tpos;
	groups[groupIndex].exellentpct = 0;
	groups[groupIndex].badpct = 0;
	groups[groupIndex].passpct = 0;
	for (tpos = 0; tpos < ObjectSize; tpos++)
	{
		if (objects[tpos].existst == 1 && objects[tpos].IsInGroup(&groups[groupIndex], ByObj) == true && objects[tpos].score >= exellentscr)
		{
			groups[groupIndex].exellentpct++;
		}
		if (objects[tpos].existst == 1 && objects[tpos].IsInGroup(&groups[groupIndex], ByObj) == true && objects[tpos].score <= badscr)
		{
			groups[groupIndex].badpct++;
		}
		if (objects[tpos].existst == 1 && objects[tpos].IsInGroup(&groups[groupIndex], ByObj) == true && objects[tpos].score >= passscr)
		{
			groups[groupIndex].passpct++;
		}
	}
	if (groups[groupIndex].size > 0)
	{
		groups[groupIndex].exellentpct = (groups[groupIndex].exellentpct) / (groups[groupIndex].size) * 100;
		groups[groupIndex].badpct = (groups[groupIndex].badpct) / (groups[groupIndex].size) * 100;
		groups[groupIndex].passpct = (groups[groupIndex].passpct) / (groups[groupIndex].size) * 100;
	}
	else
	{
		groups[groupIndex].exellentpct = 0;
		groups[groupIndex].badpct = 0;
		groups[groupIndex].passpct = 0;
	}
	epct = groups[groupIndex].exellentpct;
	tpct = groups[groupIndex].badpct;
	ppct = groups[groupIndex].passpct;
}

bool StatisticsGlobal::SetUncertainScore(short settype, float scr)
{
	if (settype > 3 || settype < 1)
	{
		errcode = Error_NoSuchItem;
		return false;
	}
	if (scr < 0 && settype == 3)
	{
		errcode = ERR_LACKPARAMETER;
		return false;
	}
	long tpos = 0;
	long tpos2 = 0;
	for (tpos = 0; tpos < ObjectSize; tpos++)
	{
		if (objects[tpos].existst == 1)
		{
			for (tpos2 = 0; tpos2 < SubjectSize; tpos2++)
			{
				if ((objects[tpos].subjectsexistst[tpos2]) == 1 && objects[tpos].subjectscores[tpos2] == -1.f)
				{
					switch (settype)
					{
					case SETUNCERTAIN_FULLMARK:
						objects[tpos].subjectscores[tpos2] = objects[tpos].subjects[tpos2]->fullscr;
						break;
					case SETUNCERTAIN_ZERO:
						objects[tpos].subjectscores[tpos2] = 0;
						break;
					case SETUNCERTAIN_CUSTOMIZE:
						objects[tpos].subjectscores[tpos2] = scr;
						break;
					default:
						errcode = Error_NoSuchItem;
						return false;
					}
					break;
				}
			}
		}
	}
	return true;
}

int StatisticsGlobal::GetLastError()
{
	int tmperrcode = errcode;
	errcode = 0;
	return tmperrcode;
}

void StatisticsGlobal::MarkUncertain()
{
	long tpos = 0;
	long tpos2 = 0;
	for (tpos = 0; tpos < ObjectSize; tpos++)
	{
		objects[tpos].marked = 0;
		if (objects[tpos].existst == 1)
		{
			for (tpos2 = 0; tpos2 < SubjectSize; tpos2++)
			{
				if ((objects[tpos].subjectsexistst[tpos2]) == 1 && objects[tpos].subjectscores[tpos2] == -1.f)
				{
					objects[tpos].marked = 1;
				}
			}
		}
	}
	return;
}

void StatisticsGlobal::MarkAll()
{
	long tpos = 0;
	for (tpos = 0; tpos < ObjectSize; tpos++)
	{
		objects[tpos].marked = 1;
	}
}

void StatisticsGlobal::UnmarkAll()
{
	long tpos = 0;
	for (tpos = 0; tpos < ObjectSize; tpos++)
	{
		objects[tpos].marked = 0;
	}
}

long StatisticsGlobal::GetMarked(long*& m)
{
	long tpos = 0;
	long tpos2 = 0;
	for (tpos = 0; tpos < ObjectSize; tpos++)
	{
		marked[tpos] = -1;
		if (objects[tpos].existst == 1 && objects[tpos].marked == 1)
		{
			marked[tpos2] = tpos;
			tpos2++;
		}
	}
	m = marked;
	return tpos2;
}

void StatisticsGlobal::InverseMark()
{
	long tpos = 0;
	for (tpos = 0; tpos < ObjectSize; tpos++)
	{
		objects[tpos].existst == 1 && objects[tpos].marked == 0 ? objects[tpos].marked = 1 : objects[tpos].marked = 0;
	}
}

bool StatisticsGlobal::AddSubject(char* snm, short stp, long maxsz, float sfm)
{
	long tpos;
	long tpos2 = -1;
	long tpos3;
	for (tpos = 0; tpos < SubjectSize; tpos++)
	{
		if (subjects[tpos].existst != 1 && tpos2 < 0)
		{
			tpos2 = tpos;
		}
		if (subjects[tpos].existst == 1)
		{
			if (strcmp(subjects[tpos].name, snm) == 0)
			{
				errcode = Error_ItemExists;;
				return false;
			}
		}
	}
	tpos = tpos2;
	if (tpos == SubjectSize)
	{
		errcode = Error_FullMemory;
		return false;
	}
	strcpy(subjects[tpos].name, snm);
	subjects[tpos].type = stp;
	subjects[tpos].existst = 1;
	if (stp == 1)
	{
		for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
		{
			if (objects[tpos2].existst == 1)
			{
				for (tpos3 = 0; objects[tpos2].subjectsexistst[tpos3] == 1 && tpos3 < SubjectSize; tpos3++)
				{
				}
				if (tpos3 < SubjectSize)
				{
					objects[tpos2].subjects[tpos3] = subjects + tpos;
					objects[tpos2].subjectsexistst[tpos3] = 1;
					objects[tpos2].subjectscores[tpos3] = -1.f;
				}
				else
				{
					errcode = Error_FullMemory;
					return false;
				}
			}
		}
		subjects[tpos].size = size;
		subjects[tpos].maxsize = ObjectSize;
	}
	else
	{
		subjects[tpos].size = 0;
		if (maxsz == 0)
		{
			subjects[tpos].maxsize = ObjectSize;
		}
		else
		{
			subjects[tpos].maxsize = maxsz;
		}
	}
	subjects[tpos].allobjectscr = 0;
	subjects[tpos].averagescr = 0;
	subjects[tpos].rmsscr = 0;
	subjects[tpos].firstobjectscr = 0;
	subjects[tpos].lastobjectscr = 0;
	subjects[tpos].fullscr = sfm;
	for (tpos2 = 0; (subjects[tpos2].existst != 1 || &subjects[tpos2] == &subjects[tpos]) && tpos2 < SubjectSize; tpos2++) {}
	if (tpos == SubjectSize)
	{
		subjectIndex = tpos;
	}
	return true;
}

bool StatisticsGlobal::AddToSubject(long spos, short stp, float sc)
{
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	for (; tpos2 < spos && tpos < SubjectSize; tpos++)
	{
		if (subjects[tpos].existst == 1 && subjects[tpos].type == stp)
		{
			tpos2++;
		}
	}
	tpos2 = -1;
	for (tpos3 = 0; tpos3 < SubjectSize; tpos3++)
	{
		if (objects[objectIndex].subjectsexistst[tpos3] != 1 && tpos2 < 0)
		{
			tpos2 = tpos3;
		}
		if (objects[objectIndex].subjectsexistst[tpos3] == 1)
		{
			if (strcmp(subjects[tpos].name, objects[objectIndex].subjects[tpos3]->name) == 0)
			{
				errcode = Error_ItemExists;;
				return false;
			}
		}
	}
	if (tpos2 < 0)
	{
		errcode = Error_FullMemory;
		return false;
	}
	objects[objectIndex].subjects[tpos2] = subjects + tpos;
	objects[objectIndex].subjectsexistst[tpos2] = 1;
	subjects[tpos].size++;
	if (sc == -1.f)
	{
		objects[objectIndex].subjectscores[tpos2] = -1;
	}
	else
	{
		objects[objectIndex].subjectscores[tpos2] = 0;
		changesubscr(sc, subjects[tpos].name, objects + objectIndex);
	}
	/*objects[objectIndex].score += tsc;
	subjects[tpos].allobjectscr += tsc;
	subjects[tpos].averagescr = (subjects[tpos].allobjectscr) / (subjects[tpos].size);
	if (objects[objectIndex].score > subjects[tpos].firstobjectscr)
	{
		subjects[tpos].firstobjectscr = objects[objectIndex].score;
	}
	if (objects[objectIndex].score < subjects[tpos].lastobjectscr || subjects[tpos].size == 1)
	{
		subjects[tpos].lastobjectscr = objects[objectIndex].score;
	}
	subjects[tpos].rmsscr = 0;
	for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
	{
		if (objects[tpos2].existst == 1)
		{
			for (tpos3 = 0; ((objects[tpos2].subjects[tpos3]) != subjects + tpos || (objects[tpos2].subjectsexistst[tpos3]) != 1) && tpos3 < SubjectSize; tpos3++) {}
			if (tpos3 < SubjectSize)
			{
				subjects[tpos].rmsscr += pow(((objects[tpos2].score) - (subjects[tpos].averagescr)), 2);
			}
		}
	}
	subjects[tpos].rmsscr = pow((subjects[tpos].rmsscr / subjects[tpos].size), 0.5);*/
	return true;
}

void StatisticsGlobal::GetSubject(char*& snm, short& stp, long& ssz, float& savg, float& sgap, float& srms)
{
	strcpy(snm, subjects[subjectIndex].name);
	stp = subjects[subjectIndex].type;
	ssz = subjects[subjectIndex].size;
	savg = subjects[subjectIndex].averagescr;
	sgap = subjects[subjectIndex].firstobjectscr - subjects[subjectIndex].lastobjectscr;
	srms = subjects[subjectIndex].rmsscr;
}

bool StatisticsGlobal::AddToSubject(const char* snm, long opos)
{
	if (opos == -1)
	{
		opos = objectIndex;
	}
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	for (; tpos < SubjectSize && !(subjects[tpos].existst == 1 && strcmp(subjects[tpos].name, snm) == 0); tpos++)
	{
	}
	tpos2 = -1;
	for (tpos3 = 0; tpos3 < SubjectSize; tpos3++)
	{
		if (objects[opos].subjectsexistst[tpos3] != 1 && tpos2 < 0)
		{
			tpos2 = tpos3;
		}
		if (objects[opos].subjectsexistst[tpos3] == 1)
		{
			if (strcmp(subjects[tpos].name, objects[opos].subjects[tpos3]->name) == 0)
			{
				errcode = Error_ItemExists;;
				return false;
			}
		}
	}
	if (tpos2 < 0)
	{
		errcode = Error_FullMemory;
		return false;
	}
	objects[opos].subjects[tpos2] = subjects + tpos;
	objects[opos].subjectsexistst[tpos2] = 1;
	objects[opos].subjectscores[tpos2] = -1;
	subjects[tpos].size++;
	return true;
}

bool StatisticsGlobal::DeleteFromGroup(long gpos, short gtp)
{
	if (size == 0)
	{
		errcode = Error_EmptyMemory;
		return false;
	}
	long tpos = 0;
	long tpos2 = 0;
	for (; tpos2 < gpos && tpos < GroupSize; tpos++)
	{
		if (groups[tpos].existst == 1 && groups[tpos].type == gtp)
		{
			tpos2++;
		}
	}
	if (tpos == GroupSize && tpos2 < gpos)
	{
		errcode = Error_NoSuchItem;
		return false;
	}
	groups[tpos].size--;
	groups[tpos].allobjectscr -= objects[objectIndex].score;
	groups[tpos].averagescr = (groups[tpos].allobjectscr) / (groups[tpos].size);
	if (objects[objectIndex].score == groups[tpos].firstobjectscr)
	{
		groups[tpos].firstobjectscr = 0;
		for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
		{
			if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(&groups[tpos], ByObj) == true && objects[tpos2].score > groups[tpos].firstobjectscr)
			{
				groups[tpos].firstobjectscr = objects[tpos2].score;
			}
		}
	}
	if (objects[objectIndex].score == groups[tpos].lastobjectscr)
	{
		if (groups[tpos].size == 0)
		{
			groups[tpos].lastobjectscr = 0;
		}
		else
		{
			groups[tpos].lastobjectscr = groups[tpos].firstobjectscr;
			for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
			{
				if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(&groups[tpos], ByObj) == true && objects[tpos2].score < groups[tpos].lastobjectscr)
				{
					groups[tpos].lastobjectscr = objects[tpos2].score;
				}
			}
		}
	}
	if (groups[tpos].size != 0)
	{
		if (objects[objectIndex].score >= exellentscr)
		{
			groups[tpos].exellentpct = (groups[tpos].exellentpct * (groups[tpos].size + 1) - 100) / (groups[tpos].size);
		}
		if (objects[objectIndex].score >= passscr)
		{
			groups[tpos].passpct = (groups[tpos].passpct * (groups[tpos].size + 1) - 100) / (groups[tpos].size);
		}
		if (objects[objectIndex].score <= badscr)
		{
			groups[tpos].badpct = (groups[tpos].badpct * (groups[tpos].size + 1) - 100) / (groups[tpos].size);
		}
	}
	else
	{
		groups[tpos].exellentpct = 0;
		groups[tpos].passpct = 0;
		groups[tpos].badpct = 0;
	}
	groups[tpos].rmsscr = 0;
	for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
	{
		if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(&groups[tpos], ByObj) == true)
		{
			groups[tpos].rmsscr += pow(((objects[tpos2].score) - (groups[tpos].averagescr)), 2);
		}
	}
	groups[tpos].rmsscr = pow((groups[tpos].rmsscr / groups[tpos].size), 0.5);
	for (; tpos2 < GroupSize && !(objects[objectIndex].groupsexistst[tpos2] == 1 && objects[objectIndex].groups[tpos2]->existst == 1 && objects[objectIndex].groups[tpos2] == &groups[tpos]); tpos2++)
	{
	}
	if (tpos2 < GroupSize)
	{
		objects[objectIndex].groupsexistst[tpos2] = 0;
		objects[objectIndex].groups[tpos2] = NULL;
	}
	return true;
}

bool StatisticsGlobal::DeleteFromGroup(const char* gnm, long opos)
{
	if (opos == -1)
	{
		opos = objectIndex;
	}
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	for (; tpos < GroupSize && !(objects[opos].groupsexistst[tpos] == 1 && objects[opos].groups[tpos]->existst == 1 && strcmp(objects[opos].groups[tpos]->name, gnm) == 0); tpos++)
	{
	}
	objects[opos].groupsexistst[tpos] = 0;
	objects[opos].groups[tpos]->size--;
	objects[opos].groups[tpos]->allobjectscr -= objects[opos].score;
	objects[opos].groups[tpos]->averagescr = (objects[opos].groups[tpos]->allobjectscr) / (objects[opos].groups[tpos]->size);
	if (objects[opos].score == objects[opos].groups[tpos]->firstobjectscr)
	{
		objects[opos].groups[tpos]->firstobjectscr = 0;
		for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
		{
			if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(objects[opos].groups[tpos], ByObj) == true && objects[tpos2].score > objects[opos].groups[tpos]->firstobjectscr)
			{
				objects[opos].groups[tpos]->firstobjectscr = objects[tpos2].score;
			}
		}
	}
	if (objects[opos].score == objects[opos].groups[tpos]->lastobjectscr)
	{
		if (objects[opos].groups[tpos]->size == 0)
		{
			objects[opos].groups[tpos]->lastobjectscr = 0;
		}
		else
		{
			objects[opos].groups[tpos]->lastobjectscr = objects[opos].groups[tpos]->firstobjectscr;
			for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
			{
				if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(objects[opos].groups[tpos], ByObj) == true && objects[tpos2].score < objects[opos].groups[tpos]->lastobjectscr)
				{
					objects[opos].groups[tpos]->lastobjectscr = objects[tpos2].score;
				}
			}
		}
	}
	if (objects[opos].groups[tpos]->size != 0)
	{
		if (objects[opos].score >= exellentscr)
		{
			objects[opos].groups[tpos]->exellentpct = (objects[opos].groups[tpos]->exellentpct * (objects[opos].groups[tpos]->size + 1) - 100) / (objects[opos].groups[tpos]->size);
		}
		if (objects[opos].score >= passscr)
		{
			objects[opos].groups[tpos]->passpct = (objects[opos].groups[tpos]->passpct * (objects[opos].groups[tpos]->size + 1) - 100) / (objects[opos].groups[tpos]->size);
		}
		if (objects[opos].score <= badscr)
		{
			objects[opos].groups[tpos]->badpct = (objects[opos].groups[tpos]->badpct * (objects[opos].groups[tpos]->size + 1) - 100) / (objects[opos].groups[tpos]->size);
		}
	}
	else
	{
		objects[opos].groups[tpos]->exellentpct = 0;
		objects[opos].groups[tpos]->passpct = 0;
		objects[opos].groups[tpos]->badpct = 0;
	}
	objects[opos].groups[tpos]->rmsscr = 0;
	for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
	{
		if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(objects[opos].groups[tpos], ByObj) == true)
		{
			objects[opos].groups[tpos]->rmsscr += pow(((objects[tpos2].score) - (objects[opos].groups[tpos]->averagescr)), 2);
		}
	}
	objects[opos].groups[tpos]->rmsscr = pow((objects[opos].groups[tpos]->rmsscr / objects[opos].groups[tpos]->size), 0.5);
	objects[opos].groups[tpos] = NULL;
	return true;
}

bool StatisticsGlobal::DeleteFromSubject(long spos, short stp, float* sc)
{
	if (size == 0)
	{
		errcode = Error_EmptyMemory;
		return false;
	}
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	long tpos4 = 0;
	for (; tpos2 < spos && tpos < SubjectSize; tpos++)
	{
		if (subjects[tpos].existst == 1 && subjects[tpos].type == stp)
		{
			tpos2++;
		}
	}
	if (tpos == SubjectSize && tpos2 < spos)
	{
		errcode = Error_NoSuchItem;
		return false;
	}
	objects[objectIndex].IsInSubject(&subjects[tpos], ByObj, tpos3);
	subjects[tpos].size--;
	subjects[tpos].allobjectscr -= objects[objectIndex].subjectscores[tpos3];
	subjects[tpos].averagescr = (subjects[tpos].allobjectscr) / (subjects[tpos].size);
	if (objects[objectIndex].subjectscores[tpos3] == subjects[tpos].firstobjectscr)
	{
		subjects[tpos].firstobjectscr = 0;
		for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
		{
			if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(&subjects[tpos], ByObj, tpos4) == true && objects[tpos2].subjectscores[tpos4] > subjects[tpos].firstobjectscr)
			{
				subjects[tpos].firstobjectscr = objects[tpos2].subjectscores[tpos4];
			}
		}
	}
	if (objects[objectIndex].subjectscores[tpos3] == subjects[tpos].lastobjectscr)
	{
		if (subjects[tpos].size == 0)
		{
			subjects[tpos].lastobjectscr = 0;
		}
		else
		{
			subjects[tpos].lastobjectscr = subjects[tpos].firstobjectscr;
			for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
			{
				if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(&subjects[tpos], ByObj, tpos4) == true && objects[tpos2].subjectscores[tpos4] < subjects[tpos].lastobjectscr)
				{
					subjects[tpos].lastobjectscr = objects[tpos2].subjectscores[tpos4];
				}
			}
		}
	}
	if (subjects[tpos].size != 0)
	{
		if (objects[objectIndex].subjectscores[tpos3] == subjects[tpos].fullscr)
		{
			subjects[tpos].fullpct = (subjects[tpos].fullpct * (subjects[tpos].size + 1) - 100) / (subjects[tpos].size);
		}
		if (objects[objectIndex].subjectscores[tpos3] >= subjects[tpos].exellentscr)
		{
			subjects[tpos].exellentpct = (subjects[tpos].exellentpct * (subjects[tpos].size + 1) - 100) / (subjects[tpos].size);
		}
		if (objects[objectIndex].subjectscores[tpos3] >= subjects[tpos].passscr)
		{
			subjects[tpos].passpct = (subjects[tpos].passpct * (subjects[tpos].size + 1) - 100) / (subjects[tpos].size);
		}
		if (objects[objectIndex].subjectscores[tpos3] <= subjects[tpos].badscr)
		{
			subjects[tpos].badpct = (subjects[tpos].badpct * (subjects[tpos].size + 1) - 100) / (subjects[tpos].size);
		}
	}
	else
	{
		subjects[tpos].fullpct = 0;
		subjects[tpos].exellentpct = 0;
		subjects[tpos].passpct = 0;
		subjects[tpos].badpct = 0;
	}
	subjects[tpos].rmsscr = 0;
	for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
	{
		if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(&subjects[tpos], ByObj, tpos4) == true)
		{
			subjects[tpos].rmsscr += pow(((objects[tpos2].subjectscores[tpos4]) - (subjects[tpos].averagescr)), 2);
		}
	}
	subjects[tpos].rmsscr = pow((subjects[tpos].rmsscr / subjects[tpos].size), 0.5);
	for (; tpos2 < SubjectSize && !(objects[objectIndex].subjectsexistst[tpos2] == 1 && objects[objectIndex].subjects[tpos2]->existst == 1 && objects[objectIndex].subjects[tpos2] == &subjects[tpos]); tpos2++)
	{
	}
	if (tpos2 < SubjectSize)
	{
		objects[objectIndex].subjectsexistst[tpos2] = 0;
		objects[objectIndex].subjects[tpos2] = NULL;
	}
	return true;
}

bool StatisticsGlobal::DeleteFromSubject(const char* snm, long opos)
{
	if (opos == -1)
	{
		opos = objectIndex;
	}
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	for (; tpos < SubjectSize && !(objects[opos].subjectsexistst[tpos] == 1 && objects[opos].subjects[tpos]->existst == 1 && strcmp(objects[opos].subjects[tpos]->name, snm) == 0); tpos++)
	{
	}
	objects[opos].subjectsexistst[tpos] = 0;
	objects[opos].subjectscores[tpos] = -1;
	objects[opos].subjects[tpos]->size--;
	objects[opos].subjects[tpos]->allobjectscr -= objects[opos].subjectscores[tpos];
	objects[opos].subjects[tpos]->averagescr = (objects[opos].subjects[tpos]->allobjectscr) / (objects[opos].subjects[tpos]->size);
	if (objects[opos].subjectscores[tpos] == objects[opos].subjects[tpos]->firstobjectscr)
	{
		objects[opos].subjects[tpos]->firstobjectscr = 0;
		for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
		{
			if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(objects[opos].subjects[tpos], ByObj, tpos3) == true && objects[tpos2].subjectscores[tpos3] > objects[opos].subjects[tpos]->firstobjectscr)
			{
				objects[opos].subjects[tpos]->firstobjectscr = objects[tpos2].subjectscores[tpos3];
			}
		}
	}
	if (objects[opos].subjectscores[tpos] == objects[opos].subjects[tpos]->lastobjectscr)
	{
		if (objects[opos].subjects[tpos]->size == 0)
		{
			objects[opos].subjects[tpos]->lastobjectscr = 0;
		}
		else
		{
			objects[opos].subjects[tpos]->lastobjectscr = objects[opos].subjects[tpos]->firstobjectscr;
			for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
			{
				if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(objects[opos].subjects[tpos], ByObj, tpos3) == true && objects[tpos2].subjectscores[tpos3] < objects[opos].subjects[tpos]->lastobjectscr)
				{
					objects[opos].subjects[tpos]->lastobjectscr = objects[tpos2].subjectscores[tpos];
				}
			}
		}
	}
	if (objects[opos].subjects[tpos]->size != 0)
	{
		if (objects[opos].subjectscores[tpos] == objects[opos].subjects[tpos]->fullscr)
		{
			objects[opos].subjects[tpos]->fullpct = (objects[opos].subjects[tpos]->fullpct * (objects[opos].subjects[tpos]->size + 1) - 100) / (objects[opos].subjects[tpos]->size);
		}
		if (objects[opos].subjectscores[tpos] >= objects[opos].subjects[tpos]->exellentscr)
		{
			objects[opos].subjects[tpos]->exellentpct = (objects[opos].subjects[tpos]->exellentpct * (objects[opos].subjects[tpos]->size + 1) - 100) / (objects[opos].subjects[tpos]->size);
		}
		if (objects[opos].subjectscores[tpos] >= objects[opos].subjects[tpos]->passscr)
		{
			objects[opos].subjects[tpos]->passpct = (objects[opos].subjects[tpos]->passpct * (objects[opos].subjects[tpos]->size + 1) - 100) / (objects[opos].subjects[tpos]->size);
		}
		if (objects[opos].subjectscores[tpos] <= objects[opos].subjects[tpos]->badscr)
		{
			objects[opos].subjects[tpos]->badpct = (objects[opos].subjects[tpos]->badpct * (objects[opos].subjects[tpos]->size + 1) - 100) / (objects[opos].subjects[tpos]->size);
		}
	}
	else
	{
		objects[opos].subjects[tpos]->fullpct = 0;
		objects[opos].subjects[tpos]->exellentpct = 0;
		objects[opos].subjects[tpos]->passpct = 0;
		objects[opos].subjects[tpos]->badpct = 0;
	}
	objects[opos].subjects[tpos]->rmsscr = 0;
	for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
	{
		if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(objects[opos].subjects[tpos], ByObj, tpos3) == true)
		{
			objects[opos].subjects[tpos]->rmsscr += pow(((objects[tpos2].subjectscores[tpos3]) - (objects[opos].subjects[tpos]->averagescr)), 2);
		}
	}
	objects[opos].subjects[tpos]->rmsscr = pow((objects[opos].subjects[tpos]->rmsscr / objects[opos].subjects[tpos]->size), 0.5);
	objects[opos].subjects[tpos] = NULL;
	return true;
}

void StatisticsGlobal::getsubjectpercents(float& fpct, float& epct, float& tpct, float& ppct)
{
	long tpos;
	long tpos2;
	subjects[subjectIndex].fullpct = 0;
	subjects[subjectIndex].exellentpct = 0;
	subjects[subjectIndex].badpct = 0;
	subjects[subjectIndex].passpct = 0;
	for (tpos = 0; tpos < ObjectSize; tpos++)
	{
		if (objects[tpos].existst == 1 && objects[tpos].IsInSubject(&subjects[subjectIndex], ByObj, tpos2) == true && objects[tpos].subjectscores[tpos2] == subjects[subjectIndex].fullscr)
		{
			subjects[subjectIndex].fullpct++;
		}
		if (objects[tpos].existst == 1 && objects[tpos].IsInSubject(&subjects[subjectIndex], ByObj, tpos2) == true && objects[tpos].subjectscores[tpos2] >= subjects[subjectIndex].exellentscr)
		{
			subjects[subjectIndex].exellentpct++;
		}
		if (objects[tpos].existst == 1 && objects[tpos].IsInSubject(&subjects[subjectIndex], ByObj, tpos2) == true && objects[tpos].subjectscores[tpos2] <= subjects[subjectIndex].badscr)
		{
			subjects[subjectIndex].badpct++;
		}
		if (objects[tpos].existst == 1 && objects[tpos].IsInSubject(&subjects[subjectIndex], ByObj, tpos2) == true && objects[tpos].subjectscores[tpos2] >= subjects[subjectIndex].passscr)
		{
			subjects[subjectIndex].passpct++;
		}
	}
	if (subjects[subjectIndex].size > 0)
	{
		subjects[subjectIndex].fullpct = (subjects[subjectIndex].fullpct) / (subjects[subjectIndex].size) * 100;
		subjects[subjectIndex].exellentpct = (subjects[subjectIndex].exellentpct) / (subjects[subjectIndex].size) * 100;
		subjects[subjectIndex].badpct = (subjects[subjectIndex].badpct) / (subjects[subjectIndex].size) * 100;
		subjects[subjectIndex].passpct = (subjects[subjectIndex].passpct) / (subjects[subjectIndex].size) * 100;
	}
	else
	{
		subjects[subjectIndex].fullpct = 0;
		subjects[subjectIndex].exellentpct = 0;
		subjects[subjectIndex].badpct = 0;
		subjects[subjectIndex].passpct = 0;
	}
	fpct = subjects[subjectIndex].fullpct;
	epct = subjects[subjectIndex].exellentpct;
	tpct = subjects[subjectIndex].badpct;
	ppct = subjects[subjectIndex].passpct;
}

bool StatisticsGlobal::getscoredpercents(float& pct, short pms, StatisticsSubject* sub = NULL, StatisticsGroup* grp = NULL)
{
	pct = 0.f;
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	long count = 0;
redo:
	switch (pms)
	{
	case 1:
		if (sub == NULL)
		{
			sub = &subjects[subjectIndex];
		}
		if (sub->existst == 0)
		{
			pct = 0;
			errcode = ERR_LACKPARAMETER;
			return false;
		}
		if (grp == NULL)
		{
			grp = &groups[groupIndex];
		}
		if (grp->existst == 0)
		{
			pms = 2;
			goto redo;
		}
		for (tpos = 0; tpos < ObjectSize; tpos++)
		{
			if (objects[tpos].existst == 1)
			{
				for (tpos2 = 0; tpos2 < GroupSize; tpos2++)
				{
					if ((objects[tpos].groupsexistst[tpos2]) == 1 && (objects[tpos].groups[tpos2]) == grp)
					{
						for (tpos3 = 0; tpos3 < SubjectSize; tpos3++)
						{
							if ((objects[tpos].subjectsexistst[tpos3]) == 1 && (objects[tpos].subjects[tpos3]) == sub)
							{
								if (objects[tpos].subjectscores[tpos3] == -1.f)
								{
									errcode = ERR_UNCERTAINSCORE;
									return false;
								}
								pct += objects[tpos].subjectscores[tpos3];
								count++;
								break;
							}
						}
						break;
					}
				}
			}
		}
		if (count != 0 && sub->fullscr != 0)
		{
			pct = pct * 100 / count / sub->fullscr;
		}
		else
		{
			pct = 0;
		}
		break;
	case 2:
		if (sub == NULL)
		{
			sub = &subjects[subjectIndex];
		}
		if (sub->existst == 0)
		{
			pct = 0;
			errcode = ERR_LACKPARAMETER;
			return false;
		}
		for (tpos = 0; tpos < ObjectSize; tpos++)
		{
			if (objects[tpos].existst == 1)
			{
				for (tpos3 = 0; tpos3 < SubjectSize; tpos3++)
				{
					if ((objects[tpos].subjectsexistst[tpos3]) == 1 && (objects[tpos].subjects[tpos3]) == sub)
					{
						if (objects[tpos].subjectscores[tpos3] == -1.f)
						{
							errcode = ERR_UNCERTAINSCORE;
							return false;
						}
						pct += objects[tpos].subjectscores[tpos3];
						count++;
						break;
					}
				}
			}
		}
		if (count != 0 && sub->fullscr != 0)
		{
			pct = pct * 100 / count / sub->fullscr;
		}
		else
		{
			pct = 0;
		}
		break;
	default:
		pct = 0;
		errcode = Error_NoSuchItem;
		return false;
	}
	return true;
}

bool StatisticsGlobal::changesubscr(float scr, char* sub, StatisticsObject* obj)
{
	if (scr < 0)
	{
		errcode = ERR_LACKPARAMETER;
		return false;
	}
	long tpos = 0;
	StatisticsSubject* psub = NULL;
	if (sub == NULL)
	{
		psub = &subjects[subjectIndex];
	}
	else
	{
		for (tpos = 0; !(subjects[tpos].existst == 1 && strcmp(subjects[tpos].name, sub) == 0) && tpos < ObjectSize; tpos++) {}
		psub = &subjects[tpos];
	}
	if (psub->existst == 0)
	{
		errcode = ERR_LACKPARAMETER;
		return false;
	}
	if (obj == NULL)
	{
		obj = &objects[objectIndex];
	}
	if (obj->existst == 0)
	{
		errcode = ERR_LACKPARAMETER;
		return false;
	}
	for (tpos = 0; !((obj->subjectsexistst[tpos]) == 1 && (obj->subjects[tpos]) == psub) && tpos < SubjectSize; tpos++) {}
	if (tpos < SubjectSize)
	{
		float tsc = obj->subjectscores[tpos];
		if (tsc == -1.f)
		{
			tsc = 0;
		}
		obj->subjectscores[tpos] = scr;
		obj->score += (scr - tsc);
		long tpos2 = 0;
		long tposition = 0;
		averagescr = allobjectscr / size;
		rmsscr = 0;
		for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
		{
			if (objects[tpos2].existst == 1)
			{
				rmsscr += pow((objects[tpos2].score - averagescr), 2);
			}
		}
		rmsscr = pow((rmsscr / size), 0.5);
		if (size == 1)
		{
			lastobjectscr = obj->score;
			objectIndex = 0;
		}
		if (firstobjectscr <= obj->score)
		{
			firstobjectscr = obj->score;
			obj->rank = 1;
		}
		else
		{
			if (lastobjectscr >= obj->score)
			{
				lastobjectscr = obj->score;
				obj->rank = size;
				if (lastobjectscr == obj->score)
				{
					for (tpos2 = 0; (objects[tpos2].existst != 1 || lastobjectscr != objects[tpos2].score) && tpos2 < ObjectSize; tpos2++) {}
					if (tpos2 < ObjectSize)
					{
						obj->rank = objects[tpos2].rank;
					}
				}
			}
			else
			{
				for (tpos2 = 0; (objects[tpos2].rank != 1 || objects[tpos2].existst != 1) && tpos2 < ObjectSize; tpos2++) {}
				tposition = 1;
				for (; objects[tpos2].score > obj->score; tposition++)
				{
				retry1:
					for (tpos2 = 0; (objects[tpos2].rank != (tposition + 1) || objects[tpos2].existst != 1) && tpos2 < ObjectSize; tpos2++) {}
					if (tpos2 == ObjectSize)
					{
						tposition++;
						goto retry1;
					}
				}
				obj->rank = tposition;
			}
		}
		for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
		{
			if (obj->score > objects[tpos2].score && objects[tpos2].existst == 1)
			{
				objects[tpos2].rank++;
			}
		}
		long tpos3;
		long tpos4 = 0;
		long tpos5 = 0;
		obj->subjects[tpos]->allobjectscr += (scr - tsc);
		obj->subjects[tpos]->averagescr = (obj->subjects[tpos]->allobjectscr) / (obj->subjects[tpos]->size);
		if (obj->subjectscores[tpos] > obj->subjects[tpos]->firstobjectscr)
		{
			obj->subjects[tpos]->firstobjectscr = obj->subjectscores[tpos];
		}
		if (obj->subjectscores[tpos] < obj->subjects[tpos]->lastobjectscr)
		{
			obj->subjects[tpos]->lastobjectscr = obj->subjectscores[tpos];
		}
		obj->subjects[tpos]->fullpct = 0;
		obj->subjects[tpos]->exellentpct = 0;
		obj->subjects[tpos]->passpct = 0;
		obj->subjects[tpos]->badpct = 0;
		obj->subjects[tpos]->rmsscr = 0;
		for (tpos4 = 0; tpos4 < ObjectSize; tpos4++)
		{
			if (objects[tpos4].existst == 1 && objects[tpos4].IsInSubject(obj->subjects[tpos], ByObj, tpos5) == true)
			{
				if (objects[tpos4].subjectscores[tpos5] == obj->subjects[tpos]->fullscr)
				{
					obj->subjects[tpos]->fullpct += 100;
				}
				if (objects[tpos4].subjectscores[tpos5] >= obj->subjects[tpos]->exellentscr)
				{
					obj->subjects[tpos]->exellentpct += 100;
				}
				if (objects[tpos4].subjectscores[tpos5] >= obj->subjects[tpos]->passscr)
				{
					obj->subjects[tpos]->passpct += 100;
				}
				if (objects[tpos4].subjectscores[tpos5] <= obj->subjects[tpos]->badscr)
				{
					obj->subjects[tpos]->badpct += 100;
				}
				obj->subjects[tpos]->rmsscr += pow(((objects[tpos4].subjectscores[tpos5]) - (obj->subjects[tpos]->averagescr)), 2);
			}
		}
		obj->subjects[tpos]->fullpct = obj->subjects[tpos]->fullpct / obj->subjects[tpos]->size;
		obj->subjects[tpos]->exellentpct = obj->subjects[tpos]->exellentpct / obj->subjects[tpos]->size;
		obj->subjects[tpos]->passpct = obj->subjects[tpos]->passpct / obj->subjects[tpos]->size;
		obj->subjects[tpos]->badpct = obj->subjects[tpos]->badpct / obj->subjects[tpos]->size;
		obj->subjects[tpos]->rmsscr = pow((obj->subjects[tpos]->rmsscr / obj->subjects[tpos]->size), 0.5);
		for (tpos3 = 0; tpos3 < GroupSize; tpos3++)
		{
			if (obj->groupsexistst[tpos3] == 1)
			{
				obj->groups[tpos3]->allobjectscr += (scr - tsc);
				obj->groups[tpos3]->averagescr = (obj->groups[tpos3]->allobjectscr) / (obj->groups[tpos3]->size);
				if (obj->score > obj->groups[tpos3]->firstobjectscr)
				{
					obj->groups[tpos3]->firstobjectscr = obj->score;
				}
				if (obj->score < obj->groups[tpos3]->lastobjectscr)
				{
					obj->groups[tpos3]->lastobjectscr = obj->score;
				}
				obj->groups[tpos3]->exellentpct = 0;
				obj->groups[tpos3]->passpct = 0;
				obj->groups[tpos3]->badpct = 0;
				obj->groups[tpos3]->rmsscr = 0;
				for (tpos4 = 0; tpos4 < ObjectSize; tpos4++)
				{
					if (objects[tpos4].existst == 1 && objects[tpos4].IsInGroup(obj->groups[tpos3], ByObj) == true)
					{
						if (objects[tpos4].score >= exellentscr)
						{
							obj->groups[tpos3]->exellentpct += 100;
						}
						if (objects[tpos4].score >= passscr)
						{
							obj->groups[tpos3]->passpct += 100;
						}
						if (objects[tpos4].score <= badscr)
						{
							obj->groups[tpos3]->badpct += 100;
						}
						obj->groups[tpos3]->rmsscr += pow(((objects[tpos4].score) - (obj->groups[tpos3]->averagescr)), 2);
					}
				}
				obj->groups[tpos3]->exellentpct = obj->groups[tpos3]->exellentpct / obj->groups[tpos3]->size;
				obj->groups[tpos3]->passpct = obj->groups[tpos3]->passpct / obj->groups[tpos3]->size;
				obj->groups[tpos3]->badpct = obj->groups[tpos3]->badpct / obj->groups[tpos3]->size;
				obj->groups[tpos3]->rmsscr = pow((obj->groups[tpos3]->rmsscr / obj->groups[tpos3]->size), 0.5);
			}
		}
	}
	else
	{
		errcode = Error_NoSuchItem;
		return false;
	}
	return true;
}

bool StatisticsGlobal::changeobjscr(float scr, StatisticsObject* obj)
{
	if (scr < 0)
	{
		errcode = ERR_LACKPARAMETER;
		return false;
	}
	if (obj == NULL)
	{
		obj = &objects[objectIndex];
	}
	if (obj->existst == 0)
	{
		errcode = ERR_LACKPARAMETER;
		return false;
	}
	obj->score = scr;
	return true;
}

bool StatisticsGlobal::GetMessage(char**& msg_string, MSGSTRUCT*& msg_array, long& msgcnt, short type)
{
	if (type - 3 > 0)
	{
		errcode = Error_NoSuchItem;
		return false;
	}
	msg_string = NULL;
	msg_array = NULL;
	msgcnt = 0;
	long tpos = 0;
	for (tpos = 0; tpos < MSGSIZE; tpos++)
	{
		if (msg_stc[tpos].msgflag > 0)
		{
			if (type != 2)
			{
				if (msgcnt < 101)
				{
					switch (msg_stc[tpos].msgflag)
					{
					case SETUNCERTAIN_FULLMARK:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Uncertain scores were replaced with full mark.");
						break;
					case SETUNCERTAIN_ZERO:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Uncertain scores were replaced with 0.");
						break;
					case SETUNCERTAIN_CUSTOMIZE:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Uncertain scores were replaced.");
						break;
					case Error_FullMemory:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:Memory full. Unable to add new item.");
						break;
					case Error_EmptyMemory:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:Memory Empty. Unable to delete item.");
						break;
					case Error_NoSuchItem:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:The chosen item does not exist.");
						break;
					case Error_NoSuchItem_RankInc:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:No such object with the input rank.The rank increased.");
						break;
					case Error_NoSuchItem_RankDec:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:No such object with the input rank.The rank decreased.");
						break;
					case ERR_UNCERTAINSCORE:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:An uncertain score was found when processing.");
						break;
					case ERR_LACKPARAMETER:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:Function was called with invalid parameters.");
						break;
					case Error_ItemExists:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:Item already exists.Unable to add it again.");
						break;
					default:
						errcode = Error_NoSuchItem;
						return false;
					}
				}
				else
				{
					strcpy(msg_str[100], "");
					strcpy(msg_str[100], "Message Array Full.");
				}
			}
			msgcnt++;
		}
	}
	if (type != 2)
	{
		msg_string = msg_str;
	}
	if (type != 1)
	{
		msg_array = msg_stc;
	}
	return true;
}

void StatisticsGlobal::ClearMessage()
{
	long tpos = 0;
	for (tpos = 0; tpos < MSGSIZE; tpos++)
	{
		msg_stc[tpos].msgflag = 0;
	}
}

bool StatisticsGlobal::SetSubjectIndex(long index, bool forced = false)
{
	if ((forced == false && index < SubjectSize && index >= 0) || forced = true)
	{
		subjectIndex = index;
		if ((forced == false && subjects[index].existst == 1) || forced == true)
		{
			return true;
		}
	}
	errcode = Error_NoSuchItem;
	return false;
}

void StatisticsGlobal::GetSubjectIndex(long& index)
{
	index = subjectIndex;
}

void StatisticsGlobal::NextSubject(char*& snm, short stp)
{
	long tpos;
	if (stp != AllSubjects)
	{
		for (tpos = subjectIndex + 1; (subjects[tpos].existst != 1 || subjects[tpos].type != stp) && tpos < SubjectSize; tpos++)
		{
		}
	}
	else
	{
		for (tpos = subjectIndex + 1; subjects[tpos].existst != 1 && tpos < SubjectSize; tpos++)
		{
		}
	}
	if (tpos == SubjectSize)
	{
		strcpy(snm, "NULLSUBJECT");
		return;
	}
	else
	{
		strcpy(snm, subjects[tpos].name);
		subjectIndex = tpos;
	}
}

bool StatisticsGlobal::SearchSubject(void* si, short type)
{
	long tpos = 0;
	switch (type)
	{
	case ByName:
		for (; (strcmp(subjects[tpos].name, (const char*)si) != 0 || subjects[tpos].existst != 1) && tpos < SubjectSize; tpos++) {}
		break;
	case ByObj:
		for (; (&subjects[tpos] != (StatisticsSubject*)si || subjects[tpos].existst != 1) && tpos < SubjectSize; tpos++) {}
		break;
	default:
		break;
	}
	if (tpos == SubjectSize)
	{
		errcode = Error_NoSuchItem;
		return false;
	}
	subjectIndex = tpos;
	return true;
}

bool StatisticsGlobal::DeleteGroup(char*& gnm, short& gtp)
{
	long tpos;
	long tpos2;
	long tpos3;
	for (tpos = 0; groups[tpos].existst != 1 && tpos < GroupSize; tpos++) {}
	if (tpos == GroupSize)
	{
		errcode = Error_EmptyMemory;
		return false;
	}
	tpos = groupIndex;
	strcpy(gnm, groups[tpos].name);
	gtp = groups[tpos].type;
	groups[tpos].existst = 0;
	for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
	{
		if (objects[tpos2].existst == 1)
		{
			for (tpos3 = 0; tpos3 < GroupSize; tpos3++)
			{
				if (objects[tpos2].groupsexistst[tpos3] == 1 && objects[tpos2].groups[tpos3] == &groups[tpos])
				{
					objects[tpos2].groups[tpos3] = NULL;
					objects[tpos2].groupsexistst[tpos3] = 0;
				}
			}
		}
	}
	for (tpos2 = 0; groups[tpos2].existst != 1 && tpos2 < GroupSize; tpos2++) {}
	if (tpos2 == GroupSize)
	{
		groupIndex = 0;
	}
	else
	{
		groupIndex = tpos2;
	}
	return true;
}

bool StatisticsGlobal::DeleteSubject(char*& snm, short& stp, long& maxsz)
{
	long tpos;
	long tpos2;
	long tpos3;
	for (tpos = 0; subjects[tpos].existst != 1 && tpos < SubjectSize; tpos++) {}
	if (tpos == SubjectSize)
	{
		errcode = Error_EmptyMemory;
		return false;
	}
	tpos = subjectIndex;
	snm = subjects[tpos].name;
	maxsz = subjects[tpos].maxsize;
	stp = subjects[tpos].type;
	subjects[tpos].existst = 0;
	for (tpos2 = 0; tpos2 < ObjectSize; tpos2++)
	{
		if (objects[tpos2].existst == 1)
		{
			for (tpos3 = 0; tpos3 < SubjectSize; tpos3++)
			{
				if (objects[tpos2].subjectsexistst[tpos3] == 1 && objects[tpos2].subjects[tpos3] == &subjects[tpos])
				{
					objects[tpos2].subjects[tpos3] = NULL;
					objects[tpos2].subjectsexistst[tpos3] = 0;
					if (objects[tpos2].subjectscores[tpos3] > 0)
					{
						objects[tpos2].score -= objects[tpos2].subjectscores[tpos3];
					}
				}
			}
		}
	}
	for (tpos2 = 0; subjects[tpos2].existst != 1 && tpos2 < SubjectSize; tpos2++) {}
	if (tpos2 == SubjectSize)
	{
		subjectIndex = 0;
	}
	else
	{
		subjectIndex = tpos2;
	}
	return true;
}
