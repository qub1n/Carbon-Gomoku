#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <math.h>
#include "coord.h"
#include "linux.h"

#ifndef _MSC_VER //snprintf je linuxova verze sprintf_s    
  #include <cstdio>    
#endif

unsigned int Coord::width;
unsigned int Coord::height;
unsigned int Coord::count;
char* Coord::distance;

Coord::Direction* Coord::direction;
int Coord::directionShift[directionCount];
Crd* Coord::neighbourList;
Crd* Coord::influencyList;
Crd Coord::intersectionBuffer[5];

void Coord::Alloc()
{
	distance = NULL;
	direction = NULL;
	neighbourList = NULL;
	influencyList = NULL;
	Set(defaultX, defaultY);
}

void Coord::Alloc(unsigned int x,unsigned int y)
{
	distance = NULL;
	direction = NULL;
	neighbourList = NULL;
	influencyList = NULL;
	Set(x, y);
}

void Coord::Dealloc()
{
	if( distance != NULL)
	{
		delete[] distance;
	}	
	if( direction != NULL)
	{
		delete[] direction;
	}
	if(	neighbourList != NULL)
	{
		delete[] neighbourList;
	}
	if( influencyList != NULL)
	{
		delete[] influencyList;
	}
}

void Coord::Set(unsigned int width, unsigned int height)
{
	Coord::width = width;
	Coord::height = height;
	Coord::count = width * height;
	unsigned int maxSize = static_cast<unsigned int>( pow(2.0,static_cast<double>(8*sizeof(char))) / 2 - 1);
	if( std::max(width,height) > maxSize)
	{
		char str[256];
		sprintf_s(str,sizeof(str),"Playing board too big, max size is %u",maxSize);
		throw _Exception(str);
	}

	Dealloc();
	distance = new char[count * count];
	direction = new Direction[count * count];
	neighbourList = new Crd[count * (MAX_NEIGHBOURS + 1)];
	influencyList = new Crd[count* directionCount  *(MAX_INFLUENCES_DIRECTION  + 1)];
	SetDistanceDirection();
	SetDirectionShift();
	SetNeighbourList();
	SetInfluencyList();
}

void Coord::SetInfluencyList()
{
	for(Crd coord = 0; coord < GetCount(); coord++)
	{
		for( int dir = 0; dir < directionCount; dir++)
		{
			Crd *head = GetInfluencyList(coord, static_cast<Coord::Direction>(dir));
			int count = 0;
			for(Crd coord2 = 0; coord2 < GetCount(); coord2++)
			{
				if( GetInfluency(coord, coord2) && GetDirection(coord, coord2) == static_cast<Coord::Direction>(dir) )
				{
					*head = coord2;
					head++;
					count++;
					_ASSERTE(count <= MAX_INFLUENCES_DIRECTION);
				}
			}
			*head = GetBadCoord();
		}
	}
}

void Coord::SetNeighbourList()
{
    //unsigned int step = MAX_NEIGHBOURS + 1;
	for(Crd coord = 0; coord < GetCount(); coord++)
	{
		Crd *head = GetNeighbourList(coord);
		int count = 0;
		for(Crd neighbour = 0; neighbour < GetCount(); neighbour++)
		{
			//policko neni samo sobe sousedem
			int dist = GetDistance(coord, neighbour);
			if( abs(dist) == 1 )
			{
				*head = neighbour;
				head++;
				//neighbourList[coord * step + count] = neighbour;
				count++;
			}
		}
		*head = GetBadCoord();
		if( count < 3)
			throw _Exception("Every coord must have at least 3 neighbours.");
		//neighbourList[coord * step + count]	= GetBadCoord();//oznacuje konec seznamu
	}
}


void Coord::SetDirectionShift()
{
	directionShift[0] = 1;
	directionShift[1] = width;
	directionShift[2] = width + 1;
	directionShift[3] = width - 1;

}

//vrati souradnici, ktera se nachazi ve vzdalenosti distance smerem direction od souradnici origin
//pokud je toto pole mimo plochu, vraci END_LIST
Crd Coord::GetCoordAt(Crd origin, int distance, Direction direction)
{
	int newCoord = directionShift[direction] * distance + origin;

	if( newCoord < 0 || newCoord >= (int)GetCount() )
	{
		return END_LIST;
	}
	else if(GetDirection(origin, newCoord) != direction )
	{
		return END_LIST;
	}
	return (unsigned int)newCoord;
}

void Coord::SetDistanceDirection()
{
	Direction dir;
	char dis;
	unsigned int x1,x2,y1,y2;
	Crd z1,z2,tmp;
	bool swaped;
	unsigned int i,j;

	for(i = 0; i < count; i++ )
	{
		for(j = 0; j < count; j++ )
		{
			if( i == j )
			{
				dir = drIdentical;
				dis = 0;
			}
			else
			{
				z1 = i;
				z2 = j;
				z2xy(x1,y1,z1);
				z2xy(x2,y2,z2);
				
				swaped = false;
				if(z2 < z1)
				{
					tmp = z1;
					z1 = z2;
					z2 = tmp;
					swaped = true;
				}
				if(x1 == x2){
					dir = drVertical;
					dis = swaped ? y1 - y2 : y2 - y1;
				}else if(y1 == y2){
					dir = drHorizontal;
					dis =  swaped ? x1 - x2 : x2 - x1;
				}else if(y2 - y1 == x2 - x1){
					dir = drQuadrant13;
					dis =  swaped ? x1 - x2 : x2 - x1;
				}else if(x1 + y1 == x2 + y2){
					dir = drQuadrant24;
					dis =  swaped ? y1 - y2 : y2 - y1;
				}else{
					dir = drNone;
					dis = CHAR_MAX;
				}
			}

			direction[i * count + j] = dir;
			direction[j * count + i] = dir;

			if( i > j && dis != CHAR_MAX)
			{
				dis = -dis;
			}
		
			distance[i * count + j] = dis;
			if(dis != CHAR_MAX)
			{
				distance[j * count + i] = -dis;
			}
			else
			{
				distance[j * count + i] = dis;
			}
		}
	}	
}

Crd* Coord::GenerateInterSections(Crd z1, Crd z2, int& count)
{	
	count = 0;

	if(z1 != z2)
	{
		unsigned int x1, y1, x2, y2;
		z2xy(x1,y1,z1);
		z2xy(x2,y2,z2);
		int difY = abs((int)y1 - (int)y2);
		int difX = abs((int)x1 - (int)x2);

		if( x1 == x2 && difY < 5) //jsou ve stejnem sloupci az 4 vysledky
		{
			//doprava 2
			if( (int)x1 + difY < (int)Coord::GetWidth() )
			{
				intersectionBuffer[count++] = xy2z(x1 + difY,y1);
				intersectionBuffer[count++] = xy2z(x1 + difY,y2);
			}
			//doleva 2
			if( (int)x1 - difY >= 0 )
			{
				intersectionBuffer[count++] = xy2z(x1 - difY,y1);
				intersectionBuffer[count++] = xy2z(x1 - difY,y2);
			}
		}
		else if(y1 == y2 && difX < 5) // jsou ve stejne rade az 4 vysledky
		{
			//nahoru 2
			if( (int)y1 + difX < (int)Coord::GetHeight() )
			{
				intersectionBuffer[count++] = xy2z(x1, y1 + difX);
				intersectionBuffer[count++] = xy2z(x2, y2 + difX);
			}
			//dolu 2
			if( (int)y1 - difX >= 0 )
			{
				intersectionBuffer[count++] = xy2z(x1, y1 - difX);
				intersectionBuffer[count++] = xy2z(x2, y2 - difX);
			}
		}
		else //az dva vysledky
		{
			if( difX < 5 && difY < 5)
			{
				intersectionBuffer[count++] = xy2z(x1,y2);
				intersectionBuffer[count++] = xy2z(x2,y1);
			}
		}
	}

	intersectionBuffer[count] = Coord::GetBadCoord();

	if( count == 0)
		return NULL;
	else
		return intersectionBuffer;
}