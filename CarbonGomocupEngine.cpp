//----------------------------------------------------------------------------//
//                       ported from Hewer Engine for Gomocup                 //
//                       Tomas Kubes <kubes.tomas@gmail.com>                  //
//----------------------------------------------------------------------------//
//                 Gomocup Wrapper around a Carbon Gomocu engine             //
//----------------------------------------------------------------------------//

#include "CarbonGomocupEngine.h"
#include "coord.h"

#include <chrono>

void WriteLog(int value, int searched, int speed)
{
	//std::cout << "MESSAGE val=" << value << " searched=" << searched << " speed=" << speed << std::endl;
}

string ResponseCoords(unsigned int x, unsigned int y)
{
	char buffer[20];
	sprintf(buffer, "%d,%d", x, y);
	return string(buffer);
}

string CarbonGomocupEngine::GetAndPlayBest()
{
	int x = 0;
	int y = 0;

	//TODO replace simple time management by something better
	int branchingFactorEstimation = 5;

	int time = _timeManager.GetTimeoutTurn();

	auto start = std::chrono::high_resolution_clock::now();
	auto timeBefore = start;

	int maxDepth = 30;
	SearchResult searchResult;

	for (int depth = 0; depth < maxDepth; depth++)
	{		
		searchResult =_carbonEngine.yourTurn(x, y, depth, time);

		//cout << "DEBUG depth = " << depth << " [" << x << "," << y << "]" << endl;

		auto timeAfter = std::chrono::high_resolution_clock::now();

		auto durationIterationMiliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeAfter - timeBefore);

		auto durationAllMiliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeAfter - start);		
		
		auto remaining = time - durationAllMiliseconds.count();	

		if (durationAllMiliseconds.count() > time)
			break;					
		
		if (remaining < branchingFactorEstimation * durationIterationMiliseconds.count())
			break;

		timeBefore = timeAfter;

		if (depth < maxDepth- 1)
			_carbonEngine.undo();
	}	

	cout << "DEBUG value = " << searchResult.value << " nodes = " << searchResult.Nodes << endl;

	string response = ResponseCoords(x, y);

	return response;

}

CarbonGomocupEngine::CarbonGomocupEngine()
{	
	_boardSize = 20;	

	_carbonEngine.start(_boardSize);
}


CarbonGomocupEngine::~CarbonGomocupEngine()
{
}

string CarbonGomocupEngine::Turn(unsigned int x, unsigned int y)
{
	_carbonEngine.move(x, y);

	return GetAndPlayBest();
}

string CarbonGomocupEngine::Begin()
{
	return GetAndPlayBest();
}

string CarbonGomocupEngine::Board(list<list<unsigned int> > mix)
{
	//lastField = opponentsField;
	list<unsigned int> line;
	while (mix.size() > 0)
	{
		line = mix.front();
		unsigned int x = line.front();
		line.pop_front();
		unsigned int y = line.front();
		line.pop_front();
		unsigned int fieldType = line.front();
		//AddMove(x, y, fieldType);

		_carbonEngine.move(x, y);

		mix.pop_front();
	}
	return GetAndPlayBest();
	//return "ERROR Board command not Supported";
}

string CarbonGomocupEngine::RectStart(unsigned int width, unsigned int height)
{
	if (width != height)
	{
		return "ERROR RectStart not supported.";
	}
	_boardSize = width;

	_carbonEngine.start(_boardSize);
	return "OK";
}

string CarbonGomocupEngine::TakeBack(unsigned int x, unsigned int y)
{
	return "ERROR TakeBack not supported.";
}

string CarbonGomocupEngine::Play(unsigned int x, unsigned int y)
{
	_carbonEngine.move(x, y);
	return "OK";
}

string CarbonGomocupEngine::Restart()
{

	_carbonEngine.start(_boardSize);

	return "OK";
}