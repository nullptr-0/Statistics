// Statisticssubject.h: interface for the Statisticssubject class.
//
//////////////////////////////////////////////////////////////////////

class __declspec(dllexport) Statisticssubject
{
public:
	char* name;
	short type;
	short existst;
	long size;
	long maxsize;
	float firstobjectscr;
	float lastobjectscr;
	float allobjectscr;
	float averagescr;
	float rmsscr;
	float fullscr;
	float exellentscr;
	float passscr;
	float badscr;
	float fullpct;
	float exellentpct;
	float passpct;
	float badpct;
	Statisticssubject();
	virtual ~Statisticssubject();
	void setscrline(float full, float exellent, float pass, float bad);
	void setmaxsize(long max);
};
