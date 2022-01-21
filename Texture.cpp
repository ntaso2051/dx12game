#include "Texture.h"
#include <cassert>

Texture::Texture() :mHeight(256), mWidth(256) {
	// デバッグ用のノイズテクスチャを作成
	for (int i = 0; i < 256 * 256; i++) {
		TexRGBA temp;
		temp.A = 255;
		temp.R = rand() % 256;
		temp.B = rand() % 256;
		temp.G = rand() % 256;
		mNoiseTexData.push_back(temp);
	}
}

HRESULT Texture::LoadImgFile(const wchar_t* filename) {
	auto result = LoadFromWICFile(filename, WIC_FLAGS_NONE, &mMetadata, mScratchImg);
	return result;
}

