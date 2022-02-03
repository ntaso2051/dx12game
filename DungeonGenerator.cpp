#include "DungeonGenerator.h"
#include "Random.h"

Area::Area() { this->room = new Room(); }
Area::~Area() {
	delete this->room;
	delete this->aisle;
}

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
Aisle::~Aisle() {}

DungeonGenerator::DungeonGenerator() { mRand = new Random(); }

DungeonGenerator::~DungeonGenerator() {
	delete mFloor;
	delete mRand;
	for (auto area : mAreas) {
		delete area;
	}
}

void DungeonGenerator::createDg() {
	delete mFloor;
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
}

void DungeonGenerator::splitArea(Area * area, int id, bool isVertical) {
	// if (id > 3) return;
	int randmax = isVertical ? area->w : area->h;
	int randmin = isVertical ? Const::AREA_MIN_WIDTH : Const::AREA_MIN_HEIGHT;
	if (id == 1) {
		randmax = isVertical ? area->w / 2 : area->h / 2;
	}

	if (randmax - 2 - (randmin) <= 0) return;

	int p = mRand->randInt(randmin, randmax - 2);
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

	bool next = (!isVertical);

	DungeonGenerator::splitArea(newArea, id + 1, next);
}

void DungeonGenerator::createRoom() {
	for (auto a : mAreas) {
		int rwmax =
			a->w - 2 <= Const::ROOM_MAX_WIDTH ? a->w - 2 : Const::ROOM_MAX_WIDTH;
		int rhmax =
			a->h - 2 <= Const::ROOM_MAX_HEIGHT ? a->h - 2 : Const::ROOM_MAX_HEIGHT;
		int rw = mRand->randInt(Const::ROOM_MIN_WIDTH, rwmax);
		int rh = mRand->randInt(Const::ROOM_MIN_HEIGHT, rhmax);
		int ix = mRand->randInt(a->x + 1, a->x + a->w - 1 - rw);
		int iy = mRand->randInt(a->y + 1, a->y + a->h - 1 - rh);
		a->room->set(ix, iy, rw, rh);
	}
}

void DungeonGenerator::connectRooms() {
	for (int i = 0; i < mAreas.size(); i++) {
		mAreas[i]->aisle = new Aisle();
		// if y==0 then there is not north area.
		if (mAreas[i]->y) {
			int north = mRand->randInt(mAreas[i]->room->x,
				mAreas[i]->room->x + mAreas[i]->room->w - 1);
			for (int j = mAreas[i]->y; j < mAreas[i]->room->y; j++) {
				mFloor->data[j][north] = Const::Cell::Aisle;
			}
			mAreas[i]->aisle->north = north;
		}

		// if y+h==FLOOR_MAX_HEIGHT, there is not south area.
		if (mAreas[i]->y + mAreas[i]->h != Const::FLOOR_MAX_HEIGHT) {
			int south = mRand->randInt(mAreas[i]->room->x,
				mAreas[i]->room->x + mAreas[i]->room->w - 1);
			for (int j = mAreas[i]->room->y + mAreas[i]->room->h;
				j < mAreas[i]->y + mAreas[i]->h; j++) {
				mFloor->data[j][south] = Const::Cell::Aisle;
			}
			mAreas[i]->aisle->south = south;
		}

		// if x==0, there is not west area.
		if (mAreas[i]->x) {
			int west = mRand->randInt(mAreas[i]->room->y,
				mAreas[i]->room->y + mAreas[i]->room->h - 1);
			for (int j = mAreas[i]->x; j < mAreas[i]->room->x; j++) {
				mFloor->data[west][j] = Const::Cell::Aisle;
			}
			mAreas[i]->aisle->west = west;
		}

		// if x+w==FLOOR_MAX_WIDTH, there is not east area.
		if (mAreas[i]->x + mAreas[i]->w != Const::FLOOR_MAX_WIDTH) {
			int east = mRand->randInt(mAreas[i]->room->y,
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

void DungeonGenerator::SetCellType(int x, int y, Const::Cell::Type cellType) {
	mFloor->data[y][x] = cellType;
}

XMFLOAT2 DungeonGenerator::getRandomPosInRoom() {
	int id = mRand->randInt(0, mAreas.size() - 1);
	XMFLOAT2 res;
	res.x = mRand->randInt(mAreas[id]->room->x,
		mAreas[id]->room->x + mAreas[id]->room->w - 1);
	res.y = mRand->randInt(mAreas[id]->room->y,
		mAreas[id]->room->y + mAreas[id]->room->h - 1);
	res.x;
	res.y;
	if (mFloor->data[res.y][res.x] != Const::Cell::Floor) {
		res = getRandomPosInRoom();
	}
	return res;
}

int DungeonGenerator::GetCurrentRoom(XMINT2 pos) {
	for (int id = 0; id < mAreas.size(); id++) {
		if (
			pos.x >= mAreas[id]->room->x &&
			pos.x < mAreas[id]->room->x + mAreas[id]->room->w &&
			pos.y >= mAreas[id]->room->y &&
			pos.y < mAreas[id]->room->y + mAreas[id]->room->h
			) {
			return id;
		}
	}
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

Floor::~Floor() {
	for (auto row : data) {
		row.clear();
	}
	data.clear();
};