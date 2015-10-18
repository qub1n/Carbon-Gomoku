#include "stdafx.h"

#include <algorithm>

#include "TimeManager.h"

const long maxTimeoutMatch = 2147483647;

TimeManager::TimeManager()
{
	_thisConstructorTime.Start();
	SetTimeoutTurn(1000);
	SetTimeoutMatch(maxTimeoutMatch);
	SetTimeLeft(300000);	
}

bool TimeManager::IsTimeOut()
{
	if( _timeoutMatch == 0)
		return false;
	return _swTimeout.Time() > 0 || _swTimeTurn.Time() > 0;
}

long TimeManager::GetTime()
{
	if( _timeoutMatch == 0)
		return -_swTimeTurn.Time();
	return std::min( -_swTimeout.Time(), -_swTimeTurn.Time());
}

void TimeManager::SetTimeOut(long miliseconds)
{
	_swTimeout.Countdown(miliseconds);
}


long TimeManager::GetRecommendMiliseconds(long &maxMiliseconds, bool firstTimeSearch, long timeLimitRation)
{
	long infoTimeoutTurn = GetTimeoutTurn();
	long infoTimeLeft = GetTimeLeft();
	long infoTimeoutMatch = GetTimeoutMatch();

	if( firstTimeSearch )
	{
		long initTime = _thisConstructorTime.Time();
		if( infoTimeoutMatch - infoTimeLeft > initTime)
			initTime = infoTimeoutMatch - infoTimeLeft;

		if( initTime > infoTimeoutTurn)
			return 0;
		else
			infoTimeoutTurn -= initTime;
	}
		
	const long reserveMiliseconds = 80;
	maxMiliseconds = std::min(infoTimeLeft, infoTimeoutTurn) - reserveMiliseconds;
	if( maxMiliseconds < 0)
		maxMiliseconds = 0;
	long output = (infoTimeLeft / timeLimitRation) - reserveMiliseconds;
	if( output > maxMiliseconds)
		output = maxMiliseconds;
	if( output < 0)
		output = 0;
	return output;
}

long TimeManager::GetTimeoutTurn()
{
	return -_swTimeTurn.Time();
}

long TimeManager::GetTimeoutMatch()
{
	return _timeoutMatch;
}

long TimeManager::GetTimeLeft()
{
	return -_swTimeLeft.Time();
}

void TimeManager::SetTimeoutTurn(long timeoutTurn)
{	
	_timeoutTurn = timeoutTurn;
	ResetTimeTurn();
}

void TimeManager::ResetTimeTurn()
{
	_swTimeTurn.Countdown(_timeoutTurn);		
}

void TimeManager::SetTimeoutMatch(long timeoutMatch)
{	
	_timeoutMatch = timeoutMatch;
}

void TimeManager::SetTimeLeft(long timeLeft)
{
	_swTimeLeft.Countdown(timeLeft);
	if( maxTimeoutMatch == GetTimeoutMatch())
		SetTimeoutMatch(timeLeft);
	ResetTimeTurn();
}