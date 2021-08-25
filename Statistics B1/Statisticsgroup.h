// Statisticsgroup.h: interface for the Statisticsgroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STATISTICSGROUP_H__INCLUDED_)
#define STATISTICSGROUP_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Statisticsgroup  
{
public:
	short grouptype;
	short existst;
	long groupmembersize;
	CString groupname;
	Statisticsgroup();
	virtual ~Statisticsgroup();

};

#endif // !defined(STATISTICSGROUP_H__INCLUDED_)
