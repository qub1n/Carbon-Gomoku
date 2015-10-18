#include "CarbonGomocupEngine.h"
#include "coord.h"

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

	int time = _timeManager.GetTimeoutTurn();

	_carbonEngine.yourTurn(x, y, _depth, time);

	string response = ResponseCoords(x, y);

	return response;

}

CarbonGomocupEngine::CarbonGomocupEngine()
{
	_depth = 7;	
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