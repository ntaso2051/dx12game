#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

class Dx12Wrapper;
class Texture;

class SpriteRenderer {
public:
	SpriteRenderer(Dx12Wrapper& dx12Wrapper);
	~SpriteRenderer();

	void Draw();
	HRESULT CreateTexture(float windowWidth, float windowHeight);
	void InitView(float windowWidth, float windowHeight, const wchar_t* filename);
	HRESULT InitGraphicPipeline();
	HRESULT InitRootSignature();
	Texture* GetTexture() { return mTexture; }
	void ReComputeMatrix(XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projMat);
	void InitMatrix(XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projMat);
private:
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};
	Dx12Wrapper& mDx12Wrapper;
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12PipelineState> mPipeline = nullptr;
	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	//頂点関連
	ComPtr<ID3D12Resource> mVb = nullptr;
	ComPtr<ID3D12Resource> mIb = nullptr;
	D3D12_VERTEX_BUFFER_VIEW mVbView = {};
	D3D12_INDEX_BUFFER_VIEW mIbView = {};
	// テクスチャ関連
	Texture* mTexture;
	ComPtr<ID3D12Resource> mTexBuff = nullptr;
	ComPtr<ID3D12DescriptorHeap> mTexDescHeap = nullptr;
	D3D12_HEAP_PROPERTIES GetHeapPropForTex();
	D3D12_RESOURCE_DESC GetResourseDescForTex();

	// 座標関連
	XMMATRIX mWorldMat;
	XMMATRIX mViewMat;
	XMMATRIX mProjMat;
	XMMATRIX* mMapMatrix;


	// テスト用
	float angle = 0.0f;
};