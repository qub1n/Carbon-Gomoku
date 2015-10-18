#ifndef COORD_H
#define COORD_H

#include <vector>
using namespace std;

class Coord;

#include <limits.h>
#include <cstdlib>

typedef unsigned short Crd;
#define directionCount 4

//coordinate
class Coord
{
	static Crd intersectionBuffer[5];
public:
	//relative line position of coordinates
	enum Direction{drHorizontal = 0, drVertical = 1, drQuadrant13 = 2, drQuadrant24 = 3, drNone = 4, drIdentical = 5, drNoDirection = 6};
	enum Corner{cnNone = 0, cnLeftTop = 1, cnRightTop = 2, cnLeftBottom = 3, cnRightBottom = 4};
	static const Crd END_LIST = USHRT_MAX;
	static const Crd SWAP_MOVE = USHRT_MAX - 1; //tah, ktery nepoklada kamen, ale prohazuje pozice protihracu

private:
	static unsigned int width;
	static unsigned int height;
	static unsigned int count; // width * height
	static char* distance;	

	//pole, kde jsou ulozeny smery pro dvojcice souradnic, indexovane pomoci i * count + j
	//pokud souradnice nelezi v jedne lajne, je tam ulozeno CHAR_MAX
	static Direction* direction;

	//maximalni pocet sousedu policka hraci plochy
	static const unsigned int MAX_NEIGHBOURS = 8;
	//maximalni pocet policek, ktere mohou byt v jednom smeru ovlivneny
	static const unsigned int MAX_INFLUENCES_DIRECTION = 8;
	//GetBadCoord() oznacuje konec seznamu
	static Crd* neighbourList;
	//GetBadCoord() oznacuje konec seznamu
	static Crd* influencyList;

	//posun na dalsi souradnici v danem smeru, pro plochu veliosti 20 je to napr. +1,+20,+19,+21
	static int directionShift[directionCount];

	static const unsigned int defaultX = 20;
	static const unsigned int defaultY = 20;
	static void SetDistanceDirection();
	static void SetDirectionShift();
	static void SetNeighbourList();
	static void SetInfluencyList();
public:
	//vrati ukazatel na pole sousedu
	inline static Crd* GetNeighbourList(Crd coord)
	{
		_ASSERTE(IsValid(coord));
		return neighbourList + (MAX_NEIGHBOURS + 1) * coord;
	}

	static bool IsMargin(Crd coord)
	{
		_ASSERTE(IsValid(coord));
		unsigned int x, y;
		z2xy(x,y,coord);
		return x == 0 || y == 0 || x == Coord::GetWidth()-1 || y == Coord::GetHeight() - 1;
	}

	static Corner GetCorner(Crd coord)
	{
		_ASSERTE(IsValid(coord));
		
		unsigned int x, y;
		z2xy(x,y,coord);
		const int cornerSize = 4;
		if( x < cornerSize)
		{
			if( y < cornerSize)
				return cnLeftTop;
			else if( y >= GetHeight() - cornerSize)
				return cnRightTop;
		}
		if( x >= GetWidth() - cornerSize)
		{
			if( y < cornerSize)
				return cnLeftBottom;
			else if( y >= GetHeight() - cornerSize)
				return cnRightBottom;
		}
		return cnNone;	
	}

	//vraci ukazatel na pole policek, ktere se daji v onom smeru ovlivnit, jsou do vzdalenosti 5
	//list je ukoncen hodnotou Coord::GetBadCoord()
	inline static Crd* GetInfluencyList(Crd coord, Coord::Direction direction)
	{
		_ASSERTE(IsValid(coord));
		return influencyList + (MAX_INFLUENCES_DIRECTION  + 1) * (coord * directionCount + direction);
	}

	inline static int GetDirectionShift(Direction direction)
	{
#ifdef DEBUG
	if( direction < 0 || direction > directionCount )
		throw _Exception("GetDirectionShift index overflow.");
#endif
		return directionShift[direction];
	}
	inline static Crd GetBadCoord(){return count;}	

	static void Set(unsigned int width, unsigned int height);

	inline static unsigned int GetWidth(){return width;}
	inline static unsigned int GetHeight(){return height;}

	//number of all fields on the board
	inline static unsigned int GetCount(){return count;}

	static void Alloc();
	static void Alloc(unsigned int x,unsigned int y);
	static void Dealloc();


	inline static char* GetDistancePointer(Crd z1)
	{
		return distance + z1*count;
	}

	//if coordinates z1 and z2 are on the same line, it returns distance, otherwise END_LIST
	inline static char GetDistance(Crd z1, Crd z2)
	{
		_ASSERTE(IsValid(z1));
		_ASSERTE(IsValid(z2));
		return distance[z1*count + z2];
	}

	inline static char GetAbsDistance(Crd z1, Crd z2)
	{
		_ASSERTE(IsValid(z1));
		_ASSERTE(IsValid(z2));
		return abs(distance[z1*count + z2]);
	}

	//return relative line position of z1 and z2
	inline static Direction GetDirection(Crd z1, Crd z2)
	{
		_ASSERTE(IsValid(z1));
		_ASSERTE(IsValid(z2));
		return direction[z1*count + z2];
	}


	bool static GetInfluency(Crd z1, Crd z2)
	{
		return GetAbsDistance(z1, z2) < 5;
	}

	bool static GetInfluencyEdge(Crd z1, Crd z2)
	{
		return GetAbsDistance(z1, z2) <= 5;
	}

	bool static GetInfluencyLong(Crd z1, Crd z2)
	{
		return GetAbsDistance(z1, z2) <= 6;
	}

	inline static bool IsValid(Crd z)
	{
		return z < count;
	}

	inline static void CheckValid(Crd z)
	{
		if ( z >= count )
			throw _Exception("Invalid Coord");
	}

	inline static void CheckValidDirection(Direction direction)
	{
		if ( direction >= directionCount || direction < 0)
			throw _Exception("Invalid Direction");
	}
	inline static Crd xy2z(unsigned int x,unsigned int y)
	{
		//#ifdef DEBUG
			if( x >= width )
				throw _Exception("X coord overflow.");
			if( y >= height )
				throw _Exception("Y coord overflow.");
		//#endif

		return x  + y * width;
	}

	inline static void z2xy(unsigned int &x, unsigned int &y, Crd z)
	{
		#ifdef DEBUG
			if( z >= count )
				throw _Exception("Z coord overflow.");
		#endif
		x = z % width;
		y = z / width;
	}

	//vrati souradnici, ktera se nachazi ve vzdalenosti distance smerem direction od souradnici origin
	//pokud je toto pole mimo plochu, vraci END_LIST
	static Crd GetCoordAt(Crd origin, int distance, Direction direction);

	//vraci 0-4 souradnice, ktere jsou ovlivneny dvojici souradnic z1 a z2
	static Crd* GenerateInterSections(Crd z1, Crd z2,int& count); //neni reentrantni
};




#endif //COORD_H