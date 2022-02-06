#include "ImguiWrapper.h"
#include "Dx12Wrapper.h"
#include "Input.h"
#include "Game.h"
#include "Hero.h"
#include <DirectXMath.h>
#include "Const.h"
#include "DungeonGenerator.h"
#include "ParameterComponent.h"
#include "CharacterManager.h"
#include "Item.h"
#include "Enemy.h"

using namespace ImGui;
using namespace DirectX;

ImguiWrapper::ImguiWrapper(HWND hwnd, Dx12Wrapper* dx12, Input* input, Game* game) :mDx12Wrapper(dx12), mInput(input), mGame(game), mItemCmd(false) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig conf;
	conf.MergeMode = true;
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("Resources/fonts/PixelMplus10-Regular.ttf", 12.0f, &conf, io.Fonts->GetGlyphRangesJapanese());
	(void)io;
	ImGui::StyleColorsDark();

	int NUM_FRAMES_IN_FLIGHT = 3;

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 1;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	mDx12Wrapper->Device()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(mDescHeap.ReleaseAndGetAddressOf()));

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(
		mDx12Wrapper->Device().Get(),
		NUM_FRAMES_IN_FLIGHT,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		mDescHeap.Get(),
		mDescHeap->GetCPUDescriptorHandleForHeapStart(),
		mDescHeap->GetGPUDescriptorHandleForHeapStart()
	);
}

ImguiWrapper::~ImguiWrapper() {
	// Cleanup
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImguiWrapper::Draw() {
	// Start the Dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// KeyState debug imgui
	if (!mGame->GetIsDisplayUI()) {
		bool pushW = mInput->GetKey(Input::KEY_INFO::W_KEY);
		bool enterW = mInput->GetKeyEnter(Input::KEY_INFO::W_KEY);
		bool exitW = mInput->GetKeyExit(Input::KEY_INFO::W_KEY);
		bool pushA = mInput->GetKey(Input::KEY_INFO::A_KEY);
		bool enterA = mInput->GetKeyEnter(Input::KEY_INFO::A_KEY);
		bool exitA = mInput->GetKeyExit(Input::KEY_INFO::A_KEY);
		bool pushS = mInput->GetKey(Input::KEY_INFO::S_KEY);
		bool enterS = mInput->GetKeyEnter(Input::KEY_INFO::S_KEY);
		bool exitS = mInput->GetKeyExit(Input::KEY_INFO::S_KEY);
		bool pushD = mInput->GetKey(Input::KEY_INFO::D_KEY);
		bool enterD = mInput->GetKeyEnter(Input::KEY_INFO::D_KEY);
		bool exitD = mInput->GetKeyExit(Input::KEY_INFO::D_KEY);
		bool pushSpace = mInput->GetKey(Input::KEY_INFO::SPACE_KEY);
		bool enterSpace = mInput->GetKeyEnter(Input::KEY_INFO::SPACE_KEY);
		bool exitSpace = mInput->GetKeyExit(Input::KEY_INFO::SPACE_KEY);
		bool pushZ = mInput->GetKey(Input::KEY_INFO::Z_KEY);
		bool enterZ = mInput->GetKeyEnter(Input::KEY_INFO::Z_KEY);
		bool exitZ = mInput->GetKeyExit(Input::KEY_INFO::Z_KEY);
		bool pushSHIFT = mInput->GetKey(Input::KEY_INFO::SHIFT_KEY);
		bool enterSHIFT = mInput->GetKeyEnter(Input::KEY_INFO::SHIFT_KEY);
		bool exitSHIFT = mInput->GetKeyExit(Input::KEY_INFO::SHIFT_KEY);

		Begin("KeyState");
		Checkbox("GetKey(w)", &pushW);
		Checkbox("GetKeyEnter(w)", &enterW);
		Checkbox("GetKeyExit(w)", &exitW);
		Checkbox("GetKey(a)", &pushA);
		Checkbox("GetKeyEnter(a)", &enterA);
		Checkbox("GetKeyExit(a)", &exitA);
		Checkbox("GetKey(S)", &pushS);
		Checkbox("GetKeyEnter(S)", &enterS);
		Checkbox("GetKeyExit(S)", &exitS);
		Checkbox("GetKey(D)", &pushD);
		Checkbox("GetKeyEnter(D)", &enterD);
		Checkbox("GetKeyExit(D)", &exitD);
		Checkbox("GetKey(SPACE)", &pushSpace);
		Checkbox("GetKeyEnter(SPACE)", &enterSpace);
		Checkbox("GetKeyExit(SPACE)", &exitSpace);
		Checkbox("GetKey(Z)", &pushZ);
		Checkbox("GetKeyEnter(Z)", &enterZ);
		Checkbox("GetKeyExit(Z)", &exitZ);
		Checkbox("GetKey(SHIFT)", &pushSHIFT);
		Checkbox("GetKeyEnter(SHIFT)", &enterSHIFT);
		Checkbox("GetKeyExit(SHIFT)", &exitSHIFT);
		End();
	}
	// Hero info debug imgui
	if (!mGame->GetIsDisplayUI()) {
		XMFLOAT3 heroPos = mGame->GetHero()->GetPosition();
		XMINT2 heroDir = mGame->GetHero()->GetDir();
		ParameterComponent* pc = static_cast<ParameterComponent*>(mGame->GetHero()->GetComponent("ParameterComponent"));
		Begin(u8"プレイヤーステータス");
		Text("Position(%f, %f)", heroPos.x, heroPos.y);
		Text("HP: %d / %d", pc->GetHp(), pc->GetMaxHp());
		Text("EXP: %d", pc->GetExp());
		Text("LEVEL: %d", pc->GetLevel());
		Text("ATTACK: %d", pc->GetAttack());
		Text("HUNGER: %d", pc->GetHunger());
		Text("mDirection: %d, %d", heroDir.x, heroDir.y);
		Text("B%dF", mGame->GetFloorNum());


		End();
	}
	// ログ
	if (!mGame->GetIsDisplayUI()) {
		Begin(u8"ログ");
		for (auto log : mLogs) {
			Text("%s", log.c_str());
		}
		End();
	}
	// Item List
	if (!mGame->GetIsDisplayUI()) {
		if (!mItemCmd) {
			Begin(u8"アイテムリスト");
			auto items = mGame->GetHero()->GetMyItems();
			for (int i = 0; i < items.size(); i++) {
				auto item = items[i];
				std::string label = std::to_string(i + 1) + u8"：" + item->GetName() + "  " + item->GetInfo();
				if (Button(label.c_str()) && !mItemCmd) {
					mItemCmd = true;
					mSelectedItem = item;
				}
			}
			End();
		}
		// Item Command
		if (mItemCmd) {
			std::string label = mSelectedItem->GetName();
			Begin(label.c_str());
			if (Button(u8"使う")) {
				mSelectedItem->Adapt();
				mSelectedItem = nullptr;
				mItemCmd = false;
			}
			if (Button(u8"捨てる")) {
				mSelectedItem->Remove();
				mSelectedItem = nullptr;
				mItemCmd = false;
			}
			// if (mSelectedItem->GetType() == Item::Type::Equipment) {
			if (Button(u8"はずす")) {
				mSelectedItem->Deadapt();
				mSelectedItem = nullptr;
				mItemCmd = false;
			}
			// }
			if (Button(u8"何もしない")) {
				mItemCmd = false;
				mSelectedItem = nullptr;
			}
			End();
		}
	}
	// Render minimap
	if (!mGame->GetIsDisplayUI()) {
		Begin("MiniMap");
		auto data = mGame->GetDgGen()->getFloor()->data;
		for (int i = data.size() - 1; i >= 0; i--) {
			std::string colStr = "";
			for (int j = 0; j < data[0].size(); j++) {
				if (data[i][j] == Const::Cell::Wall) {
					colStr += "  ";
				}
				else if (data[i][j] == Const::Cell::Hero) {
					colStr += u8"P1";
				}
				else if (data[i][j] == Const::Cell::Enemy) {
					colStr += u8"EM";
				}
				else if (data[i][j] == Const::Cell::Stair) {
					colStr += u8"ST";
				}
				else {
					colStr += u8"[]";
				}
			}
			Text("%s\n\r", colStr.c_str());
		}
		End();
	}
	if (!mGame->GetIsDisplayUI()) {
		Begin("Enemy State");
		int cnt = mGame->GetChrManager()->GetEnemiesCnt();
		int total = mGame->GetChrManager()->GetEnemiesTotal();
		Text("Count: %d, Total: %d", cnt, total);
		auto enemy = mGame->GetChrManager()->GetEnemies();
		for (int i = 0; i < enemy.size(); i++) {
			auto state = enemy[i]->GetState();
			Text("Enemy[%d]: %d", i, state);
		}
		auto deadEnemy = mGame->GetChrManager()->GetDeadEnemies();
		for (int i = 0; i < deadEnemy.size(); i++) {
			auto pos = deadEnemy[i]->GetPosition();
			Text("DeadEnemy[i] (%f, %f)", pos.x, pos.y);
		}
		End();
	}
	Render();
	mDx12Wrapper->CmdList()->SetDescriptorHeaps(1, mDescHeap.GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), mDx12Wrapper->CmdList().Get());
}

void ImguiWrapper::Cout(std::string log) {
	mLogs.insert(mLogs.begin(), log);
}

void ImguiWrapper::ClearLogs() {
	mLogs.clear();
}