#include "ImguiWrapper.h"
#include "Dx12Wrapper.h"

ImguiWrapper::ImguiWrapper(HWND hwnd, Dx12Wrapper* dx12) :mDx12Wrapper(dx12) {
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
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::ColorEdit3("clear color", (float*)& mClear_color); // Edit 3 floats representing a color

		ImGui::SameLine();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
	ImGui::Render();
	mDx12Wrapper->CmdList()->SetDescriptorHeaps(1, mDescHeap.GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), mDx12Wrapper->CmdList().Get());
}