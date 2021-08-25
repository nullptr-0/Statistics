// Statisticsinfos.h: interface for the Statisticsinfos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STATISTICSINFOS_H__INCLUDED_)
#define STATISTICSINFOS_H__INCLUDED_

#include "Statisticsinfo.h"

#define ALLGROUPS 0
#define NOTCHOOSABLEGROUPS 1
#define CHOOSABLEGROUPS 2
#define ALLSUBJECTS 0
#define NOTCHOOSABLESUBJECTS 1
#define CHOOSABLESUBJECTS 2

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Statisticsinfos  
{
public:
	void outsubject(CString snm,short stp,long smsz);
	void outgroup(CString &gnm,short &gtp);
	void searchsubject(CString *si,short type);
	void nextsubject(CString &snm,short stp);
	void gotosubject(long &pos,bool status);
	void getsubject(CString &snm,short &stp,long &ssz, float &atdpct);
	void addtosubject(long spos);
	void insubject(CString snm,short stp,long smsz);
	Statisticsinfos();
	virtual ~Statisticsinfos();
	void getob(Statisticsinfo* &pinfo);
	void gotoob(long pos);
	void lastob(long times);
	void nextob(long times);
	void getob(CString &tnm,CString &tnb,short &tsx);
	void outob(CString &tnm,CString &tnb,short &tsx);
	void inob(CString tnm,CString tnb,short tsx);
	void getallob(long &sz,long &gsz,long &ssz);
	void searchob(void* si,short type);
	void nextob(CString &tnm, CString &tnb, short &tsx);
	void gotoob(long &pos,bool status);
	void nextgroup(CString &gnm,short gtp);
	void gotogroup(long &pos, bool status);
	void ingroup(CString gnm,short gtp);
	void getgroup(CString &gnm,short &gtp,long &gsz, float &atdpct);
	void searchgroup(CString* si,short type);
	void addtogroup(long gpos);

	Statisticsinfo* infos;
	Statisticsgroup* groups;
	Statisticssubject* subjects;
	long objectsize;
	long groupsize;
	long subjectsize;
	long position;
	long groupposition;
	long subjectposition;
};

#endif // !defined(STATISTICSINFOS_H__INCLUDED_)
