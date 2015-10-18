//----------------------------------------------------------------------------//
//                       ported from Hewer Engine for Gomocup                 //
//                       Tomas Kubes <kubes.tomas@gmail.com>                  //
//----------------------------------------------------------------------------//
//                 Gomocup Wrapper around a Carbon Gomocu engine              //
//----------------------------------------------------------------------------//


#pragma once

#include "communicator.h"
#include "AICarbon.h"

using namespace std;

void WriteLog(int value, int search, int speed);

class CarbonGomocupEngine : public Communicator
{
	AICarbon _carbonEngine;	
	int _boardSize;

	string GetAndPlayBest(void);
public:
	CarbonGomocupEngine();
	~CarbonGomocupEngine();

	string Turn(unsigned int x, unsigned int y);
	string Begin();
	string Board(list<list<unsigned int> > mix);
	string RectStart(unsigned int width, unsigned int height);
	string TakeBack(unsigned int x, unsigned int y);
	string Play(unsigned int x, unsigned int y);
	string Restart();

	void OnMaxMemoryChanged(int bytes){}
};

