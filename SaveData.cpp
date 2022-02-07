#include "SaveData.h"
#include "Game.h"
#include "ImguiWrapper.h"
#include "Item.h"
#include "Hero.h"
#include "ParameterComponent.h"
#include <typeinfo>
#include <iostream>
#include <filesystem>
#include <fstream>

SaveData::SaveData(Game* game) : mGame(game) {

}

SaveData::~SaveData() {

}

bool SaveData::IsSaveFileExist() {
	std::ifstream fin(filename);
	if (!fin.is_open()) {
		fin.close();
		std::ofstream fout(filename);
		fout << "";
		fout.close();
	}
	std::string tmp = "";
	fin >> tmp;
	bool res = true;
	if (tmp == "") {
		res = false;
	}
	fin.close();
	return res;
}

void SaveData::Read() {
	std::ifstream fin(filename);
	if (!fin) return;
	fin >> mData.floorNum;
	fin >> mData.maxHp;
	fin >> mData.hp;
	fin >> mData.exp;
	fin >> mData.level;
	fin >> mData.attack;
	fin >> mData.hunger;
	while (!fin.eof()) {
		std::string cn = "";
		fin >> cn;
		fin >> cn;
		if (cn == "") break;
		ItemData itemdata;
		itemdata.classname = cn;
		mData.items.push_back(itemdata);
	}
	fin.close();
}

void SaveData::Write() {
	std::ofstream fout(filename);
	if (!fout) return;
	Hero* hero = mGame->GetHero();
	ParameterComponent* pc = static_cast<ParameterComponent*>(hero->GetComponent("ParameterComponent"));
	fout << mGame->GetFloorNum() << "\n";
	fout << pc->GetMaxHp() << "\n";
	fout << pc->GetHp() << "\n";
	fout << pc->GetExp() << "\n";
	fout << pc->GetLevel() << "\n";
	fout << pc->GetAttack() << "\n";
	fout << pc->GetHunger() << "\n";

	std::vector<Item*> items = hero->GetMyItems();
	for (auto item : items) {
		fout << typeid(*item).name() << "\n";
	}

	fout.close();
}



void SaveData::Clear() {
	std::ofstream fout(filename);
	if (!fout)return;
	fout << "";
	fout.close();
}