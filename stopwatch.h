#ifndef STOPWATCH_H
#define STOPWATCH_H
#include "stdafx.h"


#include <time.h>
#include <sys/timeb.h>

#include <iostream>
using namespace std;

//---------------------------------------------------------------------------
class StopWatch
{
    timeb timer;
	//cas v milisekundach
    unsigned long starttime;
    unsigned long  stoptime;
public:
     void Start();//spusti stopky
	void Countdown(unsigned long limit);//zacne odpocitavat od zaporneho cisla k nule
    long Time();//vraci mezicas v milisekundach a meri dal
    unsigned long Pause();
    void Continue();
    //static unsigned long Now();
	//static ostream& NowReadable(ostream & out);
};

#endif //STOPWATCH_H