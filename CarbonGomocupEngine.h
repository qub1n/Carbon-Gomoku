#pragma once

#include "communicator.h"
#include "AICarbon.h"

using namespace std;


class CarbonGomocupEngine : public Communicator
{
	AICarbon _carbonEngine;
	int _depth;	
	int _boardSize;

	string GetAndPlayBest(void);
public:
	CarbonGomocupEngine(int depth);
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

