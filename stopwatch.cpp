#include "stdafx.h"

#include "stopwatch.h"
#include <ostream>
using namespace std;

//---------------------------------------------------------------------------
/*unsigned long StopWatch::Now(){
    timeb timer2;
    ::ftime(&timer2);
    unsigned long miliseconds = (unsigned long)(timer2.time*1000 + timer2.millitm);
    return miliseconds;
}*/

/*ostream& StopWatch::NowReadable(ostream & out){
   char buff[256];
   _strtime(buff);   
   out << buff;
   return out;
}*/
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
