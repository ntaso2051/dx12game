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
#include "Enemy.h"

using namespace ImGui;
using namespace DirectX;

ImguiWrapper::ImguiWrapper(HWND hwnd, Dx12Wrapper* dx12, Input* input, Game* game) :mDx12Wrapper(dx12), mInput(input), mGame(game) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
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
	{
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
		End();
	}
	// Hero info debug imgui
	{
		XMFLOAT3 heroPos = mGame->GetHero()->GetPosition();
		XMINT2 heroDir = mGame->GetHero()->GetDir();
		ParameterComponent* pc = static_cast<ParameterComponent*>(mGame->GetHero()->GetHeroParam());
		Begin("Hero State");
		Text("Position(%f, %f)", heroPos.x, heroPos.y);
		Text("HP: %d", pc->GetHp());
		Text("EXP: %d", pc->GetExp());
		Text("LEVEL: %d", pc->GetLevel());
		Text("ATTACK: %d", pc->GetAttack());
		Text("mDirection: %d, %d", heroDir.x, heroDir.y);

		End();
	}
	// Render minimap
	{
		Begin("MiniMap");
		auto data = mGame->GetDgGen()->getFloor()->data;
		for (int i = data.size() - 1; i >= 0; i--) {
			std::string colStr = "";
			for (int j = 0; j < data[0].size(); j++) {
				if (data[i][j] == Const::Cell::Wall) {
					colStr += "  ";
				}
				else if (data[i][j] == Const::Cell::Hero) {
					colStr += "HR";
				}
				else if (data[i][j] == Const::Cell::Enemy) {
					colStr += "EM";
				}
				else if (data[i][j] == Const::Cell::Stair) {
					colStr += "xx";
				}
				else {
					colStr += "[]";
				}
			}
			Text("%s\n\r", colStr.c_str());
		}
		End();
	}
	{
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