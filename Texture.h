#pragma once
#include <vector>
#include <d3d12.h>

class Dx12Wrapper;
class Texture {
public:
	Texture(Dx12Wrapper& dx12);
	struct TexRGBA {
		unsigned char R, G, B, A;
	};
	TexRGBA* GetNoiseTexData() { return mNoiseTexData.data(); }
	int GetHeight() { return mHeight; }
	int GetWidth() { return mWidth; }
private:
	Dx12Wrapper& mDx12Wrapper;
	// デバッグ用のノイズテクスチャ
	std::vector<TexRGBA> mNoiseTexData;
	int mHeight;
	int mWidth;
};