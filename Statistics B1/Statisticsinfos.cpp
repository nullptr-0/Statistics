// Statisticsinfos.cpp: implementation of the Statisticsinfos class.
//
//////////////////////////////////////////////////////////////////////

#include "math.h"
#include "Shared.h"
#include "Statisticsinfos.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Statisticsinfos::Statisticsinfos()
{
	infos=new Statisticsinfo[DATASIZE];
	groups=new Statisticsgroup[GROUPSIZE];
	subjects=new Statisticssubject[SUBJECTSIZE];
	objectsize=0;
	groupsize=0;
	subjectsize=0;
	position=0;
	groupposition=0;
	subjectposition=0;
}

Statisticsinfos::~Statisticsinfos()
{
	delete []infos;
	delete []groups;
	delete []subjects;
}

void Statisticsinfos::searchob(void* si, short type)
{
	long tpos=0;
	switch(type)
	{
		case 1:
			for(; (infos[tpos].objectname!=*((CString*)si)||infos[tpos].existst!=1)&&tpos<DATASIZE;tpos++){}
			if(tpos==DATASIZE)
			{
				MessageBox(NULL,"没有与搜索内容匹配的对象","注意",MB_OK);
				return;
			}
			position=tpos;
			break;
		case 2:
			for(; (infos[tpos].objectnumber!=*((CString*)si)||infos[tpos].existst!=1)&&tpos<DATASIZE;tpos++){}
			if(tpos==DATASIZE)
			{
				MessageBox(NULL,"没有与搜索内容匹配的对象","注意",MB_OK);
				return;
			}
			position=tpos;
			break;
	}
}

void Statisticsinfos::getallob(long &sz,long &gsz,long &ssz)
{
	sz=objectsize;
	gsz=groupsize;
	ssz=subjectsize;
}

void Statisticsinfos::inob(CString tnm, CString tnb, short tsx)
{
	if(objectsize==DATASIZE)
	{
		MessageBox(NULL,"存储已满,不能添加项目","注意",MB_OK);
		return;
	}
	long tpos;
	for(tpos=0;infos[tpos].existst==1&&tpos<DATASIZE;tpos++){}
	infos[tpos].objectname=tnm;
	infos[tpos].objectnumber=tnb;
	infos[tpos].objectsex=tsx;
	infos[tpos].existst=1;
	objectsize++;

	long tempgp=groupposition;
	long tempsp=subjectposition;
	long tpos3;
	long tpos4=0;
	long tpos5=0;
	groupposition=0;
	for(;;)
	{
		for(tpos3=groupposition;(groups[tpos3].existst!=1||groups[tpos3].grouptype!=NOTCHOOSABLEGROUPS)&&tpos3<GROUPSIZE;tpos3++){}
		if(tpos3==GROUPSIZE)
		{
			groupposition=tempgp;
			goto con;
		}
		else
		{
			groupposition=tpos3;
		}
		for(;infos[tpos].groupsexistst[tpos5]==1&&tpos5<GROUPSIZE;tpos5++)
		{
		}
		if(tpos5==GROUPSIZE)
		{
			MessageBox(NULL,"存储已满,不能添加到集体","注意",MB_OK);
			groupposition=tempgp;
			goto con;
		}
		infos[tpos].groups[tpos5]=groups+groupposition;
		infos[tpos].groupsexistst[tpos5]=1;
		groups[groupposition].groupmembersize++;
		groupposition++;
	}
con:
	tpos4=0;
	tpos5=0;
	subjectposition=0;
	for(;;)
	{
		for(tpos3=subjectposition;(subjects[tpos3].existst!=1||subjects[tpos3].subjecttype!=NOTCHOOSABLESUBJECTS)&&tpos3<SUBJECTSIZE;tpos3++){}
		if(tpos3==SUBJECTSIZE)
		{
			subjectposition=tempsp;
			goto con2;
		}
		else
		{
			subjectposition=tpos3;
		}
		for(;infos[tpos].subjectsexistst[tpos5]==1&&tpos5<SUBJECTSIZE;tpos5++)
		{
		}
		if(tpos5==SUBJECTSIZE)
		{
			MessageBox(NULL,"存储已满,不能添加到科目","注意",MB_OK);
			subjectposition=tempsp;
			goto con2;
		}
		infos[tpos].subjects[tpos5]=subjects+subjectposition;
		infos[tpos].subjectsexistst[tpos5]=1;
		subjects[subjectposition].subjectmembersize++;
		subjectposition++;
	}
con2:;
}

void Statisticsinfos::outob(CString &tnm, CString &tnb, short &tsx)
{
	if(objectsize==0)
	{
		MessageBox(NULL,"存储已空,不能删除项目","注意",MB_OK);
		tnm=_T("NULLOBJECT");
		return;
	}
	long tpos;
	tpos=position;
	tnm=infos[tpos].objectname;
	tnb=infos[tpos].objectnumber;
	tsx=infos[tpos].objectsex;
	infos[tpos].existst=0;
	objectsize--;
	if(objectsize==0)
	{
		position=0;
		return;
	}

	long tpos3;
	long tpos4=0;
	long tpos5=0;
	long grppos=0;
	for(;;)
	{
		for(tpos3=grppos;infos[tpos].groupsexistst[tpos3]!=1&&tpos3<GROUPSIZE;tpos3++){}
		if(tpos3==GROUPSIZE)
		{
			goto con;
		}
		else
		{
			grppos=tpos3;
		}
		(infos[tpos].groups[grppos]->groupmembersize)--;
		grppos++;
	}
con:
	tpos4=0;
	tpos5=0;
	long sbjpos=0;
	for(;;)
	{
		for(tpos3=sbjpos;infos[tpos].subjectsexistst[tpos3]!=1&&tpos3<SUBJECTSIZE;tpos3++){}
		if(tpos3==SUBJECTSIZE)
		{
			goto con2;
		}
		else
		{
			sbjpos=tpos3;
		}
		(infos[tpos].subjects[sbjpos]->subjectmembersize)--;
		sbjpos++;
	}
con2:;
}

void Statisticsinfos::getob(CString &tnm, CString &tnb, short &tsx)
{
	tnm=infos[position].objectname;
	tnb=infos[position].objectnumber;
	tsx=infos[position].objectsex;
}

void Statisticsinfos::nextob(long times)
{
	if(times==0)
		return;
	long tpos=0;
	if(tpos>=objectsize)
	{
		MessageBox(NULL,"无效数值","注意",MB_OK);
		return;
	}
	long tpos2=position+1;
retry2:
	if(tpos2>objectsize)
	{
		MessageBox(NULL,"没有可用的对象","注意",MB_OK);
		return;
	}
	for(tpos=0;tpos!=times&&tpos2<DATASIZE;tpos2++)
	{
		if(infos[tpos2].existst==1)
			tpos++;
	}
	if(tpos2==DATASIZE&&tpos!=times)
	{
		tpos++;
		MessageBox(NULL,"没有可用的对象","注意",MB_OK);
		goto retry2;
	}
	position=tpos2-1;
}

void Statisticsinfos::lastob(long times)
{
	if(times==0)
		return;
	long tpos=0;
	if(times<=0)
	{
		MessageBox(NULL,"无效数值","注意",MB_OK);
		return;
	}
	long tpos2=position-1;
retry3:
	if(tpos2<0)
	{
		MessageBox(NULL,"没有可用的对象","注意",MB_OK);
		return;
	}
	for(tpos=0;tpos!=times&&tpos2>=0;tpos2--)
	{
		if(infos[tpos2].existst==1)
			tpos++;
	}
	if(tpos2<0&&tpos!=times)
	{
		tpos--;
		MessageBox(NULL,"没有可用的对象","注意",MB_OK);
		goto retry3;
	}
	position=tpos2+1;
}

void Statisticsinfos::gotoob(long pos)
{
	if(pos>DATASIZE||pos<=0)
	{
		MessageBox(NULL,"没有可用的对象","注意",MB_OK);
		return;
	}
	long tpos;
	long tpos2;
retry6:
	for(tpos=0,tpos2=0;tpos2!=pos&&tpos<DATASIZE;tpos++)
	{
		if(infos[tpos].existst==1)
			tpos2++;
	}
	if(tpos==DATASIZE&&tpos2!=pos)
	{
		pos--;
		MessageBox(NULL,"没有可用的对象","注意",MB_OK);
		goto retry6;
	}
	position=tpos-1;
}

void Statisticsinfos::getob(Statisticsinfo* &pinfo)
{
	pinfo=infos+position;
}

void Statisticsinfos::nextob(CString &tnm, CString &tnb, short &tsx)
{
	long tpos;
	for(tpos=position;infos[tpos].existst!=1&&tpos<DATASIZE;tpos++)
	{
	}
	if(tpos==DATASIZE)
	{
		tnm=_T("NULLOBJECT");
		return;
	}
	else
	{
		tnm=infos[tpos].objectname;
		tnb=infos[tpos].objectnumber;
		tsx=infos[tpos].objectsex;
		position=tpos+1;
	}
}

void Statisticsinfos::gotoob(long &pos, bool status)
{
	if(status==true)
	{
		pos=position;
	}
	else
	{
		position=pos;
	}
}

void Statisticsinfos::ingroup(CString gnm,short gtp)
{
	long tpos;
	long tpos2;
	long tpos3;
	for(tpos=0;groups[tpos].existst==1&&tpos<GROUPSIZE;tpos++){}
	if(tpos==GROUPSIZE)
	{
		MessageBox(NULL,"存储已满,不能添加项目","注意",MB_OK);
		return;
	}
	groups[tpos].groupname=gnm;
	groups[tpos].grouptype=gtp;
	groups[tpos].existst=1;
	groupsize++;
	if(gtp==1)
	{
		for(tpos2=0;tpos2<DATASIZE;tpos2++)
		{
			if(infos[tpos2].existst==1)
			{
				for(tpos3=0;infos[tpos2].groupsexistst[tpos3]==1&&tpos3<GROUPSIZE;tpos3++)
				{
				}
				if(tpos3<GROUPSIZE)
				{
					infos[tpos2].groups[tpos3]=groups+tpos;
					infos[tpos2].groupsexistst[tpos3]=1;
				}
			}
		}
		groups[tpos].groupmembersize=objectsize;
	}
	for(tpos2=0;(groups[tpos2].existst!=1||groups+tpos2==groups+tpos)&&tpos2<GROUPSIZE;tpos2++){}
	if(tpos==GROUPSIZE)
	{
		groupposition=tpos;
	}
}

void Statisticsinfos::getgroup(CString &gnm, short &gtp, long &gsz, float &atdpct)
{
	gnm=groups[groupposition].groupname;
	gtp=groups[groupposition].grouptype;
	gsz=groups[groupposition].groupmembersize;
	if(objectsize!=0)
		atdpct=(groups[groupposition].groupmembersize*100)/objectsize;
	else
		atdpct=0;
}

void Statisticsinfos::searchgroup(CString* si, short type)
{
	long tpos=0;
	for(;(groups[tpos].groupname!=*si||groups[tpos].existst!=1)&&tpos<GROUPSIZE;tpos++){}
	if(tpos==GROUPSIZE)
	{
		MessageBox(NULL,"没有与搜索内容匹配的对象","注意",MB_OK);
		return;
	}
	groupposition=tpos;
}

void Statisticsinfos::addtogroup(long gpos)
{
	long tpos=0;
	long tpos2=0;
	long tpos3=0;
	for(;infos[position].groupsexistst[tpos3]==1&&tpos3<GROUPSIZE;tpos3++){}
	if(tpos3==GROUPSIZE)
	{
		MessageBox(NULL,"存储已满,不能添加到集体","注意",MB_OK);
		return;
	}
	for(;tpos2<gpos&&tpos<GROUPSIZE;tpos++)
	{
		if(groups[tpos].existst==1)
		{
			tpos2++;
		}
	}
	infos[position].groups[tpos3]=groups+tpos;
	infos[position].groupsexistst[tpos3]=1;
	groups[tpos].groupmembersize++;
}

void Statisticsinfos::nextgroup(CString &gnm,short gtp)
{
	long tpos;
	if(gtp!=ALLGROUPS)
	{
		for(tpos=groupposition+1;(groups[tpos].existst!=1||groups[tpos].grouptype!=gtp)&&tpos<GROUPSIZE;tpos++)
		{
		}
	}
	else
	{
		for(tpos=groupposition+1;groups[tpos].existst!=1&&tpos<GROUPSIZE;tpos++)
		{
		}
	}
	if(tpos==GROUPSIZE)
	{
		gnm=_T("NULLGROUP");
		return;
	}
	else
	{
		gnm=groups[tpos].groupname;
		groupposition=tpos;
	}
}

void Statisticsinfos::gotogroup(long &pos, bool status)
{
	if(status==true)
	{
		pos=groupposition;
	}
	else
	{
		groupposition=pos;
	}
}

void Statisticsinfos::insubject(CString snm, short stp,long smsz)
{
	long tpos;
	long tpos2;
	long tpos3;
	for(tpos=0;subjects[tpos].existst==1&&tpos<SUBJECTSIZE;tpos++){}
	if(tpos==SUBJECTSIZE)
	{
		MessageBox(NULL,"存储已满,不能添加科目","注意",MB_OK);
		return;
	}
	subjects[tpos].subjectname=snm;
	subjects[tpos].subjecttype=stp;
	subjects[tpos].subjectmaxsize=smsz;
	subjects[tpos].existst=1;
	subjectsize++;
	if(stp==1)
	{
		for(tpos2=0;tpos2<DATASIZE;tpos2++)
		{
			if(infos[tpos2].existst==1)
			{
				for(tpos3=0;infos[tpos2].subjectsexistst[tpos3]==1&&tpos3<SUBJECTSIZE;tpos3++)
				{
				}
				if(tpos3<SUBJECTSIZE)
				{
					infos[tpos2].subjects[tpos3]=subjects+tpos;
					infos[tpos2].subjectsexistst[tpos3]=1;
				}
			}
		}
		subjects[tpos].subjectmembersize=objectsize;
	}
	for(tpos2=0;(subjects[tpos2].existst!=1||subjects+tpos2==subjects+tpos)&&tpos2<SUBJECTSIZE;tpos2++){}
	if(tpos==SUBJECTSIZE)
	{
		subjectposition=tpos;
	}
}

void Statisticsinfos::addtosubject(long spos)
{
	long tpos=0;
	long tpos2=0;
	long tpos3=0;
	for(;infos[position].subjectsexistst[tpos3]==1&&tpos3<SUBJECTSIZE;tpos3++){}
	if(tpos3==SUBJECTSIZE)
	{
		MessageBox(NULL,"存储已满,不能添加到科目","注意",MB_OK);
		return;
	}
	for(;tpos2<spos&&tpos<SUBJECTSIZE;tpos++)
	{
		if(subjects[tpos].existst==1&&subjects[tpos].subjecttype==CHOOSABLESUBJECTS)
		{
			tpos2++;
		}
	}
	tpos--;
	if((subjects[tpos].subjectmembersize)==(subjects[tpos].subjectmaxsize))
	{
		MessageBox(NULL,"科目人数已达到最大限制,不能添加到科目","注意",MB_OK);
		return;
	}
	infos[position].subjects[tpos3]=subjects+tpos;
	infos[position].subjectsexistst[tpos3]=1;
	subjects[tpos].subjectmembersize++;
}

void Statisticsinfos::getsubject(CString &snm, short &stp, long &ssz, float &atdpct)
{
	snm=subjects[subjectposition].subjectname;
	stp=subjects[subjectposition].subjecttype;
	ssz=subjects[subjectposition].subjectmembersize;
	if(objectsize!=0)
		atdpct=(subjects[subjectposition].subjectmembersize*100)/objectsize;
	else
		atdpct=0;
}

void Statisticsinfos::gotosubject(long &pos, bool status)
{
	if(status==true)
	{
		pos=subjectposition;
	}
	else
	{
		subjectposition=pos;
	}
}

void Statisticsinfos::nextsubject(CString &snm, short stp)
{
	long tpos;
	if(stp!=ALLSUBJECTS)
	{
		for(tpos=subjectposition+1;(subjects[tpos].existst!=1||subjects[tpos].subjecttype!=stp)&&tpos<SUBJECTSIZE;tpos++)
		{
		}
	}
	else
	{
		for(tpos=subjectposition+1;subjects[tpos].existst!=1&&tpos<SUBJECTSIZE;tpos++)
		{
		}
	}
	if(tpos==SUBJECTSIZE)
	{
		snm=_T("NULLSUBJECT");
		return;
	}
	else
	{
		snm=subjects[tpos].subjectname;
		subjectposition=tpos;
	}
}

void Statisticsinfos::searchsubject(CString *si, short type)
{
	long tpos=0;
	for(;(subjects[tpos].subjectname!=*si||subjects[tpos].existst!=1)&&tpos<SUBJECTSIZE;tpos++){}
	if(tpos==SUBJECTSIZE)
	{
		MessageBox(NULL,"没有与搜索内容匹配的对象","注意",MB_OK);
		return;
	}
	subjectposition=tpos;
}

void Statisticsinfos::outgroup(CString &gnm, short &gtp)
{
	long tpos;
	long tpos2;
	long tpos3;
	for(tpos=0;groups[tpos].existst!=1&&tpos<GROUPSIZE;tpos++){}
	if(tpos==GROUPSIZE)
	{
		MessageBox(NULL,"存储已空,不能删除项目","注意",MB_OK);
		gnm=_T("NULLGROUP");
		return;
	}
	tpos=groupposition;
	gnm=groups[tpos].groupname;
	gtp=groups[tpos].grouptype;
	groups[tpos].existst=0;
	groupsize--;
	for(tpos2=0;tpos2<DATASIZE;tpos2++)
	{
		if(infos[tpos2].existst==1)
		{
			for(tpos3=0;tpos3<GROUPSIZE;tpos3++)
			{
				if(infos[tpos2].groupsexistst[tpos3]==1&&infos[tpos2].groups[tpos3]==groups+tpos)
				{
					infos[tpos2].groups[tpos3]=NULL;
					infos[tpos2].groupsexistst[tpos3]=0;
				}
			}
		}
	}
	for(tpos2=0;groups[tpos2].existst!=1&&tpos2<GROUPSIZE;tpos2++){}
	if(tpos2==GROUPSIZE)
	{
		groupposition=0;
		groups[groupposition].groupname=_T("");
		groups[groupposition].grouptype=0;
		groups[groupposition].existst=0;
		groups[groupposition].groupmembersize=0;
		return;
	}
	for(tpos2=tpos+1;groups[tpos2].existst!=1&&tpos2<GROUPSIZE;tpos2++){}
	if(tpos==GROUPSIZE)
	{
		for(tpos2=tpos-1;groups[tpos2].existst!=1&&tpos2>=0;tpos2--){}
		groupposition=tpos2;
	}
	else
	{
		groupposition=tpos2;
	}
}

void Statisticsinfos::outsubject(CString snm, short stp,long smsz)
{
	long tpos;
	long tpos2;
	long tpos3;
	for(tpos=0;subjects[tpos].existst!=1&&tpos<SUBJECTSIZE;tpos++){}
	if(tpos==SUBJECTSIZE)
	{
		MessageBox(NULL,"存储已空,不能删除科目","注意",MB_OK);
		snm=_T("NULLSUBJECT");
		return;
	}
	tpos=subjectposition;
	snm=subjects[tpos].subjectname;
	stp=subjects[tpos].subjecttype;
	smsz=subjects[tpos].subjectmaxsize;
	subjects[tpos].existst=0;
	subjectsize--;
	for(tpos2=0;tpos2<DATASIZE;tpos2++)
	{
		if(infos[tpos2].existst==1)
		{
			for(tpos3=0;tpos3<SUBJECTSIZE;tpos3++)
			{
				if(infos[tpos2].subjectsexistst[tpos3]==1&&infos[tpos2].subjects[tpos3]==subjects+tpos)
				{
					infos[tpos2].subjects[tpos3]=NULL;
					infos[tpos2].subjectsexistst[tpos3]=0;
				}
			}
		}
	}
	for(tpos2=0;subjects[tpos2].existst!=1&&tpos2<SUBJECTSIZE;tpos2++){}
	if(tpos2==SUBJECTSIZE)
	{
		subjectposition=0;
		return;
	}
	for(tpos2=tpos+1;subjects[tpos2].existst!=1&&tpos2<SUBJECTSIZE;tpos2++){}
	if(tpos==SUBJECTSIZE)
	{
		for(tpos2=tpos-1;subjects[tpos2].existst!=1&&tpos2>=0;tpos2--){}
		subjectposition=tpos2;
	}
	else
	{
		subjectposition=tpos2;
	}
}
