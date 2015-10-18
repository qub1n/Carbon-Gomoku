//----------------------------------------------------------------------------//
//                       ported from Hewer Engine for Gomocup                 //
//                       Tomas Kubes <kubes.tomas@gmail.com>                  //
//----------------------------------------------------------------------------//
//               Base class for comunication in Gomocup format                //
//----------------------------------------------------------------------------//

#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

//#include "common.h"

#include <string>
#include <sstream>
#include <list>
#include <fstream>
using namespace std;

enum GomokuRule{ rlFreeStyle = 0, rlStandard = 1, rlFreeStyleContinual = 2, rlStandardContinual = 3 };

#define CPPWIN32DLL_API
#define PRODUCT_VER_STRING "2.1"
#define GIT_BRANCH_STR "N/A"
#define GIT_SHA_STR "N/A"
#define DEFAULT_RULE rlFreeStyle
#define _Exception std::exception

class Communicator;

#include "TimeManager.h"
//#include "OutflankBoard.h"

#define MAX_LINE_COMMAND_LENGTH 256

//implements gomocup protocol
class CPPWIN32DLL_API Communicator
{
	bool _run;
	bool _boardCommand;
	string line;
    
#ifdef LOGFILES
	ofstream logout;
#endif

	enum GameType{ gtHuman = 0, gtAI = 1, gtTournament = 2, gtNetworkTournament = 3};
public:
	enum AsciArtType{ aaSmile};
protected:
    stringstream _debugAndMessageBuffer;
	int infoMaxMemory;
	GameType infoGameType;
	GomokuRule infoRule;

#if defined LOGFILES|| defined(PARAMEXPORT)
	string folder;
	string exeName;
#endif

private:
#pragma warning(suppress : 4251)
	list<list<unsigned int> > boardData;

	//vraci true, pokud sezrar prikaz
	void ProcessLine();	
	void PutLine(string out);
	void WaitForInterrupt();
	void StartCriticalSection();
	void EndCriticalSection();
protected:
	TimeManager _timeManager;
#if defined LOGFILES || defined(PARAMEXPORT)
	void CreateWorkingFolder(string workDir);
#endif

#ifdef PARAMEXPORT
	std::ofstream _paramExportStream;
	void SaveParam(OutFlankBoard* board, int depth, int value);
#endif

public:	
	Communicator();
	~Communicator();

	string ProcessLine(string commandline);

#ifdef LOGFILES
	string GetFullPath(const char* filename);
#endif
	void Run();
	
	//commands to brain
	void Info(string key, string value);
	virtual string Turn(unsigned int x, unsigned int y) = 0;
	virtual string Begin() = 0;
	virtual string Board(list<list<unsigned int> > mix) = 0;
	virtual string RectStart(unsigned int width, unsigned int height) = 0;
	virtual string Restart() = 0;
	virtual string TakeBack(unsigned int x,unsigned int y) = 0;
	virtual string Play(unsigned int x, unsigned int y) = 0;
	virtual void OnMaxMemoryChanged(int bytes) = 0;
	virtual string GetAndPlayBest(void) = 0;

	//vraci odpoved, pokud byl prikaz zpracovan, jinak prazdny string
	virtual string ProcessCommand(string command, string arguments); 

	virtual void OnRuleChange(GomokuRule newRule);	

	//command from brain
	string Error(string message);
	void Message(string message);
	void Debug(string message);
	string Suggest(unsigned int x, unsigned int y);
	string OK();
	string ResponseCoords(unsigned int x, unsigned int y);
};

#endif //COMMUNICATOR_H