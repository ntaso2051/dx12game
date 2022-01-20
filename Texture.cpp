#include "Texture.h"
#include "Dx12Wrapper.h"
#include <cassert>

Texture::Texture(Dx12Wrapper& dx12) :mDx12Wrapper(dx12), mHeight(256), mWidth(256) {
	// �f�o�b�O�p�̃m�C�Y�e�N�X�`�����쐬
	for (int i = 0; i < 256 * 256; i++) {
		TexRGBA temp;
		temp.A = 255;
		temp.R = rand() % 256;
		temp.B = rand() % 256;
		temp.G = rand() % 256;
		mNoiseTexData.push_back(temp);
	}
}


