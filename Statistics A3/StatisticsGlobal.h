// StatisticsGlobal.h: interface for the StatisticsGlobal class.
//
//////////////////////////////////////////////////////////////////////

struct MSGSTRUCT
{
	StatisticsObject* objsrc;
	StatisticsGroup* grpsrc;
	StatisticsSubject* subsrc;
	int msgflag;
};

struct SUB_SCR
{
	StatisticsSubject* sub;
	float scr;
};

class __declspec(dllexport) StatisticsGlobal
{
public:
	StatisticsGlobal();
	virtual ~StatisticsGlobal();
	bool AddObject(char* onm, char* onb, short osx,StatisticsObject** reto=NULL);
	bool AddGroup(char* gnm, short gtp);
	bool AddSubject(char* snm, short stp, long maxsz = ObjectSize, float sfm = 0.f);
	bool DeleteObject(char*& onm, char*& onb, short& osx);
	bool DeleteGroup(char*& gnm, short& gtp);
	bool DeleteSubject(char*& snm, short& stp, long& maxsz);
	bool SearchObject(void* si, short type, short stype);
	bool SearchGroup(void* si, short type);
	bool SearchSubject(void* si, short type);
	bool NextObject(char*& onm, char*& onb, short& osx, float& osc, long& ops);
	void NextGroup(char*& gnm, short gtp);
	void NextSubject(char*& snm, short stp);
	bool NextObject(long times);
	bool NastObject(long times);
	void GetAll(long& sz, float& avg, float& gap, float& rms);
	void GetObject(char*& tnm, char*& tnb, short& tsx, float& tsc, long& tps);
	void GetGroup(char*& gnm, short& gtp, long& gsz, float& gavg, float& ggap, float& grms);
	void GetSubject(char*& snm, short& stp, long& ssz, float& savg, float& sgap, float& srms);
	bool AddToGroup(long gpos, short gtp = AllGroups);
	bool AddToGroup(const char* gnm, long opos = -1);
	bool AddToSubject(long spos, short stp, float sc = -1.f);
	bool AddToSubject(const char* snm, long opos = -1);
	bool DeleteFromGroup(long gpos, short gtp = AllGroups);
	bool DeleteFromGroup(const char* gnm, long opos = -1);
	bool DeleteFromSubject(long spos, short stp, float* sc = NULL);
	bool DeleteFromSubject(const char* snm, long opos = -1);
	void Setscrline(float esc, float bsc, float psc);
	void setscrline(StatisticsSubject* sub, float esc, float bsc, float psc, float fsc);
	void getscrline(StatisticsSubject* sub, float& esc, float& bsc, float& psc, float& fsc);
	bool GoToObject(unsigned long pos, short errorHandling);
	void GetObjectIndex(long& index);
	bool SetObjectIndex(long index, bool forced);
	void GetGroupIndex(long& index);
	bool SetGroupIndex(long index, bool forced);
	void GetSubjectIndex(long& index);
	bool SetSubjectIndex(long index, bool forced);
	void GetPercents(float& epct, float& tpct, float& ppct);
	void getgrouppercents(float& epct, float& tpct, float& ppct);
	void getsubjectpercents(float& fpct, float& epct, float& tpct, float& ppct);
	bool getscoredpercents(float& pct, short pms, StatisticsSubject* sub, StatisticsGroup* grp);
	bool changesubscr(float scr, char* sub = NULL, StatisticsObject* obj = NULL);
	bool changeobjscr(float scr, StatisticsObject* obj);
	bool GetMessage(char**& msg_string, MSGSTRUCT*& msg_array, long& msgcnt, short type);
	void ClearMessage();
	bool SetUncertainScore(short settype, float scr);
	int GetLastError();
	void MarkUncertain();
	void MarkAll();
	void UnmarkAll();
	long GetMarked(long*& m);
	void InverseMark();

	StatisticsObject* objects;
	StatisticsGroup* groups;
	StatisticsSubject* subjects;
	long size;
	float allobjectscr;
	float averagescr;
	float rmsscr;
	float firstobjectscr;
	float lastobjectscr;
	float exellentscr;
	float badscr;
	float passscr;
	float exellentpct;
	float badpct;
	float passpct;
	long objectIndex;
	long groupIndex;
	long subjectIndex;
	int errcode;
	long marked[ObjectSize];
	char* msg_str[MSGSIZE];
	MSGSTRUCT msg_stc[MSGSIZE];

};
