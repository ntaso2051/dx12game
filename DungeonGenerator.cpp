#include "DungeonGenerator.h"
#ifdef _DEBUG
#include <iostream>
#endif
#include "Random.h"

Area::Area() { this->room = new Room(); }
Area::~Area() { delete this->room; }

void Area::set(int x, int y, int w, int h, int id) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->id = id;
}

void Room::set(int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void Aisle::set(int north, int south, int west, int east) {
	this->north = north;
	this->south = south;
	this->west = west;
	this->east = east;
}

Aisle::Aisle() { this->set(-1, -1, -1, -1); }

DungeonGenerator::DungeonGenerator() { rand = new Random(); }

DungeonGenerator::~DungeonGenerator() { delete mFloor; }

void DungeonGenerator::createDg() {
	mFloor = new Floor();

	Area* area = new Area();
	area->set(0, 0, Const::FLOOR_MAX_WIDTH, Const::FLOOR_MAX_HEIGHT, 0);
	mAreas.push_back(area);

	splitArea(mAreas[0], 1, true);

	createRoom();

	setFloor();

	connectRooms();
}

void DungeonGenerator::draw() {
#ifdef _DEBUG
	for (auto& vh : mFloor->data) {
		for (auto& vw : vh) {
			std::cout << vw << " ";
		}
		std::cout << std::endl;
	}
#endif
}

void DungeonGenerator::splitArea(Area * area, int id, bool isVertical) {
	// if (id > 3) return;
	int randmax = isVertical ? area->w : area->h;
	int randmin = isVertical ? Const::AREA_MIN_WIDTH : Const::AREA_MIN_HEIGHT;
	if (id == 1) {
		randmax = isVertical ? area->w / 2 : area->h / 2;
	}

	if (randmax - 2 - (randmin) <= 0) return;

	int p = rand->randInt(randmin, randmax - 2);
	Area * newArea = new Area();
	if (isVertical) {
		newArea->set(area->x + p, area->y, Const::FLOOR_MAX_WIDTH - (p + area->x),
			area->h, id + 1);
		area->set(area->x, area->y, p, area->h, id);
	}
	else {
		newArea->set(area->x, area->y + p, area->w,
			Const::FLOOR_MAX_HEIGHT - (p + area->y), id + 1);
		area->set(area->x, area->y, area->w, p, id);
	}

	if (newArea->h <= Const::AREA_MIN_HEIGHT ||
		newArea->w <= Const::AREA_MIN_WIDTH)
		return;
	mAreas.push_back(newArea);

#ifdef _DEBUG
	std::cout << id << ":"
		<< "p = " << p << std::endl;
	for (auto a : mAreas) {
		std::cout << "(x, y, w, h) = (" << a->x << ", " << a->y << ", " << a->w
			<< ", " << a->h << ")" << std::endl;
	}
#endif
	bool next = (!isVertical);

#ifdef _DEBUG
	std::cout << "next: " << next << std::endl;
#endif 
	DungeonGenerator::splitArea(newArea, id + 1, next);
}

void DungeonGenerator::createRoom() {
	for (auto a : mAreas) {
		int rwmax =
			a->w - 2 <= Const::ROOM_MAX_WIDTH ? a->w - 2 : Const::ROOM_MAX_WIDTH;
		int rhmax =
			a->h - 2 <= Const::ROOM_MAX_HEIGHT ? a->h - 2 : Const::ROOM_MAX_HEIGHT;
		int rw = rand->randInt(Const::ROOM_MIN_WIDTH, rwmax);
		int rh = rand->randInt(Const::ROOM_MIN_HEIGHT, rhmax);
#ifdef _DEBUG
		std::cout << "rw, rh = " << rw << ", " << rh << std::endl;

		std::cout << "x:min, max= " << a->x + 1 << ", " << a->x + a->w - 1 - rw
			<< std::endl;
		std::cout << "y:min, max= " << a->y + 1 << ", " << a->y + a->h - 1 - rh
			<< std::endl;
#endif
		int ix = rand->randInt(a->x + 1, a->x + a->w - 1 - rw);
		int iy = rand->randInt(a->y + 1, a->y + a->h - 1 - rh);
		a->room->set(ix, iy, rw, rh);
	}
}

void DungeonGenerator::connectRooms() {
	for (int i = 0; i < mAreas.size(); i++) {
		mAreas[i]->aisle = new Aisle();
		// if y==0 then there is not north area.
		if (mAreas[i]->y) {
			int north = rand->randInt(mAreas[i]->room->x,
				mAreas[i]->room->x + mAreas[i]->room->w - 1);
			for (int j = mAreas[i]->y; j < mAreas[i]->room->y; j++) {
				mFloor->data[j][north] = Const::Cell::Aisle;
			}
			mAreas[i]->aisle->north = north;
		}

		// if y+h==FLOOR_MAX_HEIGHT, there is not south area.
		if (mAreas[i]->y + mAreas[i]->h != Const::FLOOR_MAX_HEIGHT) {
			int south = rand->randInt(mAreas[i]->room->x,
				mAreas[i]->room->x + mAreas[i]->room->w - 1);
			for (int j = mAreas[i]->room->y + mAreas[i]->room->h;
				j < mAreas[i]->y + mAreas[i]->h; j++) {
				mFloor->data[j][south] = Const::Cell::Aisle;
			}
			mAreas[i]->aisle->south = south;
		}

		// if x==0, there is not west area.
		if (mAreas[i]->x) {
			int west = rand->randInt(mAreas[i]->room->y,
				mAreas[i]->room->y + mAreas[i]->room->h - 1);
			for (int j = mAreas[i]->x; j < mAreas[i]->room->x; j++) {
				mFloor->data[west][j] = Const::Cell::Aisle;
			}
			mAreas[i]->aisle->west = west;
		}

		// if x+w==FLOOR_MAX_WIDTH, there is not east area.
		if (mAreas[i]->x + mAreas[i]->w != Const::FLOOR_MAX_WIDTH) {
			int east = rand->randInt(mAreas[i]->room->y,
				mAreas[i]->room->y + mAreas[i]->room->h - 1);
			for (int j = mAreas[i]->room->x + mAreas[i]->room->w;
				j < mAreas[i]->x + mAreas[i]->w; j++) {
				mFloor->data[east][j] = Const::Cell::Aisle;
			}
			mAreas[i]->aisle->east = east;
		}
	}
	// connect
	for (int i = 0; i < mAreas.size() - 1; i++) {
		if (mAreas[i]->aisle->east != -1 && mAreas[i + 1]->aisle->west != -1) {
			int start = mAreas[i]->aisle->east > mAreas[i + 1]->aisle->west
				? mAreas[i + 1]->aisle->west
				: mAreas[i]->aisle->east;
			int fin = mAreas[i]->aisle->east < mAreas[i + 1]->aisle->west
				? mAreas[i + 1]->aisle->west
				: mAreas[i]->aisle->east;
#ifdef _DEBUG
			std::cout << start << ", " << fin << std::endl;
#endif
			for (int j = start; j <= fin; j++) {
				mFloor->data[j][mAreas[i]->x + mAreas[i]->w] = Const::Cell::Aisle;
			}
		}

		if (mAreas[i]->aisle->south != -1 && mAreas[i + 1]->aisle->north != -1) {
			int start = mAreas[i]->aisle->south < mAreas[i + 1]->aisle->north
				? mAreas[i]->aisle->south
				: mAreas[i + 1]->aisle->north;
			int fin = mAreas[i]->aisle->south > mAreas[i + 1]->aisle->north
				? mAreas[i]->aisle->south
				: mAreas[i + 1]->aisle->north;
			for (int j = start; j <= fin; j++) {
				mFloor->data[mAreas[i]->y + mAreas[i]->h - 1][j] = Const::Cell::Aisle;
			}
		}
	}
}

void DungeonGenerator::setFloor() {
	bool drawRoom = true;
	if (drawRoom) {
		for (auto a : mAreas) {
			for (int i = a->room->x; i < a->room->x + a->room->w; i++) {
				for (int j = a->room->y; j < a->room->y + a->room->h; j++) {
					mFloor->data[j][i] = Const::Cell::Floor;
				}
			}
		}
	}
}

XMFLOAT2 DungeonGenerator::getRandomPosInRoom() {
	int id = rand->randInt(0, mAreas.size() - 1);
	XMFLOAT2 res;
	res.x = rand->randInt(mAreas[id]->room->x,
		mAreas[id]->room->x + mAreas[id]->room->w - 1);
	res.y = rand->randInt(mAreas[id]->room->y,
		mAreas[id]->room->y + mAreas[id]->room->h - 1);
	res.x *= Const::CELL_SIZE;
	res.y *= Const::CELL_SIZE;
	return res;
}

Floor::Floor() {
	data.resize(Const::FLOOR_MAX_HEIGHT);
	for (int i = 0; i < Const::FLOOR_MAX_HEIGHT; i++) {
		data[i].resize(Const::FLOOR_MAX_WIDTH);
	}

	for (int i = 0; i < Const::FLOOR_MAX_HEIGHT; i++) {
		for (int j = 0; j < Const::FLOOR_MAX_WIDTH; j++) {
			data[i][j] = Const::Cell::Wall;
		}
	}
}

Floor::~Floor() {};