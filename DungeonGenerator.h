#pragma once
#include <string>
#include <vector>
#include "Const.h"
#include <DirectXMath.h>

using namespace DirectX;

class Floor {
public:
	Floor();
	~Floor();
	std::vector<std::vector<Const::Cell::Type>> data;
};

class Room {
public:
	void set(int x, int y, int w, int h);
	int x;
	int y;
	int w;
	int h;
};

class Aisle {
public:
	Aisle();
	~Aisle();
	void set(int north, int south, int west, int east);
	int north;
	int south;
	int west;
	int east;
};

class Area {
public:
	Area();
	~Area();
	void set(int x, int y, int w, int h, int id);
	int x;
	int y;
	int w;
	int h;
	int id;
	Room* room;
	Aisle* aisle;
};

class DungeonGenerator {
public:
	DungeonGenerator();
	~DungeonGenerator();
	void createDg();
	void draw();
	void splitArea(Area* area, int id, bool isVertical);
	void createRoom();
	void connectRooms();

	XMFLOAT2 getRandomPosInRoom();

	XMFLOAT2 getRandomPosInRoomItem();

	int GetCurrentRoom(XMINT2 pos);

	void setFloor();

	void SetCellType(int x, int y, Const::Cell::Type cellType);

	Floor* getFloor() { return mFloor; }
	Const::Cell::Type getCellType(int x, int y) { return mFloor->data[y][x]; }

	class Random* GetRandom() { return mRand; }

private:
	Floor* mFloor;
	std::vector<Area*> mAreas;
	class Random* mRand;
};