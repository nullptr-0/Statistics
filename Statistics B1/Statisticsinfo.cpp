// Statisticsinfo.cpp: implementation of the Statisticsinfo class.
//
//////////////////////////////////////////////////////////////////////

#include "Shared.h"
#include "Statisticsinfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Statisticsinfo::Statisticsinfo()
{
	groups=new Statisticsgroup*[GROUPSIZE];
	subjects=new Statisticssubject*[SUBJECTSIZE];
	groupsexistst=new short[GROUPSIZE];
	subjectsexistst=new short[SUBJECTSIZE];
	objectname=_T("");
	objectnumber=_T("");
	objectsex=0;
	existst=0;
	long tpos;
	for(tpos=0;tpos<GROUPSIZE;tpos++)
	{
		groups[tpos]=NULL;
		groupsexistst[tpos]=0;
	}
	for(tpos=0;tpos<SUBJECTSIZE;tpos++)
	{
		subjects[tpos]=NULL;
		subjectsexistst[tpos]=0;
	}
}

Statisticsinfo::~Statisticsinfo()
{
	delete []groups;
	delete []subjects;
	delete []groupsexistst;
	delete []subjectsexistst;
}

#define BYNAME 0

bool Statisticsinfo::IsInGroup(void* item,short type)
{
	long tpos;
	bool state=false;
	switch(type)
	{
		case BYNAME:
			for(tpos=0;tpos<GROUPSIZE&&((groupsexistst[tpos]!=1)||(groups[tpos]->groupname!=*((CString*)item)));tpos++)
			{
			}
			if(tpos<GROUPSIZE)
				state=true;
			break;
		default:
			break;
	}
	return state;
}

bool Statisticsinfo::IsInSubject(void* item,short type)
{
	long tpos;
	bool state=false;
	switch(type)
	{
		case BYNAME:
			for(tpos=0;tpos<SUBJECTSIZE&&((subjectsexistst[tpos]!=1)||(subjects[tpos]->subjectname!=*((CString*)item)));tpos++)
			{
			}
			if(tpos<SUBJECTSIZE)
				state=true;
			break;
		default:
			break;
	}
	return state;
}
