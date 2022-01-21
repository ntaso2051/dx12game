#pragma once
#include <vector>
#include <d3d12.h>
#include "DirectXTex.h"
#include<string>

#pragma comment (lib, "DirectXTex.lib")

using namespace DirectX;

class Texture {
public:
	Texture();
	struct TexRGBA {
		unsigned char R, G, B, A;
	};
	TexRGBA* GetNoiseTexData() { return mNoiseTexData.data(); }
	int GetHeight() { return mHeight; }
	int GetWidth() { return mWidth; }

	const Image* GetImgData() { return mScratchImg.GetImage(0, 0, 0); }
	const TexMetadata GetMetadata() { return mMetadata; }

	HRESULT LoadImgFile(const wchar_t* filename);
private:
	// デバッグ用のノイズテクスチャ
	std::vector<TexRGBA> mNoiseTexData;
	int mHeight;
	int mWidth;

	TexMetadata mMetadata;
	ScratchImage mScratchImg;
};