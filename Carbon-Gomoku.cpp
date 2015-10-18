// Carbon-Gomoku.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CarbonGomocupEngine.h"


int _tmain(int argc, _TCHAR* argv[])
{
	int depth = 9;

	CarbonGomocupEngine engine(depth);
	engine.Run();
	return 0;
}

