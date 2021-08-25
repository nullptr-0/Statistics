// StatisticsGroup.h: interface for the StatisticsGroup class.
//
//////////////////////////////////////////////////////////////////////

class StatisticsGroup  
{
public:
	short type;
	short existst;
	long size;
	char* name;
	float firstobjectscr;
	float lastobjectscr;
	float allobjectscr;
	float averagescr;
	float rmsscr;
	float exellentpct;
	float passpct;
	float badpct;
	StatisticsGroup();
	virtual ~StatisticsGroup();
};
