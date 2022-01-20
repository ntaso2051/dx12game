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
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};
	struct TexRGBA {
		unsigned char R, G, B, A;
	};
	Dx12Wrapper& mDx12Wrapper;
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12PipelineState> mPipeline = nullptr;
	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	// �V�F�[�_�[�֘A
	ComPtr<ID3DBlob> mVsBlob = nullptr;
	ComPtr<ID3DBlob> mPsBlob = nullptr;
	//���_�֘A
	ComPtr<ID3D12Resource> mVb = nullptr;
	ComPtr<ID3D12Resource> mIb = nullptr;
	D3D12_VERTEX_BUFFER_VIEW mVbView = {};
	D3D12_INDEX_BUFFER_VIEW mIbView = {};

	// �f�o�b�O�p�̃m�C�Y�e�N�X�`��
	std::vector<TexRGBA> noiseTexData;
};