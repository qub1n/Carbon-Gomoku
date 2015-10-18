#include "GomocupInterface.h"

#include <iostream>

GomocupInterface::GomocupInterface()
{
}


GomocupInterface::~GomocupInterface()
{
}


void WriteLog(int value, int searched, int speed)
{
	std::cout << "MESSAGE val=" << value << " searched=" << searched << " speed=" << speed << std::endl;
}