// Statisticsinfos.cpp: implementation of the Statisticsinfos class.
//
//////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "math.h"
#include "Shared.h"
#include "Statisticsgroup.h"
#include "Statisticssubject.h"
#include "Statisticsinfo.h"
#include "Statisticsinfos.h"
#include <string.h>

#pragma warning(disable : 4996)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Statisticsinfos::Statisticsinfos()
{
	objects = new Statisticsinfo[OBJECTSIZE];
	groups = new Statisticsgroup[GROUPSIZE];
	subjects = new Statisticssubject[SUBJECTSIZE];
	for (int i = 0; i < OBJECTSIZE; i++)
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
	objectposition = 0;
	groupposition = 0;
	subjectposition = 0;
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

Statisticsinfos::~Statisticsinfos()
{
	delete[]objects;
	delete[]groups;
	delete[]subjects;
	for (int i = 0; i < MSGSIZE; i++)
	{
		delete[]msg_str[i];
	}
}

void Statisticsinfos::setscrline(float esc, float bsc, float psc)
{
	exellentscr = esc;
	badscr = bsc;
	passscr = psc;
}

bool Statisticsinfos::searchobject(void* si, short type, short stype = SEARCH_SINGLE)
{
	long tpos = 0;
	switch (type)
	{
	case 1:
		if (stype != SEARCH_FUZZY)
		{
			for (; (strcmp(objects[tpos].name, (const char*)si) != 0 || objects[tpos].existst != 1) && tpos < OBJECTSIZE; tpos++) {}
		}
		else
		{
			for (; tpos < OBJECTSIZE; tpos++)
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
		if (stype != SEARCH_FUZZY)
		{
			for (; (strcmp(objects[tpos].number, (const char*)si) != 0 || objects[tpos].existst != 1) && tpos < OBJECTSIZE; tpos++) {}
		}
		else
		{
			for (; tpos < OBJECTSIZE; tpos++)
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
		if (stype == SEARCH_SINGLE)
		{
			long tpos2 = 0;
			SUB_SCR* tsi;
			tsi = (SUB_SCR*)si;
			if (tsi->sub == NULL)
			{
				for (; (objects[tpos].score != tsi->scr || objects[tpos].existst != 1) && tpos < OBJECTSIZE; tpos++) {}
			}
			else
			{
				for (; tpos < OBJECTSIZE; tpos++)
				{
					if (objects[tpos].existst == 1)
					{
						for (tpos2 = 0; (objects[tpos].subjects[tpos2] != tsi->sub || objects[tpos].subjectsexistst[tpos2] != 1 || objects[tpos].subjectscores[tpos2] != tsi->scr) && tpos2 < SUBJECTSIZE; tpos2++) {}
						if (tpos2 < SUBJECTSIZE)
						{
							objectposition = tpos;
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
				for (; tpos < OBJECTSIZE; tpos++)
				{
					if (objects[tpos].score == tsi->scr && objects[tpos].existst == 1)
					{
						objects[tpos].marked = 1;
					}
				}
			}
			else
			{
				for (; tpos < OBJECTSIZE; tpos++)
				{
					if (objects[tpos].existst == 1 && objects[tpos].IsInSubject(tsi->sub, BYOBJ, tpos2) == true && objects[tpos].subjectscores[tpos2] == tsi->scr)
					{
						objects[tpos].marked = 1;
					}
				}
			}
			return true;
		}
		break;
	case 4:
		if (stype == SEARCH_SINGLE)
		{
			for (; (objects[tpos].rank != *((long*)si) || objects[tpos].existst != 1) && tpos < OBJECTSIZE; tpos++) {}
		}
		else
		{
			for (; tpos < OBJECTSIZE; tpos++)
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
	if (tpos == OBJECTSIZE)
	{
		errcode = ERR_NOSUCHITEM;
		return false;
	}
	objectposition = tpos;
	return true;
}

void Statisticsinfos::getall(long& sz, float& avg, float& gap, float& rms)
{
	sz = size;
	avg = averagescr;
	gap = firstobjectscr - lastobjectscr;
	rms = rmsscr;
}

bool Statisticsinfos::inobject(char* onm, char* onb, short osx, Statisticsinfo** reto)
{
	if (size == OBJECTSIZE)
	{
		errcode = ERR_NOMEMORY;;
		return false;
	}
	long tpos;
	long tpos2 = -1;
	bool retv = true;
	for (tpos = 0; tpos < OBJECTSIZE; tpos++)
	{
		if (objects[tpos].existst != 1 && tpos2 < 0)
		{
			tpos2 = tpos;
		}
		if (objects[tpos].existst == 1)
		{
			if (strcmp(objects[tpos].name, onm) == 0 || strcmp(objects[tpos].number, onb) == 0)
			{
				errcode = ERR_ITEMALREADYEXISTS;;
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
	for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
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
		objectposition = 0;
		objects[tpos].rank = 1;
	}
	else
	{
		objects[tpos].rank = size;
		for (tpos2 = 0; (objects[tpos2].existst != 1 || lastobjectscr != objects[tpos2].score) && tpos2 < OBJECTSIZE; tpos2++) {}
		if (tpos2 < OBJECTSIZE)
		{
			objects[tpos].rank = objects[tpos2].rank;
		}
	}

	tpos2 = 0;
	long tpos3 = 0;
	long tpos4 = 0;
	long tpos5 = 0;
	for (tpos2 = 0; tpos2 < GROUPSIZE; tpos2++)
	{
		if (groups[tpos2].existst == 1 && groups[tpos2].type == NOTCHOOSABLEGROUPS)
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
			for (tpos4 = 0; tpos4 < OBJECTSIZE; tpos4++)
			{
				if (objects[tpos4].existst == 1 && objects[tpos4].IsInGroup(objects[tpos].groups[tpos3], BYOBJ) == true)
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
	for (tpos2 = 0; tpos2 < SUBJECTSIZE; tpos2++)
	{
		if (subjects[tpos2].existst == 1 && subjects[tpos2].type == NOTCHOOSABLESUBJECTS)
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
			for (tpos4 = 0; tpos4 < OBJECTSIZE; tpos4++)
			{
				if (objects[tpos4].existst == 1 && objects[tpos4].IsInSubject(objects[tpos].subjects[tpos3], BYOBJ, tpos5) == true)
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

bool Statisticsinfos::outobject(char*& onm, char*& onb, short& osx)
{
	if (size == 0)
	{
		errcode = ERR_EMPTYMEMORY;
		return false;
	}
	long tpos;
	long tpos2;
	long plus;
	tpos = objectposition;
	strcpy(onm, objects[tpos].name);
	strcpy(onb, objects[tpos].number);
	osx = objects[tpos].sex;
	objects[tpos].existst = 0;
	size--;
	if (size == 0)
	{
		objectposition = 0;
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
	for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
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
		for (tpos2 = 0; (objects[tpos2].rank != plus || objects[tpos2].existst != 1) && tpos2 < OBJECTSIZE; tpos2++) {}
		if (tpos2 == OBJECTSIZE)
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
		for (tpos2 = 0; (objects[tpos2].rank != plus || objects[tpos2].existst != 1) && tpos2 < OBJECTSIZE; tpos2++) {}
		if (tpos2 == OBJECTSIZE)
		{
			plus--;
			goto retry5;
		}
		lastobjectscr = objects[tpos2].score;
	}
	for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
	{
		if (objects[tpos].score > objects[tpos2].score && objects[tpos2].existst == 1)
		{
			objects[tpos2].rank--;
		}
	}

	long tpos3;
	long tpos4 = 0;
	long tpos5 = 0;
	for (tpos3 = 0; tpos3 < GROUPSIZE; tpos3++)
	{
		if (objects[tpos].groupsexistst[tpos3] == 1)
		{
			objects[tpos].groups[tpos3]->size--;
			objects[tpos].groups[tpos3]->allobjectscr -= objects[tpos].score;
			objects[tpos].groups[tpos3]->averagescr = (objects[tpos].groups[tpos3]->allobjectscr) / (objects[tpos].groups[tpos3]->size);
			if (objects[tpos].score == objects[tpos].groups[tpos3]->firstobjectscr)
			{
				objects[tpos].groups[tpos3]->firstobjectscr = 0;
				for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
				{
					if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(objects[tpos].groups[tpos3], BYOBJ) == true && objects[tpos2].score > objects[tpos].groups[tpos3]->firstobjectscr)
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
					for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
					{
						if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(objects[tpos].groups[tpos3], BYOBJ) == true && objects[tpos2].score < objects[tpos].groups[tpos3]->lastobjectscr)
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
			for (tpos4 = 0; tpos4 < OBJECTSIZE; tpos4++)
			{
				if (objects[tpos4].existst == 1 && objects[tpos4].IsInGroup(objects[tpos].groups[tpos3], BYOBJ) == true)
				{
					objects[tpos].groups[tpos3]->rmsscr += pow(((objects[tpos4].score) - (objects[tpos].groups[tpos3]->averagescr)), 2);
				}
			}
			objects[tpos].groups[tpos3]->rmsscr = pow((objects[tpos].groups[tpos3]->rmsscr / objects[tpos].groups[tpos3]->size), 0.5);
		}
	}
	tpos4 = 0;
	for (tpos3 = 0; tpos3 < SUBJECTSIZE; tpos3++)
	{
		if (objects[tpos].subjectsexistst[tpos3] == 1)
		{
			objects[tpos].subjects[tpos3]->size--;
			objects[tpos].subjects[tpos3]->allobjectscr -= objects[tpos].subjectscores[tpos3];
			objects[tpos].subjects[tpos3]->averagescr = (objects[tpos].subjects[tpos3]->allobjectscr) / (objects[tpos].subjects[tpos3]->size);
			if (objects[tpos].subjectscores[tpos3] == objects[tpos].subjects[tpos3]->firstobjectscr)
			{
				objects[tpos].subjects[tpos3]->firstobjectscr = 0;
				for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
				{
					if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(objects[tpos].subjects[tpos3], BYOBJ, tpos5) == true && objects[tpos2].subjectscores[tpos5] > objects[tpos].subjects[tpos3]->firstobjectscr)
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
					for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
					{
						if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(objects[tpos].subjects[tpos3], BYOBJ, tpos5) == true && objects[tpos2].subjectscores[tpos5] < objects[tpos].subjects[tpos3]->lastobjectscr)
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
			for (tpos4 = 0; tpos4 < OBJECTSIZE; tpos4++)
			{
				if (objects[tpos4].existst == 1 && objects[tpos4].IsInSubject(objects[tpos].subjects[tpos3], BYOBJ, tpos5) == true)
				{
					objects[tpos].subjects[tpos3]->rmsscr += pow(((objects[tpos4].subjectscores[tpos5]) - (objects[tpos].subjects[tpos3]->averagescr)), 2);
				}
			}
			objects[tpos].subjects[tpos3]->rmsscr = pow((objects[tpos].subjects[tpos3]->rmsscr / objects[tpos].subjects[tpos3]->size), 0.5);
		}
	}
	return true;
}

void Statisticsinfos::getobject(char*& tnm, char*& tnb, short& tsx, float& tsc, long& tps)
{
	strcpy(tnm, objects[objectposition].name);
	strcpy(tnb, objects[objectposition].number);
	tsx = objects[objectposition].sex;
	tsc = objects[objectposition].score;
	tps = objects[objectposition].rank;
}

bool Statisticsinfos::nextobject(long times)
{
	if (times == 0)
		return true;
	long tpos = objects[objectposition].rank + times;
	if (tpos > size)
	{
		errcode = ERR_NOSUCHITEM;
		return false;
	}
	long tpos2;
retry2:
	for (tpos2 = 0; ((tpos != objects[tpos2].rank || objects[tpos2].existst != 1) && tpos2 < OBJECTSIZE) || tpos2 == objectposition; tpos2++)
	{
	}
	if (tpos2 == OBJECTSIZE)
	{
		tpos++;
		if (tpos > size)
		{
			errcode = ERR_NOSUCHITEM;
			return false;
		}
		goto retry2;
	}
	if ((objects[objectposition].rank + times) != tpos)
	{
		errcode = ERR_NOSUCHITEM_POSINC;
		objectposition = tpos2;
		return false;
	}
	objectposition = tpos2;
	return true;
}

bool Statisticsinfos::lastobject(long times)
{
	if (times == 0)
		return true;
	long tpos = objects[objectposition].rank - times;
	if (tpos <= 0)
	{
		errcode = ERR_NOSUCHITEM;
		return false;
	}
	long tpos2;
retry3:
	for (tpos2 = 0; ((tpos != objects[tpos2].rank || objects[tpos2].existst != 1) && tpos2 < OBJECTSIZE) || tpos2 == objectposition; tpos2++)
	{
	}
	if (tpos2 == OBJECTSIZE)
	{
		tpos--;
		if (tpos < 1)
		{
			errcode = ERR_NOSUCHITEM;
			return false;
		}
		goto retry3;
	}
	if ((objects[objectposition].rank - times) != tpos)
	{
		errcode = ERR_NOSUCHITEM_POSDEC;
		objectposition = tpos2;
		return false;
	}
	objectposition = tpos2;
	return true;
}

void Statisticsinfos::getscrline(Statisticssubject* sub, float& esc, float& bsc, float& psc, float& fsc)
{
	if (sub == NULL)
	{
		fsc = subjects[subjectposition].fullscr;
		esc = subjects[subjectposition].exellentscr;
		bsc = subjects[subjectposition].badscr;
		psc = subjects[subjectposition].passscr;
	}
	else
	{
		fsc = sub->fullscr;
		esc = sub->exellentscr;
		bsc = sub->badscr;
		psc = sub->passscr;
	}
}

bool Statisticsinfos::gotoobject(unsigned long pos, short errpolicy = ERRPOLICY_POSINC)
{
	if (pos > OBJECTSIZE)
	{
		errcode = ERR_NOSUCHITEM;
		return false;
	}
	long tpos;
	long tpos2 = pos;
retry6:
	for (tpos = 0; (objects[tpos].rank != pos || objects[tpos].existst != 1) && tpos < OBJECTSIZE; tpos++) {}
	if (tpos == OBJECTSIZE)
	{
		if (errpolicy == ERRPOLICY_POSINC)
			pos++;
		else
			pos--;
		if (pos > OBJECTSIZE || pos <= 0)
		{
			errcode = ERR_NOSUCHITEM;
			return false;
		}
		goto retry6;
	}
	if (tpos2 != pos)
	{
		errcode = errpolicy + 3;
		objectposition = tpos;
		return false;
	}
	objectposition = tpos;
	return true;
}

void Statisticsinfos::setscrline(Statisticssubject* sub, float esc, float bsc, float psc, float fsc)
{
	if (sub == NULL)
	{
		subjects[subjectposition].fullscr = fsc;
		subjects[subjectposition].exellentscr = esc;
		subjects[subjectposition].badscr = bsc;
		subjects[subjectposition].passscr = psc;
	}
	else
	{
		sub->fullscr = fsc;
		sub->exellentscr = esc;
		sub->badscr = bsc;
		sub->passscr = psc;
	}
}

void Statisticsinfos::getpercents(float& epct, float& tpct, float& ppct)
{
	long tpos;
	exellentpct = 0;
	for (tpos = 0; tpos < OBJECTSIZE; tpos++)
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
	for (tpos = 0; tpos < OBJECTSIZE; tpos++)
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
	for (tpos = 0; tpos < OBJECTSIZE; tpos++)
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

void Statisticsinfos::nextobject(char*& onm, char*& onb, short& osx, float& osc, long& ops)
{
	long tpos;
	for (tpos = objectposition + 1; objects[tpos].existst != 1 && tpos < OBJECTSIZE; tpos++)
	{
	}
	if (tpos == OBJECTSIZE)
	{
		strcpy(onm, "NULLOBJECT");
		return;
	}
	else
	{
		strcpy(onm, objects[tpos].name);
		onb = objects[tpos].number;
		osx = objects[tpos].sex;
		osc = objects[tpos].score;
		ops = objects[tpos].rank;
		objectposition = tpos;
	}
}

void Statisticsinfos::gotoobject(long& pos, bool status)
{
	if (pos < OBJECTSIZE)
	{
		if (status == true)
		{
			pos = objectposition;
		}
		else
		{
			objectposition = pos;
		}
	}
}

bool Statisticsinfos::ingroup(char* gnm, short gtp)
{
	long tpos;
	long tpos2 = -1;
	long tpos3;
	for (tpos = 0; tpos < GROUPSIZE; tpos++)
	{
		if (groups[tpos].existst != 1 && tpos2 < 0)
		{
			tpos2 = tpos;
		}
		if (groups[tpos].existst == 1)
		{
			if (strcmp(groups[tpos].name, gnm) == 0)
			{
				errcode = ERR_ITEMALREADYEXISTS;;
				return false;
			}
		}
	}
	tpos = tpos2;
	if (tpos == GROUPSIZE)
	{
		errcode = ERR_NOMEMORY;
		return false;
	}
	strcpy(groups[tpos].name, gnm);
	groups[tpos].type = gtp;
	groups[tpos].existst = 1;
	if (gtp == 1)
	{
		for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
		{
			if (objects[tpos2].existst == 1)
			{
				for (tpos3 = 0; objects[tpos2].groupsexistst[tpos3] == 1 && tpos3 < GROUPSIZE; tpos3++)
				{
				}
				if (tpos3 < GROUPSIZE)
				{
					objects[tpos2].groups[tpos3] = groups + tpos;
					objects[tpos2].groupsexistst[tpos3] = 1;
				}
				else
				{
					errcode = ERR_NOMEMORY;
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
	for (tpos2 = 0; (groups[tpos2].existst != 1 || &groups[tpos2] == &groups[tpos]) && tpos2 < GROUPSIZE; tpos2++) {}
	if (tpos == GROUPSIZE)
	{
		groupposition = tpos;
	}
	return true;
}

void Statisticsinfos::getgroup(char*& gnm, short& gtp, long& gsz, float& gavg, float& ggap, float& grms)
{
	strcpy(gnm, groups[groupposition].name);
	gtp = groups[groupposition].type;
	gsz = groups[groupposition].size;
	gavg = groups[groupposition].averagescr;
	ggap = groups[groupposition].firstobjectscr - groups[groupposition].lastobjectscr;
	grms = groups[groupposition].rmsscr;
}

bool Statisticsinfos::searchgroup(void* si, short type)
{
	long tpos = 0;
	switch (type)
	{
	case BYNAME:
		for (; (strcmp(groups[tpos].name, (const char*)si) != 0 || groups[tpos].existst != 1) && tpos < GROUPSIZE; tpos++) {}
		break;
	case BYOBJ:
		for (; (&groups[tpos] != (Statisticsgroup*)si || groups[tpos].existst != 1) && tpos < GROUPSIZE; tpos++) {}
		break;
	default:
		break;
	}
	if (tpos == GROUPSIZE)
	{
		errcode = ERR_NOSUCHITEM;
		return false;
	}
	groupposition = tpos;
	return true;
}

bool Statisticsinfos::addtogroup(long gpos, short gtp)
{
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	for (; tpos2 < gpos && tpos < GROUPSIZE; tpos++)
	{
		if (groups[tpos].existst == 1 && groups[tpos].type == gtp)
		{
			tpos2++;
		}
	}
	tpos2 = -1;
	for (tpos3 = 0; tpos3 < GROUPSIZE; tpos3++)
	{
		if (objects[objectposition].groupsexistst[tpos3] != 1 && tpos2 < 0)
		{
			tpos2 = tpos3;
		}
		if (objects[objectposition].groupsexistst[tpos3] == 1)
		{
			if (strcmp(groups[tpos].name, objects[objectposition].groups[tpos3]->name) == 0)
			{
				errcode = ERR_ITEMALREADYEXISTS;;
				return false;
			}
		}
	}
	tpos3 = tpos2;
	if (tpos3 < 0)
	{
		errcode = ERR_NOMEMORY;
		return false;
	}
	objects[objectposition].groups[tpos3] = &groups[tpos];
	objects[objectposition].groupsexistst[tpos3] = 1;
	groups[tpos].size++;
	groups[tpos].allobjectscr += objects[objectposition].score;
	groups[tpos].averagescr = (groups[tpos].allobjectscr) / (groups[tpos].size);
	if (objects[objectposition].score > groups[tpos].firstobjectscr)
	{
		groups[tpos].firstobjectscr = objects[objectposition].score;
	}
	if (objects[objectposition].score < groups[tpos].lastobjectscr || groups[tpos].size == 1)
	{
		groups[tpos].lastobjectscr = objects[objectposition].score;
	}
	groups[tpos].rmsscr = 0;
	for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
	{
		if (objects[tpos2].existst == 1)
		{
			for (tpos3 = 0; ((objects[tpos2].groups[tpos3]) != groups + tpos || (objects[tpos2].groupsexistst[tpos3]) != 1) && tpos3 < GROUPSIZE; tpos3++) {}
			if (tpos3 < GROUPSIZE)
			{
				groups[tpos].rmsscr += pow(((objects[tpos2].score) - (groups[tpos].averagescr)), 2);
			}
		}
	}
	groups[tpos].rmsscr = pow((groups[tpos].rmsscr / groups[tpos].size), 0.5);
	return true;
}

bool Statisticsinfos::addtogroup(const char* gnm, long opos)
{
	if (opos == -1)
	{
		opos = objectposition;
	}
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	for (; tpos < GROUPSIZE && !(groups[tpos].existst == 1 && strcmp(groups[tpos].name, gnm) == 0); tpos++)
	{
	}
	tpos2 = -1;
	for (tpos3 = 0; tpos3 < GROUPSIZE; tpos3++)
	{
		if (objects[opos].groupsexistst[tpos3] != 1 && tpos2 < 0)
		{
			tpos2 = tpos3;
		}
		if (objects[opos].groupsexistst[tpos3] == 1)
		{
			if (strcmp(groups[tpos].name, objects[opos].groups[tpos3]->name) == 0)
			{
				errcode = ERR_ITEMALREADYEXISTS;;
				return false;
			}
		}
	}
	tpos3 = tpos2;
	if (tpos3 < 0)
	{
		errcode = ERR_NOMEMORY;
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
	for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
	{
		if (objects[tpos2].existst == 1)
		{
			for (tpos3 = 0; ((objects[tpos2].groups[tpos3]) != groups + tpos || (objects[tpos2].groupsexistst[tpos3]) != 1) && tpos3 < GROUPSIZE; tpos3++) {}
			if (tpos3 < GROUPSIZE)
			{
				groups[tpos].rmsscr += pow(((objects[tpos2].score) - (groups[tpos].averagescr)), 2);
			}
		}
	}
	groups[tpos].rmsscr = pow((groups[tpos].rmsscr / groups[tpos].size), 0.5);
	return true;
}

void Statisticsinfos::nextgroup(char*& gnm, short gtp)
{
	long tpos;
	if (gtp != ALLGROUPS)
	{
		for (tpos = groupposition + 1; (groups[tpos].existst != 1 || groups[tpos].type != gtp) && tpos < GROUPSIZE; tpos++)
		{
		}
	}
	else
	{
		for (tpos = groupposition + 1; groups[tpos].existst != 1 && tpos < GROUPSIZE; tpos++)
		{
		}
	}
	if (tpos == GROUPSIZE)
	{
		strcpy(gnm, "NULLGROUP");
		return;
	}
	else
	{
		strcpy(gnm, groups[tpos].name);
		groupposition = tpos;
	}
}

void Statisticsinfos::gotogroup(long& pos, bool status)
{
	if (pos < GROUPSIZE)
	{
		if (status == true)
		{
			pos = groupposition;
		}
		else
		{
			groupposition = pos;
		}
	}
}

void Statisticsinfos::getgrouppercents(float& epct, float& tpct, float& ppct)
{
	long tpos;
	groups[groupposition].exellentpct = 0;
	groups[groupposition].badpct = 0;
	groups[groupposition].passpct = 0;
	for (tpos = 0; tpos < OBJECTSIZE; tpos++)
	{
		if (objects[tpos].existst == 1 && objects[tpos].IsInGroup(&groups[groupposition], BYOBJ) == true && objects[tpos].score >= exellentscr)
		{
			groups[groupposition].exellentpct++;
		}
		if (objects[tpos].existst == 1 && objects[tpos].IsInGroup(&groups[groupposition], BYOBJ) == true && objects[tpos].score <= badscr)
		{
			groups[groupposition].badpct++;
		}
		if (objects[tpos].existst == 1 && objects[tpos].IsInGroup(&groups[groupposition], BYOBJ) == true && objects[tpos].score >= passscr)
		{
			groups[groupposition].passpct++;
		}
	}
	if (groups[groupposition].size > 0)
	{
		groups[groupposition].exellentpct = (groups[groupposition].exellentpct) / (groups[groupposition].size) * 100;
		groups[groupposition].badpct = (groups[groupposition].badpct) / (groups[groupposition].size) * 100;
		groups[groupposition].passpct = (groups[groupposition].passpct) / (groups[groupposition].size) * 100;
	}
	else
	{
		groups[groupposition].exellentpct = 0;
		groups[groupposition].badpct = 0;
		groups[groupposition].passpct = 0;
	}
	epct = groups[groupposition].exellentpct;
	tpct = groups[groupposition].badpct;
	ppct = groups[groupposition].passpct;
}

bool Statisticsinfos::setuncertainscore(short settype, float scr)
{
	if (settype > 3 || settype < 1)
	{
		errcode = ERR_NOSUCHITEM;
		return false;
	}
	if (scr < 0 && settype == 3)
	{
		errcode = ERR_LACKPARAMETER;
		return false;
	}
	long tpos = 0;
	long tpos2 = 0;
	for (tpos = 0; tpos < OBJECTSIZE; tpos++)
	{
		if (objects[tpos].existst == 1)
		{
			for (tpos2 = 0; tpos2 < SUBJECTSIZE; tpos2++)
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
						errcode = ERR_NOSUCHITEM;
						return false;
					}
					break;
				}
			}
		}
	}
	return true;
}

int Statisticsinfos::getlasterror()
{
	int tmperrcode = errcode;
	errcode = 0;
	return tmperrcode;
}

void Statisticsinfos::markuncertain()
{
	long tpos = 0;
	long tpos2 = 0;
	for (tpos = 0; tpos < OBJECTSIZE; tpos++)
	{
		objects[tpos].marked = 0;
		if (objects[tpos].existst == 1)
		{
			for (tpos2 = 0; tpos2 < SUBJECTSIZE; tpos2++)
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

void Statisticsinfos::markall()
{
	long tpos = 0;
	for (tpos = 0; tpos < OBJECTSIZE; tpos++)
	{
		objects[tpos].marked = 1;
	}
}

void Statisticsinfos::unmarkall()
{
	long tpos = 0;
	for (tpos = 0; tpos < OBJECTSIZE; tpos++)
	{
		objects[tpos].marked = 0;
	}
}

long Statisticsinfos::getmarked(long*& m)
{
	long tpos = 0;
	long tpos2 = 0;
	for (tpos = 0; tpos < OBJECTSIZE; tpos++)
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

void Statisticsinfos::inversemark()
{
	long tpos = 0;
	for (tpos = 0; tpos < OBJECTSIZE; tpos++)
	{
		objects[tpos].existst == 1 && objects[tpos].marked == 0 ? objects[tpos].marked = 1 : objects[tpos].marked = 0;
	}
}

bool Statisticsinfos::insubject(char* snm, short stp, long maxsz, float sfm)
{
	long tpos;
	long tpos2 = -1;
	long tpos3;
	for (tpos = 0; tpos < SUBJECTSIZE; tpos++)
	{
		if (subjects[tpos].existst != 1 && tpos2 < 0)
		{
			tpos2 = tpos;
		}
		if (subjects[tpos].existst == 1)
		{
			if (strcmp(subjects[tpos].name, snm) == 0)
			{
				errcode = ERR_ITEMALREADYEXISTS;;
				return false;
			}
		}
	}
	tpos = tpos2;
	if (tpos == SUBJECTSIZE)
	{
		errcode = ERR_NOMEMORY;
		return false;
	}
	strcpy(subjects[tpos].name, snm);
	subjects[tpos].type = stp;
	subjects[tpos].existst = 1;
	if (stp == 1)
	{
		for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
		{
			if (objects[tpos2].existst == 1)
			{
				for (tpos3 = 0; objects[tpos2].subjectsexistst[tpos3] == 1 && tpos3 < SUBJECTSIZE; tpos3++)
				{
				}
				if (tpos3 < SUBJECTSIZE)
				{
					objects[tpos2].subjects[tpos3] = subjects + tpos;
					objects[tpos2].subjectsexistst[tpos3] = 1;
					objects[tpos2].subjectscores[tpos3] = -1.f;
				}
				else
				{
					errcode = ERR_NOMEMORY;
					return false;
				}
			}
		}
		subjects[tpos].size = size;
		subjects[tpos].maxsize = OBJECTSIZE;
	}
	else
	{
		subjects[tpos].size = 0;
		if (maxsz == 0)
		{
			subjects[tpos].maxsize = OBJECTSIZE;
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
	for (tpos2 = 0; (subjects[tpos2].existst != 1 || &subjects[tpos2] == &subjects[tpos]) && tpos2 < SUBJECTSIZE; tpos2++) {}
	if (tpos == SUBJECTSIZE)
	{
		subjectposition = tpos;
	}
	return true;
}

bool Statisticsinfos::addtosubject(long spos, short stp, float sc)
{
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	for (; tpos2 < spos && tpos < SUBJECTSIZE; tpos++)
	{
		if (subjects[tpos].existst == 1 && subjects[tpos].type == stp)
		{
			tpos2++;
		}
	}
	tpos2 = -1;
	for (tpos3 = 0; tpos3 < SUBJECTSIZE; tpos3++)
	{
		if (objects[objectposition].subjectsexistst[tpos3] != 1 && tpos2 < 0)
		{
			tpos2 = tpos3;
		}
		if (objects[objectposition].subjectsexistst[tpos3] == 1)
		{
			if (strcmp(subjects[tpos].name, objects[objectposition].subjects[tpos3]->name) == 0)
			{
				errcode = ERR_ITEMALREADYEXISTS;;
				return false;
			}
		}
	}
	if (tpos2 < 0)
	{
		errcode = ERR_NOMEMORY;
		return false;
	}
	objects[objectposition].subjects[tpos2] = subjects + tpos;
	objects[objectposition].subjectsexistst[tpos2] = 1;
	subjects[tpos].size++;
	if (sc == -1.f)
	{
		objects[objectposition].subjectscores[tpos2] = -1;
	}
	else
	{
		objects[objectposition].subjectscores[tpos2] = 0;
		changesubscr(sc, subjects[tpos].name, objects + objectposition);
	}
	/*objects[objectposition].score += tsc;
	subjects[tpos].allobjectscr += tsc;
	subjects[tpos].averagescr = (subjects[tpos].allobjectscr) / (subjects[tpos].size);
	if (objects[objectposition].score > subjects[tpos].firstobjectscr)
	{
		subjects[tpos].firstobjectscr = objects[objectposition].score;
	}
	if (objects[objectposition].score < subjects[tpos].lastobjectscr || subjects[tpos].size == 1)
	{
		subjects[tpos].lastobjectscr = objects[objectposition].score;
	}
	subjects[tpos].rmsscr = 0;
	for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
	{
		if (objects[tpos2].existst == 1)
		{
			for (tpos3 = 0; ((objects[tpos2].subjects[tpos3]) != subjects + tpos || (objects[tpos2].subjectsexistst[tpos3]) != 1) && tpos3 < SUBJECTSIZE; tpos3++) {}
			if (tpos3 < SUBJECTSIZE)
			{
				subjects[tpos].rmsscr += pow(((objects[tpos2].score) - (subjects[tpos].averagescr)), 2);
			}
		}
	}
	subjects[tpos].rmsscr = pow((subjects[tpos].rmsscr / subjects[tpos].size), 0.5);*/
	return true;
}

void Statisticsinfos::getsubject(char*& snm, short& stp, long& ssz, float& savg, float& sgap, float& srms)
{
	strcpy(snm, subjects[subjectposition].name);
	stp = subjects[subjectposition].type;
	ssz = subjects[subjectposition].size;
	savg = subjects[subjectposition].averagescr;
	sgap = subjects[subjectposition].firstobjectscr - subjects[subjectposition].lastobjectscr;
	srms = subjects[subjectposition].rmsscr;
}

bool Statisticsinfos::addtosubject(const char* snm, long opos)
{
	if (opos == -1)
	{
		opos = objectposition;
	}
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	for (; tpos < SUBJECTSIZE && !(subjects[tpos].existst == 1 && strcmp(subjects[tpos].name, snm) == 0); tpos++)
	{
	}
	tpos2 = -1;
	for (tpos3 = 0; tpos3 < SUBJECTSIZE; tpos3++)
	{
		if (objects[opos].subjectsexistst[tpos3] != 1 && tpos2 < 0)
		{
			tpos2 = tpos3;
		}
		if (objects[opos].subjectsexistst[tpos3] == 1)
		{
			if (strcmp(subjects[tpos].name, objects[opos].subjects[tpos3]->name) == 0)
			{
				errcode = ERR_ITEMALREADYEXISTS;;
				return false;
			}
		}
	}
	if (tpos2 < 0)
	{
		errcode = ERR_NOMEMORY;
		return false;
	}
	objects[opos].subjects[tpos2] = subjects + tpos;
	objects[opos].subjectsexistst[tpos2] = 1;
	objects[opos].subjectscores[tpos2] = -1;
	subjects[tpos].size++;
	return true;
}

bool Statisticsinfos::delfromgroup(long gpos, short gtp)
{
	if (size == 0)
	{
		errcode = ERR_EMPTYMEMORY;
		return false;
	}
	long tpos = 0;
	long tpos2 = 0;
	for (; tpos2 < gpos && tpos < GROUPSIZE; tpos++)
	{
		if (groups[tpos].existst == 1 && groups[tpos].type == gtp)
		{
			tpos2++;
		}
	}
	if (tpos == GROUPSIZE && tpos2 < gpos)
	{
		errcode = ERR_NOSUCHITEM;
		return false;
	}
	groups[tpos].size--;
	groups[tpos].allobjectscr -= objects[objectposition].score;
	groups[tpos].averagescr = (groups[tpos].allobjectscr) / (groups[tpos].size);
	if (objects[objectposition].score == groups[tpos].firstobjectscr)
	{
		groups[tpos].firstobjectscr = 0;
		for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
		{
			if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(&groups[tpos], BYOBJ) == true && objects[tpos2].score > groups[tpos].firstobjectscr)
			{
				groups[tpos].firstobjectscr = objects[tpos2].score;
			}
		}
	}
	if (objects[objectposition].score == groups[tpos].lastobjectscr)
	{
		if (groups[tpos].size == 0)
		{
			groups[tpos].lastobjectscr = 0;
		}
		else
		{
			groups[tpos].lastobjectscr = groups[tpos].firstobjectscr;
			for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
			{
				if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(&groups[tpos], BYOBJ) == true && objects[tpos2].score < groups[tpos].lastobjectscr)
				{
					groups[tpos].lastobjectscr = objects[tpos2].score;
				}
			}
		}
	}
	if (groups[tpos].size != 0)
	{
		if (objects[objectposition].score >= exellentscr)
		{
			groups[tpos].exellentpct = (groups[tpos].exellentpct * (groups[tpos].size + 1) - 100) / (groups[tpos].size);
		}
		if (objects[objectposition].score >= passscr)
		{
			groups[tpos].passpct = (groups[tpos].passpct * (groups[tpos].size + 1) - 100) / (groups[tpos].size);
		}
		if (objects[objectposition].score <= badscr)
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
	for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
	{
		if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(&groups[tpos], BYOBJ) == true)
		{
			groups[tpos].rmsscr += pow(((objects[tpos2].score) - (groups[tpos].averagescr)), 2);
		}
	}
	groups[tpos].rmsscr = pow((groups[tpos].rmsscr / groups[tpos].size), 0.5);
	for (; tpos2 < GROUPSIZE && !(objects[objectposition].groupsexistst[tpos2] == 1 && objects[objectposition].groups[tpos2]->existst == 1 && objects[objectposition].groups[tpos2] == &groups[tpos]); tpos2++)
	{
	}
	if (tpos2 < GROUPSIZE)
	{
		objects[objectposition].groupsexistst[tpos2] = 0;
		objects[objectposition].groups[tpos2] = NULL;
	}
	return true;
}

bool Statisticsinfos::delfromgroup(const char* gnm, long opos)
{
	if (opos == -1)
	{
		opos = objectposition;
	}
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	for (; tpos < GROUPSIZE && !(objects[opos].groupsexistst[tpos] == 1 && objects[opos].groups[tpos]->existst == 1 && strcmp(objects[opos].groups[tpos]->name, gnm) == 0); tpos++)
	{
	}
	objects[opos].groupsexistst[tpos] = 0;
	objects[opos].groups[tpos]->size--;
	objects[opos].groups[tpos]->allobjectscr -= objects[opos].score;
	objects[opos].groups[tpos]->averagescr = (objects[opos].groups[tpos]->allobjectscr) / (objects[opos].groups[tpos]->size);
	if (objects[opos].score == objects[opos].groups[tpos]->firstobjectscr)
	{
		objects[opos].groups[tpos]->firstobjectscr = 0;
		for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
		{
			if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(objects[opos].groups[tpos], BYOBJ) == true && objects[tpos2].score > objects[opos].groups[tpos]->firstobjectscr)
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
			for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
			{
				if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(objects[opos].groups[tpos], BYOBJ) == true && objects[tpos2].score < objects[opos].groups[tpos]->lastobjectscr)
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
	for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
	{
		if (objects[tpos2].existst == 1 && objects[tpos2].IsInGroup(objects[opos].groups[tpos], BYOBJ) == true)
		{
			objects[opos].groups[tpos]->rmsscr += pow(((objects[tpos2].score) - (objects[opos].groups[tpos]->averagescr)), 2);
		}
	}
	objects[opos].groups[tpos]->rmsscr = pow((objects[opos].groups[tpos]->rmsscr / objects[opos].groups[tpos]->size), 0.5);
	objects[opos].groups[tpos] = NULL;
	return true;
}

bool Statisticsinfos::delfromsubject(long spos, short stp, float* sc)
{
	if (size == 0)
	{
		errcode = ERR_EMPTYMEMORY;
		return false;
	}
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	long tpos4 = 0;
	for (; tpos2 < spos && tpos < SUBJECTSIZE; tpos++)
	{
		if (subjects[tpos].existst == 1 && subjects[tpos].type == stp)
		{
			tpos2++;
		}
	}
	if (tpos == SUBJECTSIZE && tpos2 < spos)
	{
		errcode = ERR_NOSUCHITEM;
		return false;
	}
	objects[objectposition].IsInSubject(&subjects[tpos], BYOBJ, tpos3);
	subjects[tpos].size--;
	subjects[tpos].allobjectscr -= objects[objectposition].subjectscores[tpos3];
	subjects[tpos].averagescr = (subjects[tpos].allobjectscr) / (subjects[tpos].size);
	if (objects[objectposition].subjectscores[tpos3] == subjects[tpos].firstobjectscr)
	{
		subjects[tpos].firstobjectscr = 0;
		for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
		{
			if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(&subjects[tpos], BYOBJ, tpos4) == true && objects[tpos2].subjectscores[tpos4] > subjects[tpos].firstobjectscr)
			{
				subjects[tpos].firstobjectscr = objects[tpos2].subjectscores[tpos4];
			}
		}
	}
	if (objects[objectposition].subjectscores[tpos3] == subjects[tpos].lastobjectscr)
	{
		if (subjects[tpos].size == 0)
		{
			subjects[tpos].lastobjectscr = 0;
		}
		else
		{
			subjects[tpos].lastobjectscr = subjects[tpos].firstobjectscr;
			for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
			{
				if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(&subjects[tpos], BYOBJ, tpos4) == true && objects[tpos2].subjectscores[tpos4] < subjects[tpos].lastobjectscr)
				{
					subjects[tpos].lastobjectscr = objects[tpos2].subjectscores[tpos4];
				}
			}
		}
	}
	if (subjects[tpos].size != 0)
	{
		if (objects[objectposition].subjectscores[tpos3] == subjects[tpos].fullscr)
		{
			subjects[tpos].fullpct = (subjects[tpos].fullpct * (subjects[tpos].size + 1) - 100) / (subjects[tpos].size);
		}
		if (objects[objectposition].subjectscores[tpos3] >= subjects[tpos].exellentscr)
		{
			subjects[tpos].exellentpct = (subjects[tpos].exellentpct * (subjects[tpos].size + 1) - 100) / (subjects[tpos].size);
		}
		if (objects[objectposition].subjectscores[tpos3] >= subjects[tpos].passscr)
		{
			subjects[tpos].passpct = (subjects[tpos].passpct * (subjects[tpos].size + 1) - 100) / (subjects[tpos].size);
		}
		if (objects[objectposition].subjectscores[tpos3] <= subjects[tpos].badscr)
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
	for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
	{
		if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(&subjects[tpos], BYOBJ, tpos4) == true)
		{
			subjects[tpos].rmsscr += pow(((objects[tpos2].subjectscores[tpos4]) - (subjects[tpos].averagescr)), 2);
		}
	}
	subjects[tpos].rmsscr = pow((subjects[tpos].rmsscr / subjects[tpos].size), 0.5);
	for (; tpos2 < SUBJECTSIZE && !(objects[objectposition].subjectsexistst[tpos2] == 1 && objects[objectposition].subjects[tpos2]->existst == 1 && objects[objectposition].subjects[tpos2] == &subjects[tpos]); tpos2++)
	{
	}
	if (tpos2 < SUBJECTSIZE)
	{
		objects[objectposition].subjectsexistst[tpos2] = 0;
		objects[objectposition].subjects[tpos2] = NULL;
	}
	return true;
}

bool Statisticsinfos::delfromsubject(const char* snm, long opos)
{
	if (opos == -1)
	{
		opos = objectposition;
	}
	long tpos = 0;
	long tpos2 = 0;
	long tpos3 = 0;
	for (; tpos < SUBJECTSIZE && !(objects[opos].subjectsexistst[tpos] == 1 && objects[opos].subjects[tpos]->existst == 1 && strcmp(objects[opos].subjects[tpos]->name, snm) == 0); tpos++)
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
		for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
		{
			if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(objects[opos].subjects[tpos], BYOBJ, tpos3) == true && objects[tpos2].subjectscores[tpos3] > objects[opos].subjects[tpos]->firstobjectscr)
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
			for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
			{
				if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(objects[opos].subjects[tpos], BYOBJ, tpos3) == true && objects[tpos2].subjectscores[tpos3] < objects[opos].subjects[tpos]->lastobjectscr)
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
	for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
	{
		if (objects[tpos2].existst == 1 && objects[tpos2].IsInSubject(objects[opos].subjects[tpos], BYOBJ, tpos3) == true)
		{
			objects[opos].subjects[tpos]->rmsscr += pow(((objects[tpos2].subjectscores[tpos3]) - (objects[opos].subjects[tpos]->averagescr)), 2);
		}
	}
	objects[opos].subjects[tpos]->rmsscr = pow((objects[opos].subjects[tpos]->rmsscr / objects[opos].subjects[tpos]->size), 0.5);
	objects[opos].subjects[tpos] = NULL;
	return true;
}

void Statisticsinfos::getsubjectpercents(float& fpct, float& epct, float& tpct, float& ppct)
{
	long tpos;
	long tpos2;
	subjects[subjectposition].fullpct = 0;
	subjects[subjectposition].exellentpct = 0;
	subjects[subjectposition].badpct = 0;
	subjects[subjectposition].passpct = 0;
	for (tpos = 0; tpos < OBJECTSIZE; tpos++)
	{
		if (objects[tpos].existst == 1 && objects[tpos].IsInSubject(&subjects[subjectposition], BYOBJ, tpos2) == true && objects[tpos].subjectscores[tpos2] == subjects[subjectposition].fullscr)
		{
			subjects[subjectposition].fullpct++;
		}
		if (objects[tpos].existst == 1 && objects[tpos].IsInSubject(&subjects[subjectposition], BYOBJ, tpos2) == true && objects[tpos].subjectscores[tpos2] >= subjects[subjectposition].exellentscr)
		{
			subjects[subjectposition].exellentpct++;
		}
		if (objects[tpos].existst == 1 && objects[tpos].IsInSubject(&subjects[subjectposition], BYOBJ, tpos2) == true && objects[tpos].subjectscores[tpos2] <= subjects[subjectposition].badscr)
		{
			subjects[subjectposition].badpct++;
		}
		if (objects[tpos].existst == 1 && objects[tpos].IsInSubject(&subjects[subjectposition], BYOBJ, tpos2) == true && objects[tpos].subjectscores[tpos2] >= subjects[subjectposition].passscr)
		{
			subjects[subjectposition].passpct++;
		}
	}
	if (subjects[subjectposition].size > 0)
	{
		subjects[subjectposition].fullpct = (subjects[subjectposition].fullpct) / (subjects[subjectposition].size) * 100;
		subjects[subjectposition].exellentpct = (subjects[subjectposition].exellentpct) / (subjects[subjectposition].size) * 100;
		subjects[subjectposition].badpct = (subjects[subjectposition].badpct) / (subjects[subjectposition].size) * 100;
		subjects[subjectposition].passpct = (subjects[subjectposition].passpct) / (subjects[subjectposition].size) * 100;
	}
	else
	{
		subjects[subjectposition].fullpct = 0;
		subjects[subjectposition].exellentpct = 0;
		subjects[subjectposition].badpct = 0;
		subjects[subjectposition].passpct = 0;
	}
	fpct = subjects[subjectposition].fullpct;
	epct = subjects[subjectposition].exellentpct;
	tpct = subjects[subjectposition].badpct;
	ppct = subjects[subjectposition].passpct;
}

bool Statisticsinfos::getscoredpercents(float& pct, short pms, Statisticssubject* sub = NULL, Statisticsgroup* grp = NULL)
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
			sub = &subjects[subjectposition];
		}
		if (sub->existst == 0)
		{
			pct = 0;
			errcode = ERR_LACKPARAMETER;
			return false;
		}
		if (grp == NULL)
		{
			grp = &groups[groupposition];
		}
		if (grp->existst == 0)
		{
			pms = 2;
			goto redo;
		}
		for (tpos = 0; tpos < OBJECTSIZE; tpos++)
		{
			if (objects[tpos].existst == 1)
			{
				for (tpos2 = 0; tpos2 < GROUPSIZE; tpos2++)
				{
					if ((objects[tpos].groupsexistst[tpos2]) == 1 && (objects[tpos].groups[tpos2]) == grp)
					{
						for (tpos3 = 0; tpos3 < SUBJECTSIZE; tpos3++)
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
			sub = &subjects[subjectposition];
		}
		if (sub->existst == 0)
		{
			pct = 0;
			errcode = ERR_LACKPARAMETER;
			return false;
		}
		for (tpos = 0; tpos < OBJECTSIZE; tpos++)
		{
			if (objects[tpos].existst == 1)
			{
				for (tpos3 = 0; tpos3 < SUBJECTSIZE; tpos3++)
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
		errcode = ERR_NOSUCHITEM;
		return false;
	}
	return true;
}

bool Statisticsinfos::changesubscr(float scr, char* sub, Statisticsinfo* obj)
{
	if (scr < 0)
	{
		errcode = ERR_LACKPARAMETER;
		return false;
	}
	long tpos = 0;
	Statisticssubject* psub = NULL;
	if (sub == NULL)
	{
		psub = &subjects[subjectposition];
	}
	else
	{
		for (tpos = 0; !(subjects[tpos].existst == 1 && strcmp(subjects[tpos].name, sub) == 0) && tpos < OBJECTSIZE; tpos++) {}
		psub = &subjects[tpos];
	}
	if (psub->existst == 0)
	{
		errcode = ERR_LACKPARAMETER;
		return false;
	}
	if (obj == NULL)
	{
		obj = &objects[objectposition];
	}
	if (obj->existst == 0)
	{
		errcode = ERR_LACKPARAMETER;
		return false;
	}
	for (tpos = 0; !((obj->subjectsexistst[tpos]) == 1 && (obj->subjects[tpos]) == psub) && tpos < SUBJECTSIZE; tpos++) {}
	if (tpos < SUBJECTSIZE)
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
		for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
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
			objectposition = 0;
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
					for (tpos2 = 0; (objects[tpos2].existst != 1 || lastobjectscr != objects[tpos2].score) && tpos2 < OBJECTSIZE; tpos2++) {}
					if (tpos2 < OBJECTSIZE)
					{
						obj->rank = objects[tpos2].rank;
					}
				}
			}
			else
			{
				for (tpos2 = 0; (objects[tpos2].rank != 1 || objects[tpos2].existst != 1) && tpos2 < OBJECTSIZE; tpos2++) {}
				tposition = 1;
				for (; objects[tpos2].score > obj->score; tposition++)
				{
				retry1:
					for (tpos2 = 0; (objects[tpos2].rank != (tposition + 1) || objects[tpos2].existst != 1) && tpos2 < OBJECTSIZE; tpos2++) {}
					if (tpos2 == OBJECTSIZE)
					{
						tposition++;
						goto retry1;
					}
				}
				obj->rank = tposition;
			}
		}
		for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
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
		for (tpos4 = 0; tpos4 < OBJECTSIZE; tpos4++)
		{
			if (objects[tpos4].existst == 1 && objects[tpos4].IsInSubject(obj->subjects[tpos], BYOBJ, tpos5) == true)
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
		for (tpos3 = 0; tpos3 < GROUPSIZE; tpos3++)
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
				for (tpos4 = 0; tpos4 < OBJECTSIZE; tpos4++)
				{
					if (objects[tpos4].existst == 1 && objects[tpos4].IsInGroup(obj->groups[tpos3], BYOBJ) == true)
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
		errcode = ERR_NOSUCHITEM;
		return false;
	}
	return true;
}

bool Statisticsinfos::changeobjscr(float scr, Statisticsinfo* obj)
{
	if (scr < 0)
	{
		errcode = ERR_LACKPARAMETER;
		return false;
	}
	if (obj == NULL)
	{
		obj = &objects[objectposition];
	}
	if (obj->existst == 0)
	{
		errcode = ERR_LACKPARAMETER;
		return false;
	}
	obj->score = scr;
	return true;
}

bool Statisticsinfos::getmsg(char**& msg_string, MSGSTRUCT*& msg_array, long& msgcnt, short type)
{
	if (type - 3 > 0)
	{
		errcode = ERR_NOSUCHITEM;
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
					case ERR_NOMEMORY:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:Memory full. Unable to add new item.");
						break;
					case ERR_EMPTYMEMORY:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:Memory Empty. Unable to delete item.");
						break;
					case ERR_NOSUCHITEM:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:The chosen item does not exist.");
						break;
					case ERR_NOSUCHITEM_POSINC:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:No such object with the input rank.The rank increased.");
						break;
					case ERR_NOSUCHITEM_POSDEC:
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
					case ERR_ITEMALREADYEXISTS:
						strcpy(msg_str[msgcnt], "");
						strcpy(msg_str[msgcnt], "Error Message:Item already exists.Unable to add it again.");
						break;
					default:
						errcode = ERR_NOSUCHITEM;
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

void Statisticsinfos::clearmsg()
{
	long tpos = 0;
	for (tpos = 0; tpos < MSGSIZE; tpos++)
	{
		msg_stc[tpos].msgflag = 0;
	}
}

void Statisticsinfos::gotosubject(long& pos, bool status)
{
	if (pos < SUBJECTSIZE)
	{
		if (status == true)
		{
			pos = subjectposition;
		}
		else
		{
			subjectposition = pos;
		}
	}
}

void Statisticsinfos::nextsubject(char*& snm, short stp)
{
	long tpos;
	if (stp != ALLSUBJECTS)
	{
		for (tpos = subjectposition + 1; (subjects[tpos].existst != 1 || subjects[tpos].type != stp) && tpos < SUBJECTSIZE; tpos++)
		{
		}
	}
	else
	{
		for (tpos = subjectposition + 1; subjects[tpos].existst != 1 && tpos < SUBJECTSIZE; tpos++)
		{
		}
	}
	if (tpos == SUBJECTSIZE)
	{
		strcpy(snm, "NULLSUBJECT");
		return;
	}
	else
	{
		strcpy(snm, subjects[tpos].name);
		subjectposition = tpos;
	}
}

bool Statisticsinfos::searchsubject(void* si, short type)
{
	long tpos = 0;
	switch (type)
	{
	case BYNAME:
		for (; (strcmp(subjects[tpos].name, (const char*)si) != 0 || subjects[tpos].existst != 1) && tpos < SUBJECTSIZE; tpos++) {}
		break;
	case BYOBJ:
		for (; (&subjects[tpos] != (Statisticssubject*)si || subjects[tpos].existst != 1) && tpos < SUBJECTSIZE; tpos++) {}
		break;
	default:
		break;
	}
	if (tpos == SUBJECTSIZE)
	{
		errcode = ERR_NOSUCHITEM;
		return false;
	}
	subjectposition = tpos;
	return true;
}

bool Statisticsinfos::outgroup(char*& gnm, short& gtp)
{
	long tpos;
	long tpos2;
	long tpos3;
	for (tpos = 0; groups[tpos].existst != 1 && tpos < GROUPSIZE; tpos++) {}
	if (tpos == GROUPSIZE)
	{
		errcode = ERR_EMPTYMEMORY;
		return false;
	}
	tpos = groupposition;
	strcpy(gnm, groups[tpos].name);
	gtp = groups[tpos].type;
	groups[tpos].existst = 0;
	for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
	{
		if (objects[tpos2].existst == 1)
		{
			for (tpos3 = 0; tpos3 < GROUPSIZE; tpos3++)
			{
				if (objects[tpos2].groupsexistst[tpos3] == 1 && objects[tpos2].groups[tpos3] == &groups[tpos])
				{
					objects[tpos2].groups[tpos3] = NULL;
					objects[tpos2].groupsexistst[tpos3] = 0;
				}
			}
		}
	}
	for (tpos2 = 0; groups[tpos2].existst != 1 && tpos2 < GROUPSIZE; tpos2++) {}
	if (tpos2 == GROUPSIZE)
	{
		groupposition = 0;
	}
	else
	{
		groupposition = tpos2;
	}
	return true;
}

bool Statisticsinfos::outsubject(char*& snm, short& stp, long& maxsz)
{
	long tpos;
	long tpos2;
	long tpos3;
	for (tpos = 0; subjects[tpos].existst != 1 && tpos < SUBJECTSIZE; tpos++) {}
	if (tpos == SUBJECTSIZE)
	{
		errcode = ERR_EMPTYMEMORY;
		return false;
	}
	tpos = subjectposition;
	snm = subjects[tpos].name;
	maxsz = subjects[tpos].maxsize;
	stp = subjects[tpos].type;
	subjects[tpos].existst = 0;
	for (tpos2 = 0; tpos2 < OBJECTSIZE; tpos2++)
	{
		if (objects[tpos2].existst == 1)
		{
			for (tpos3 = 0; tpos3 < SUBJECTSIZE; tpos3++)
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
	for (tpos2 = 0; subjects[tpos2].existst != 1 && tpos2 < SUBJECTSIZE; tpos2++) {}
	if (tpos2 == SUBJECTSIZE)
	{
		subjectposition = 0;
	}
	else
	{
		subjectposition = tpos2;
	}
	return true;
}
