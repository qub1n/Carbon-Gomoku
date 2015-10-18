#include "communicator.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

#ifdef WIN_OS
#include <io.h>   // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <windows.h>
#endif



using namespace std;


int StringToInt(const string& str)
{
	int i;
	if (sscanf(str.c_str(), "%d", &i) != 1)
	{
		throw _Exception("StringToInt Conversion failed.");
	}
	return i;
}
Communicator::Communicator()
{
	_boardCommand = false;
	_run = true;
}

Communicator::~Communicator()
{
}


void Communicator::OnRuleChange(GomokuRule newRule)
{	
	//cout << "DEBUG changing lookup tables finished." << endl;
}


void Communicator::Run()
{
	istream *in;
	ifstream fin;

	bool fileInput = false;
	in = &cin;

	while (_run)
	{
		line.clear();
		if(in->eof() )
		{
			//end of file, switch to keyboard
			in = &cin;
		}
		getline(*in, line);

		if( fileInput )
		{
			cout << line << endl;
		}
		for(unsigned int i = 0; i < line.length(); i++)
		{
			line[i] = tolower(line[i]);
		}
		//TODO record in
		ProcessLine();			
	}
}

void Communicator::Info(string key, string value)
{
	if( key == "timeout_turn")
	{
		_timeManager.SetTimeoutTurn(StringToInt(value));
	}
	else if( key == "timeout_match")
	{
		_timeManager.SetTimeoutMatch(StringToInt(value));
	}
	else if( key == "max_memory")
	{
		infoMaxMemory = StringToInt(value);
		OnMaxMemoryChanged(infoMaxMemory);
	}
	else if( key == "time_left")
	{
		_timeManager.SetTimeLeft(StringToInt(value));
	}
	else if( key == "game_type")
	{
		infoGameType = (GameType)StringToInt(value);
	}
	else if( key == "rule")
	{
		GomokuRule newRule = (GomokuRule)StringToInt(value);
		if( newRule  != infoRule)
		{
			infoRule = newRule;
			OnRuleChange(infoRule);
		}
	}
}

void Communicator::PutLine(string out)
{
	cout << out << endl;
}

void Communicator::ProcessLine()
{	
	string response = ProcessLine(line);
	if(response.length() > 0)	
		PutLine(response/* + "\n"*/);
}

string Communicator::ProcessLine(string commandline)
{	
	string command;
	string arguments;

    if(commandline.length() == 0)
        return string();
    
	size_t pos = commandline.find_first_of(' ');

	if( pos == string::npos ) //no space
	{
		command = commandline;
	}
	else
	{
		command = commandline.substr(0, pos);
		arguments = commandline.substr(pos + 1);
	}
	string response = ProcessCommand(command, arguments);	
    if( _debugAndMessageBuffer.str().length() > 0)
    {		
        response.insert(0, _debugAndMessageBuffer.str() + "\n");
        _debugAndMessageBuffer.str("");
        _debugAndMessageBuffer.clear();
    }
    
    if( response.length() > 0)    
        return response;
    else
        return string();
}

string Communicator::ProcessCommand(string command, string arguments)
{
	char buffer1[MAX_LINE_COMMAND_LENGTH + 1];
	char buffer2[MAX_LINE_COMMAND_LENGTH + 1];
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	//process commands
	if( command.length() == 0 )
	{
		return string();
	}
	else if( _boardCommand )
	{
		if( command == "done")
        {
			_boardCommand = false;
			StartCriticalSection();
			string ret = Board(boardData);
			EndCriticalSection();
            return ret;
		}
		else
		{
			sscanf(command.c_str(),"%u,%u,%u",&x,&y,&z);
			list<unsigned int> move = list<unsigned int>();
			move.push_back(x);
			move.push_back(y);
			move.push_back(z);
			boardData.push_back(move);
			return string();
		}
	}
	else if( command == "board")
	{
		_boardCommand = true;
		while( ! boardData.empty() )
		{
			boardData.pop_front();
		}
		return string();
	}
	else if( command == "end")
	{
		_run = false;
		return string();
	}
	else if(command == "about")
	{
		return string("name=\"Carbon Gomoku\", version=\"") + string(PRODUCT_VER_STRING) +
			string("\", build=\"") + string(GIT_BRANCH_STR) + string("_") + string(GIT_SHA_STR) + string("\", author=\"Tomas Kubes\", email=\"kubes.tomas@gmail.com\", country=\"Czech Republic\", builded=\"") + string(__DATE__) + " " + string(__TIME__) + string("\"");
	}
	else if(command == "turn")
	{
		sscanf(arguments.c_str(),"%u,%u",&x,&y);
		StartCriticalSection();
		return Turn(x, y);
		EndCriticalSection();
	}
	else if(command == "begin")
	{
		StartCriticalSection();
		return Begin();
		EndCriticalSection();
	}
	else if(command == "play")
	{
		sscanf(arguments.c_str(),"%u,%u",&x,&y);
		StartCriticalSection();
		return Play(x, y);
		EndCriticalSection();
	}
	else if(command == "takeback")
	{
		sscanf(arguments.c_str(),"%u,%u",&x,&y);
		StartCriticalSection();
		return TakeBack(x, y);
		EndCriticalSection();
	}	
	else if(command == "info")
	{
		string::size_type loc = arguments.find(' ', 0 );
		if( loc != string::npos ) 
		{
			string str1 = arguments.substr(0, loc);
			string str2 = arguments.substr(loc + 1);
			strcpy_s(buffer1,MAX_LINE_COMMAND_LENGTH, str1.c_str() );
			strcpy_s(buffer2,MAX_LINE_COMMAND_LENGTH, str2.c_str() );
			Info(buffer1, buffer2);
			return string();
		} 
		else
		{
			return string("ERROR uknown info format.");			
		}	
	}	
	else if(command == "start")
	{
		WaitForInterrupt();
		sscanf(arguments.c_str(),"%u",&x);	
		StartCriticalSection();
		return RectStart(x,x);
		EndCriticalSection();
	}
	else if(command == "rectstart")
	{		
		WaitForInterrupt();
		sscanf(arguments.c_str(),"%u,%u",&x,&y);
		StartCriticalSection();
		return RectStart(x,y);
		EndCriticalSection();
	}	
	else if(command == "restart")
	{
		WaitForInterrupt();
		StartCriticalSection();
		return Restart();
		EndCriticalSection();
	}
	else
		return string("UKNOWN");	
}

void Communicator::WaitForInterrupt()
{
	
}

void Communicator::StartCriticalSection()
{
	//TODO
}

void Communicator::EndCriticalSection()
{
	//TODO
}

string Communicator::Error(string message)
{
	//PutLine("ERROR " + message);
	return "ERROR " + message;

}

void Communicator::Message(string message)
{
	_debugAndMessageBuffer << "MESSAGE " + message << endl;
}

void Communicator::Debug(string message)
{
	_debugAndMessageBuffer << "DEBUG " + message << endl;
}

string Communicator::Suggest(unsigned int x, unsigned int y)
{	
	char buffer[20];
	sprintf(buffer,"SUGGEST %d,%d", x, y);
	return string(buffer);
}

string Communicator::OK()
{
	return string("OK");
}

string Communicator::ResponseCoords(unsigned int x, unsigned int y)
{
	char buffer[20];
	sprintf(buffer,"%d,%d", x, y);
	return string(buffer);
}