#pragma once
namespace Const {
	const int CELL_SIZE = 16;

	const int MINI_MAP_CELL_SIZE = 12;

	const int FLOOR_MAX_WIDTH = 32;
	const int FLOOR_MAX_HEIGHT = 16;
	const int AREA_MIN_WIDTH = 4;
	const int AREA_MIN_HEIGHT = 4;
	const int ROOM_PADING = 4;
	const int ROOM_MIN_WIDTH = 2;
	const int ROOM_MIN_HEIGHT = 2;
	const int ROOM_MAX_WIDTH = 12;
	const int ROOM_MAX_HEIGHT = 12;

	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;

	const float CHARACTER_MOVE_TIME = 0.5f;

	namespace Cell {
		enum Type {
			None,
			Floor,
			Aisle,
			Wall,
		};
	};

	namespace TexId {
		enum Type {
			Hero,
			Wall,
		};
	};
};  // namespace Const