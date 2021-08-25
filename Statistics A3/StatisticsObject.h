// StatisticsObject.h: interface for the StatisticsObject class.
//
//////////////////////////////////////////////////////////////////////

class __declspec(dllexport) StatisticsObject
{
public:
	short existst;
	char* name;
	char* number;
	short sex;
	float score;
	long rank;
	StatisticsGroup** groups;
	StatisticsSubject** subjects;
	short* groupsexistst;
	long* groupranks;
	short* subjectsexistst;
	float* subjectscores;
	long* subjectranks;
	short marked;
	StatisticsObject();
	virtual ~StatisticsObject();
	bool IsInGroup(void* item, short type);
	bool IsInSubject(void* item, short type);
	bool IsInSubject(void* item, short type, long &pos);
};
