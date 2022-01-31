#pragma once
namespace Const {
	const int CELL_SIZE = 16;

	const int MINI_MAP_CELL_SIZE = 12;

	const int FLOOR_MAX_WIDTH = 64;
	const int FLOOR_MAX_HEIGHT = 32;
	const int AREA_MIN_WIDTH = 4;
	const int AREA_MIN_HEIGHT = 4;
	const int ROOM_PADING = 4;
	const int ROOM_MIN_WIDTH = 2;
	const int ROOM_MIN_HEIGHT = 2;
	const int ROOM_MAX_WIDTH = 12;
	const int ROOM_MAX_HEIGHT = 12;

	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;

	const int HERO_INIT_HP = 100;
	const int HERO_INIT_EXP = 0;
	const int HERO_INIT_LEVEL = 1;
	const int HERO_INIT_ATTACK = 3;

	const int BLOB_INIT_HP = 5;
	const int BLOB_INIT_EXP = 2;
	const int BLOB_INIT_LEVEL = 1;
	const int BLOB_INIT_ATTACK = 1;

	const float CHARACTER_MOVE_TIME = 0.5f;

	namespace Cell {
		enum Type {
			None,
			Floor,
			Aisle,
			Wall,
			Hero,
			Enemy,
			Stair,
			Item,
		};
	};

	namespace TexId {
		enum Type {
			Hero,
			Wall,
			Blob,
		};
	};

	enum Dir {
		N,
		NE,
		E,
		SE,
		S,
		SW,
		W,
		NW,
	};
};  // namespace Const