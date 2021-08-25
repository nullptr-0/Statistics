// Statisticsgroup.h: interface for the Statisticsgroup class.
//
//////////////////////////////////////////////////////////////////////

class Statisticsgroup  
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
	Statisticsgroup();
	virtual ~Statisticsgroup();
};
