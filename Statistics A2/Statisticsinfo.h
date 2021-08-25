// Statisticsinfo.h: interface for the Statisticsinfo class.
//
//////////////////////////////////////////////////////////////////////

class __declspec(dllexport) Statisticsinfo
{
public:
	short existst;
	char* name;
	char* number;
	short sex;
	float score;
	long rank;
	Statisticsgroup** groups;
	Statisticssubject** subjects;
	short* groupsexistst;
	long* groupranks;
	short* subjectsexistst;
	float* subjectscores;
	long* subjectranks;
	short marked;
	Statisticsinfo();
	virtual ~Statisticsinfo();
	bool IsInGroup(void* item, short type);
	bool IsInSubject(void* item, short type);
	bool IsInSubject(void* item, short type, long &pos);
};
