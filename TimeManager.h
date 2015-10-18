//----------------------------------------------------------------------------//
//                       ported from Hewer Engine for Gomocup                 //
//                       Tomas Kubes <kubes.tomas@gmail.com>                  //
//----------------------------------------------------------------------------//
//    This module is used to compute how much time can be spent by engine     //
//----------------------------------------------------------------------------//

#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H
#include "stdafx.h"


#include "stopwatch.h"

class TimeManager
{
	//casy jsou v milisekundach
	long _timeoutTurn;
	long _timeoutMatch;
	StopWatch _swTimeLeft;
	StopWatch _swTimeTurn;
	StopWatch _swTimeout;
	StopWatch _thisConstructorTime;

	void ResetTimeTurn();
protected:
	long GetTimeoutMatch();
	long GetTimeLeft();
public:
	TimeManager();
	long GetTimeoutTurn();
	bool IsTimeOut();
	void SetTimeOut(long miliseconds);	
	long GetTime();

	void SetTimeoutTurn(long timeoutTurn);
	void SetTimeoutMatch(long timeoutMatch);
	void SetTimeLeft(long timeLeft);

	//vraci doporuceny cas k premysleni
	//odkazem vraci cas, ktery se urcite nesmi prekrocit
	//vracena cisla mohou byt i zaporna
	//poprve se pripocitava cas inicializace
	long GetRecommendMiliseconds(long &maxMiliseconds, bool firstTimeSearch, long timeLimitRation = 45);
};

#endif //TIMEMANAGER_H