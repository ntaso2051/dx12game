#include "ImguiWrapper.h"
#include "Dx12Wrapper.h"
#include "Input.h"

ImguiWrapper::ImguiWrapper(HWND hwnd, Dx12Wrapper* dx12, Input* input) :mDx12Wrapper(dx12), mInput(input) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsClassic();

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
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		ImGui::Begin("KeyState");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Checkbox("GetKey(w)", &pushW);
		ImGui::Checkbox("GetKeyEnter(w)", &enterW);
		ImGui::Checkbox("GetKeyExit(w)", &exitW);
		ImGui::Checkbox("GetKey(a)", &pushA);
		ImGui::Checkbox("GetKeyEnter(a)", &enterA);
		ImGui::Checkbox("GetKeyExit(a)", &exitA);
		ImGui::Checkbox("GetKey(S)", &pushS);
		ImGui::Checkbox("GetKeyEnter(S)", &enterS);
		ImGui::Checkbox("GetKeyExit(S)", &exitS);
		ImGui::Checkbox("GetKey(D)", &pushD);
		ImGui::Checkbox("GetKeyEnter(D)", &enterD);
		ImGui::Checkbox("GetKeyExit(D)", &exitD);
		ImGui::End();
	}
	ImGui::Render();
	mDx12Wrapper->CmdList()->SetDescriptorHeaps(1, mDescHeap.GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), mDx12Wrapper->CmdList().Get());
}