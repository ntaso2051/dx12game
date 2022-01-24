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
	SpriteRenderer(Dx12Wrapper& dx12Wrapper, Texture* tex);
	~SpriteRenderer();

	void Draw();
	HRESULT CreateTexture(float windowWidth, float windowHeight);
	Texture* GetTexture() { return mTexture; }
	void ReComputeMatrix(XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projMat);
	void InitMatrix(XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projMat);
private:
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};
	Dx12Wrapper& mDx12Wrapper;
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	// �e�N�X�`���֘A
	Texture* mTexture;
	ComPtr<ID3D12Resource> mTexBuff = nullptr;
	ComPtr<ID3D12DescriptorHeap> mTexDescHeap = nullptr;

	// ���W�֘A
	XMMATRIX mWorldMat;
	XMMATRIX mViewMat;
	XMMATRIX mProjMat;
	XMMATRIX* mMapMatrix;


	// �e�X�g�p
	float angle = 0.0f;
};