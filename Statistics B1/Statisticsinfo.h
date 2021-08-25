// Statisticsinfo.h: interface for the Statisticsinfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STATISTICSINFO_H__INCLUDED_)
#define STATISTICSINFO_H__INCLUDED_

#include "Statisticsgroup.h"
#include "Statisticssubject.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Statisticsinfo  
{
public:
	short existst;
	CString objectname;
	CString objectnumber;
	short objectsex;
	Statisticsgroup** groups;
	Statisticssubject** subjects;
	short* groupsexistst;
	short* subjectsexistst;
	Statisticsinfo();
	virtual ~Statisticsinfo();
	bool IsInGroup(void* item,short type);
	bool IsInSubject(void* item,short type);
};

#endif // !defined(STATISTICSINFO_H__INCLUDED_)
