//----------------------------------------------------------------------------//
//                       ported from Hewer Engine for Gomocup                 //
//                       Tomas Kubes <kubes.tomas@gmail.com>                  //
//----------------------------------------------------------------------------//
//    This module is Stopwatch implementation. TODO replace by chrono         //
//----------------------------------------------------------------------------//

#include "stdafx.h"
#include "stopwatch.h"
#include <ostream>
using namespace std;

//---------------------------------------------------------------------------
void StopWatch::Start(){
    ::ftime(&timer);
    starttime = (unsigned long)(timer.time*1000 + timer.millitm);
}

void StopWatch::Countdown(unsigned long limit){
	Start();
    starttime +=  limit;
}
//---------------------------------------------------------------------------
long StopWatch::Time(){
    ::ftime(&timer);
    stoptime = (unsigned long)(timer.time*1000 + timer.millitm);
    return (long)stoptime - (long)starttime;
}
//---------------------------------------------------------------------------
unsigned long StopWatch::Pause(){
    stoptime = (unsigned long)(timer.time*1000 + timer.millitm);
    return stoptime;
}
//---------------------------------------------------------------------------
void StopWatch::Continue(){
    ::ftime(&timer);
    starttime = (unsigned long)(timer.time*1000 + timer.millitm - stoptime);
}
