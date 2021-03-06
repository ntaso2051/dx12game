#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

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
	HRESULT InitVbIbForSprite();

	ComPtr<ID3D12Device> Device();
	ComPtr<ID3D12GraphicsCommandList> CmdList() { return mCmdList; }
	ComPtr<ID3D12CommandAllocator> CmdAllocator() { return mCmdAllocator; }
	ComPtr<ID3D12Resource> GetVbForSprite() { return mVbForSprite; }
	ComPtr<ID3D12Resource> GetIbForSprite() { return mIbForSprite; }
	D3D12_VERTEX_BUFFER_VIEW GetVbView() { return mVbView; }
	D3D12_INDEX_BUFFER_VIEW GetIbView() { return mIbView; }
	ComPtr<ID3D12RootSignature> GetRootsignatureForSprite() { return mRootsignatureForSprite; }
	ComPtr<ID3D12PipelineState> GetPipelinestateForSprite() { return mPipelinestateForSprite; }

	D3D12_VIEWPORT Viewport() { return mViewport; }
	D3D12_RECT Scissorrect() { return mScissorrect; }
	// ウィンドウをクリア
	void StartDraw();
	void EndDraw();

	// シェーダー関連
	void CompileShader();
	class Shader* GetShader() { return mShader; }

	void InitViewport(const int w, const int h);

	void ShaderCompile();

	// デバッグ用
	void EnableDebugLayer();
private:
	// DXGI関連
	ComPtr<IDXGIFactory4> mDxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4> mSwapChain = nullptr;

	// DirectX12関連
	ComPtr<ID3D12Device> mDev = nullptr;													// デバイス
	ComPtr<ID3D12CommandAllocator> mCmdAllocator = nullptr;				// コマンドアロケータ
	ComPtr<ID3D12GraphicsCommandList> mCmdList = nullptr;					// コマンドリスト
	ComPtr<ID3D12CommandQueue> mCmdQueue = nullptr;								// コマンドキュー
	ComPtr<ID3D12DescriptorHeap> mRtvHeaps = nullptr;							// ディスクリプタヒープ
	std::vector<ID3D12Resource*> mBackBuffers;										// バックバッファ

	// フェンス
	ComPtr<ID3D12Fence> mFence = nullptr;
	UINT mFenceVal = 0;

	// シェーダー
	class Shader* mShader;

	// 頂点バッファビュー 
	D3D12_VERTEX_BUFFER_VIEW mVbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW mIbView = {};
	// ビューポート
	D3D12_VIEWPORT mViewport = {};
	// シザー短径
	D3D12_RECT mScissorrect = {};
	// ルートシグニチャ
	ComPtr<ID3D12RootSignature> mRootsignatureForSprite = nullptr;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> mPipelinestateForSprite = nullptr;

	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};
	ComPtr<ID3D12Resource> mVbForSprite = nullptr;
	ComPtr<ID3D12Resource> mIbForSprite = nullptr;

	// デバッグ用
	ComPtr<ID3D12Debug> mDebugLayer = nullptr;
};