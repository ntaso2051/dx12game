#include "Texture.h"
#include <cassert>
#include "Game.h"
#include <d3dx12.h>

Texture::Texture(Game* game) :mGame(game), mHeight(256), mWidth(256) {
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

HRESULT Texture::LoadImgFile(const wchar_t* filename) {
	auto result = LoadFromWICFile(filename, WIC_FLAGS_NONE, &mMetadata, mScratchImg);
	return result;
}


HRESULT Texture::CreateTexture(float windowWidth, float windowHeight) {
	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;//����Ȑݒ�Ȃ̂�default�ł�upload�ł��Ȃ�
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//���C�g�o�b�N��
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//�]����L0�܂�CPU�����璼��
	texHeapProp.CreationNodeMask = 0;//�P��A�_�v�^�̂���0
	texHeapProp.VisibleNodeMask = 0;//�P��A�_�v�^�̂���0

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = mMetadata.format;//DXGI_FORMAT_R8G8B8A8_UNORM;//RGBA�t�H�[�}�b�g
	resDesc.Width = static_cast<UINT>(mMetadata.width);//��
	resDesc.Height = static_cast<UINT>(mMetadata.height);//����
	resDesc.DepthOrArraySize = static_cast<uint16_t>(mMetadata.arraySize);//2D�Ŕz��ł��Ȃ��̂łP
	resDesc.SampleDesc.Count = 1;//�ʏ�e�N�X�`���Ȃ̂ŃA���`�F�����Ȃ�
	resDesc.SampleDesc.Quality = 0;//
	resDesc.MipLevels = static_cast<uint16_t>(mMetadata.mipLevels);//�~�b�v�}�b�v���Ȃ��̂Ń~�b�v���͂P��
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(mMetadata.dimension);//2D�e�N�X�`���p
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;//���C�A�E�g�ɂ��Ă͌��肵�Ȃ�
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;//�Ƃ��Ƀt���O�Ȃ�

	auto result = mGame->GetDx12().Device()->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,//���Ɏw��Ȃ�
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,//�e�N�X�`���p(�s�N�Z���V�F�[�_���猩��p)
		nullptr,
		IID_PPV_ARGS(mTexBuff.ReleaseAndGetAddressOf())
	);

	result = mTexBuff->WriteToSubresource(
		0,
		nullptr,//�S�̈�փR�s�[
		GetImgData()->pixels,//���f�[�^�A�h���X
		static_cast<UINT>(GetImgData()->rowPitch),//1���C���T�C�Y
		static_cast<UINT>(GetImgData()->slicePitch)//�S�T�C�Y
	);
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NodeMask = 0;//�}�X�N��0
	descHeapDesc.NumDescriptors = 2;//�r���[�͍��̂Ƃ���P����
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//�V�F�[�_���\�[�X�r���[(����ђ萔�AUAV��)
	result = mGame->GetDx12().Device()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(mTexDescHeap.ReleaseAndGetAddressOf()));//����


	//�ʏ�e�N�X�`���r���[�쐬
	mSrvDesc.Format = mMetadata.format;//DXGI_FORMAT_R8G8B8A8_UNORM;//RGBA(0.0f�`1.0f�ɐ��K��)
	mSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//��q
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	mSrvDesc.Texture2D.MipLevels = 1;//�~�b�v�}�b�v�͎g�p���Ȃ��̂�1

	return result;
}