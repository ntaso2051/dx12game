#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

class Dx12Wrapper {
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Dx12Wrapper();
	HRESULT InitDXGIDevice();
	HRESULT InitCommand();
	HRESULT InitSwapChain(const HWND& hwnd, const unsigned int w, const unsigned int h);
	HRESULT InitRenderTargets();
	HRESULT InitFence();
	HRESULT InitGraphicPipelineForSprite();
	HRESULT InitRootsignatureForSprite();

	ComPtr<ID3D12Device> Device();
	ComPtr<ID3D12GraphicsCommandList> CmdList() { return mCmdList; }
	ComPtr<ID3D12CommandAllocator> CmdAllocator() { return mCmdAllocator; }

	ComPtr<ID3D12RootSignature> GetRootsignatureForSprite() { return mRootsignatureForSprite; }
	ComPtr<ID3D12PipelineState> GetPipelinestateForSprite() { return mPipelinestateForSprite; }

	D3D12_VIEWPORT Viewport() { return mViewport; }
	D3D12_RECT Scissorrect() { return mScissorrect; }
	// �E�B���h�E���N���A
	void StartDraw();
	void EndDraw();

	// �V�F�[�_�[�֘A
	void CompileShader();
	class Shader* GetShader() { return mShader; }

	void InitViewport(const int w, const int h);

	void ShaderCompile();

	// �f�o�b�O�p
	void EnableDebugLayer();
private:
	// DXGI�֘A
	ComPtr<IDXGIFactory4> mDxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4> mSwapChain = nullptr;

	// DirectX12�֘A
	ComPtr<ID3D12Device> mDev = nullptr;													// �f�o�C�X
	ComPtr<ID3D12CommandAllocator> mCmdAllocator = nullptr;				// �R�}���h�A���P�[�^
	ComPtr<ID3D12GraphicsCommandList> mCmdList = nullptr;					// �R�}���h���X�g
	ComPtr<ID3D12CommandQueue> mCmdQueue = nullptr;								// �R�}���h�L���[
	ComPtr<ID3D12DescriptorHeap> mRtvHeaps = nullptr;							// �f�B�X�N���v�^�q�[�v
	std::vector<ID3D12Resource*> mBackBuffers;										// �o�b�N�o�b�t�@

	// �t�F���X
	ComPtr<ID3D12Fence> mFence = nullptr;
	UINT mFenceVal = 0;

	// �V�F�[�_�[
	class Shader* mShader;

	// ���_�o�b�t�@�r���[ 
	D3D12_VERTEX_BUFFER_VIEW mVbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW mIbView = {};
	// �r���[�|�[�g
	D3D12_VIEWPORT mViewport = {};
	// �V�U�[�Z�a
	D3D12_RECT mScissorrect = {};
	// ���[�g�V�O�j�`��
	ComPtr<ID3D12RootSignature> mRootsignatureForSprite = nullptr;
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> mPipelinestateForSprite = nullptr;



	// �f�o�b�O�p
	ComPtr<ID3D12Debug> mDebugLayer = nullptr;
};