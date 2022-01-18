#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <DirectXMath.h>

class Dx12Wrapper;
class SpriteRenderer {
public:
	SpriteRenderer(Dx12Wrapper& dx12Wrapper);
	~SpriteRenderer();

	void Draw();

	void InitView();
	void CompileShader();
	HRESULT InitGraphicPipeline();
	HRESULT InitRootSignature();
private:
	Dx12Wrapper& mDx12Wrapper;
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12PipelineState> mPipeline = nullptr;
	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	// シェーダー関連
	ComPtr<ID3DBlob> mVsBlob = nullptr;
	ComPtr<ID3DBlob> mPsBlob = nullptr;
	//頂点関連
	ComPtr<ID3D12Resource> mVb = nullptr;
	ComPtr<ID3D12Resource> mIb = nullptr;
	D3D12_VERTEX_BUFFER_VIEW mVbView = {};
	D3D12_INDEX_BUFFER_VIEW mIbView = {};
};