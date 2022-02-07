#pragma once
#include <fstream>
#include <vector>
#include <string>

class SaveData {
public:
	SaveData(class Game* game);
	~SaveData();
	void Read();
	void Write();

	bool IsSaveFileExist();
	struct ItemData {
		std::string classname;
	};
	struct Data {
		int floorNum;
		int maxHp;
		int hp;
		int exp;
		int level;
		int attack;
		int hunger;
		std::vector<ItemData> items;
	};
	Data GetData() { return mData; }
private:
	const std::string filename = "Resources/SaveData.txt";
	class Game* mGame;
	Data mData;
};