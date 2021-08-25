// Statisticsinfos.h: interface for the Statisticsinfos class.
//
//////////////////////////////////////////////////////////////////////

struct MSGSTRUCT
{
	Statisticsinfo* objsrc;
	Statisticsgroup* grpsrc;
	Statisticssubject* subsrc;
	int msgflag;
};

struct SUB_SCR
{
	Statisticssubject* sub;
	float scr;
};

class __declspec(dllexport) Statisticsinfos
{
public:
	Statisticsinfos();
	virtual ~Statisticsinfos();
	bool inobject(char* onm, char* onb, short osx,Statisticsinfo** reto=NULL);
	bool ingroup(char* gnm, short gtp);
	bool insubject(char* snm, short stp, long maxsz = OBJECTSIZE, float sfm = 0.f);
	bool outobject(char*& onm, char*& onb, short& osx);
	bool outgroup(char*& gnm, short& gtp);
	bool outsubject(char*& snm, short& stp, long& maxsz);
	bool searchobject(void* si, short type, short stype);
	bool searchgroup(void* si, short type);
	bool searchsubject(void* si, short type);
	void nextobject(char*& onm, char*& onb, short& osx, float& osc, long& ops);
	void nextgroup(char*& gnm, short gtp);
	void nextsubject(char*& snm, short stp);
	bool nextobject(long times);
	bool lastobject(long times);
	void getall(long& sz, float& avg, float& gap, float& rms);
	void getobject(char*& tnm, char*& tnb, short& tsx, float& tsc, long& tps);
	void getgroup(char*& gnm, short& gtp, long& gsz, float& gavg, float& ggap, float& grms);
	void getsubject(char*& snm, short& stp, long& ssz, float& savg, float& sgap, float& srms);
	bool addtogroup(long gpos, short gtp = ALLGROUPS);
	bool addtogroup(const char* gnm, long opos = -1);
	bool addtosubject(long spos, short stp, float sc = -1.f);
	bool addtosubject(const char* snm, long opos = -1);
	bool delfromgroup(long gpos, short gtp = ALLGROUPS);
	bool delfromgroup(const char* gnm, long opos = -1);
	bool delfromsubject(long spos, short stp, float* sc = NULL);
	bool delfromsubject(const char* snm, long opos = -1);
	void setscrline(float esc, float bsc, float psc);
	void setscrline(Statisticssubject* sub, float esc, float bsc, float psc, float fsc);
	void getscrline(Statisticssubject* sub, float& esc, float& bsc, float& psc, float& fsc);
	bool gotoobject(unsigned long pos, short errpolicy);
	void gotoobject(long& pos, bool status);
	void gotogroup(long& pos, bool status);
	void gotosubject(long& pos, bool status);
	void getpercents(float& epct, float& tpct, float& ppct);
	void getgrouppercents(float& epct, float& tpct, float& ppct);
	void getsubjectpercents(float& fpct, float& epct, float& tpct, float& ppct);
	bool getscoredpercents(float& pct, short pms, Statisticssubject* sub, Statisticsgroup* grp);
	bool changesubscr(float scr, char* sub = NULL, Statisticsinfo* obj = NULL);
	bool changeobjscr(float scr, Statisticsinfo* obj);
	bool getmsg(char**& msg_string, MSGSTRUCT*& msg_array, long& msgcnt, short type);
	void clearmsg();
	bool setuncertainscore(short settype, float scr);
	int getlasterror();
	void markuncertain();
	void markall();
	void unmarkall();
	long getmarked(long*& m);
	void inversemark();

	Statisticsinfo* objects;
	Statisticsgroup* groups;
	Statisticssubject* subjects;
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
	long objectposition;
	long groupposition;
	long subjectposition;
	int errcode;
	long marked[OBJECTSIZE];
	char* msg_str[MSGSIZE];
	MSGSTRUCT msg_stc[MSGSIZE];

};
