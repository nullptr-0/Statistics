// Statisticssubject.h: interface for the Statisticssubject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STATISTICSSUBJECT_H__INCLUDED_)
#define STATISTICSSUBJECT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Statisticssubject  
{
public:
	short subjecttype;
	short existst;
	long subjectmembersize;
	long subjectmaxsize;
	CString subjectname;
	Statisticssubject();
	virtual ~Statisticssubject();

};

#endif // !defined(STATISTICSSUBJECT_H__INCLUDED_)
