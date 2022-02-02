#pragma once
#include <vector>
#include <d3d12.h>
#include "DirectXTex.h"
#include<string>
#include <wrl.h>

#pragma comment (lib, "DirectXTex.lib")

using namespace DirectX;
template<typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class Texture {
public:
	Texture(class Game* game);
	struct TexRGBA {
		unsigned char R, G, B, A;
	};
	TexRGBA* GetNoiseTexData() { return mNoiseTexData.data(); }
	int GetHeight() { return mHeight; }
	int GetWidth() { return mWidth; }

	const Image* GetImgData() { return mScratchImg.GetImage(0, 0, 0); }
	const TexMetadata GetMetadata() { return mMetadata; }

	HRESULT LoadImgFile(const wchar_t* filename);
	HRESULT CreateTexture(float windowWidth, float windowHeight);
	ComPtr<ID3D12Resource> GetTexBuff() { return mTexBuff; }
	ComPtr<ID3D12DescriptorHeap> GetTexDescHeap() { return mTexDescHeap; }
	D3D12_SHADER_RESOURCE_VIEW_DESC GetSrvDesc() { return mSrvDesc; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() { return mTexDescHeap->GetCPUDescriptorHandleForHeapStart(); }
private:
	// デバッグ用のノイズテクスチャ
	std::vector<TexRGBA> mNoiseTexData;
	int mHeight;
	int mWidth;
	class Game* mGame;

	ComPtr<ID3D12Resource> mTexBuff = nullptr;
	ComPtr<ID3D12DescriptorHeap> mTexDescHeap = nullptr;
	D3D12_SHADER_RESOURCE_VIEW_DESC mSrvDesc;
	TexMetadata mMetadata;
	ScratchImage mScratchImg;
};