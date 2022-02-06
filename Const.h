#pragma once
namespace Const {
	const int CELL_SIZE = 16;

	const int INIT_ENEMIES_NUM = 5;
	const int INIT_ITEM_NUM = 5;

	const int MINI_MAP_CELL_SIZE = 12;

	const int FLOOR_MAX_WIDTH = 32;
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
			Wall,
			Blob,
			STAIR,

			Weapon,
			Food,

			HFront1,
			HFront2,
			HFront3,
			HRight1,
			HRight2,
			HRight3,
			HLeft1,
			HLeft2,
			HLeft3,
			HBack1,
			HBack2,
			HBack3,
			HFrontRight1,
			HFrontRight2,
			HFrontRight3,
			HFrontLeft1,
			HFrontLeft2,
			HFrontLeft3,
			HBackRight1,
			HBackRight2,
			HBackRight3,
			HBackLeft1,
			HBackLeft2,
			HBackLeft3,

			UI,
			UI0,
			UI1,
			UI2,
			UI3,
			UI4,
			UI5,
			UI6,
			UI7,
			UI8,
			UI9,
			UIB,
			UIF,

			Title,
			TitleSub,
		};
	};

	enum State {
		Move,		// 動いてる間
		Idle,		// 入力待ち
		End,		// ターン中の行動が終わった状態
		Attack, // 攻撃中
		Dead,		// 死んでる
	};
};  // namespace Const