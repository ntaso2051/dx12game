#include "SpriteRenderer.h"
#include "Dx12Wrapper.h"
#include <random>
#include "Texture.h"
#include "d3dx12.h"
#include "Shader.h"



SpriteRenderer::SpriteRenderer(Dx12Wrapper& dx12Wrapper, Texture* tex) :mDx12Wrapper(dx12Wrapper), mTexture(tex) {
}
SpriteRenderer::~SpriteRenderer() {}

void SpriteRenderer::ReComputeMatrix(XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projMat) {
	*mMapMatrix = worldMat * viewMat * projMat;
}

void SpriteRenderer::InitMatrix(XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projMat) {

}

void SpriteRenderer::Draw() {
	// *mMapMatrix = mWorldMat * mViewMat * mProjMat;
	mDx12Wrapper.CmdList()->SetPipelineState(mDx12Wrapper.GetPipelinestateForSprite().Get());
	mDx12Wrapper.CmdList()->RSSetViewports(1, &mDx12Wrapper.Viewport());
	mDx12Wrapper.CmdList()->RSSetScissorRects(1, &mDx12Wrapper.Scissorrect());
	mDx12Wrapper.CmdList()->SetGraphicsRootSignature(mDx12Wrapper.GetRootsignatureForSprite().Get());
	mDx12Wrapper.CmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDx12Wrapper.CmdList()->IASetVertexBuffers(0, 1, &mDx12Wrapper.GetVbView());
	mDx12Wrapper.CmdList()->IASetIndexBuffer(&mDx12Wrapper.GetIbView());
	mDx12Wrapper.CmdList()->SetGraphicsRootSignature(mDx12Wrapper.GetRootsignatureForSprite().Get());
	mDx12Wrapper.CmdList()->SetDescriptorHeaps(1, mTexDescHeap.GetAddressOf());
	mDx12Wrapper.CmdList()->SetGraphicsRootDescriptorTable(0, mTexDescHeap->GetGPUDescriptorHandleForHeapStart());
	mDx12Wrapper.CmdList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

HRESULT SpriteRenderer::CreateTexture(float windowWidth, float windowHeight) {
	auto metadata = mTexture->GetMetadata();
	auto img = mTexture->GetImgData();
	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;//����Ȑݒ�Ȃ̂�default�ł�upload�ł��Ȃ�
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//���C�g�o�b�N��
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//�]����L0�܂�CPU�����璼��
	texHeapProp.CreationNodeMask = 0;//�P��A�_�v�^�̂���0
	texHeapProp.VisibleNodeMask = 0;//�P��A�_�v�^�̂���0

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = metadata.format;//DXGI_FORMAT_R8G8B8A8_UNORM;//RGBA�t�H�[�}�b�g
	resDesc.Width = static_cast<UINT>(metadata.width);//��
	resDesc.Height = static_cast<UINT>(metadata.height);//����
	resDesc.DepthOrArraySize = static_cast<uint16_t>(metadata.arraySize);//2D�Ŕz��ł��Ȃ��̂łP
	resDesc.SampleDesc.Count = 1;//�ʏ�e�N�X�`���Ȃ̂ŃA���`�F�����Ȃ�
	resDesc.SampleDesc.Quality = 0;//
	resDesc.MipLevels = static_cast<uint16_t>(metadata.mipLevels);//�~�b�v�}�b�v���Ȃ��̂Ń~�b�v���͂P��
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);//2D�e�N�X�`���p
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;//���C�A�E�g�ɂ��Ă͌��肵�Ȃ�
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;//�Ƃ��Ƀt���O�Ȃ�

	auto result = mDx12Wrapper.Device()->CreateCommittedResource(
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
		img->pixels,//���f�[�^�A�h���X
		static_cast<UINT>(img->rowPitch),//1���C���T�C�Y
		static_cast<UINT>(img->slicePitch)//�S�T�C�Y
	);
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NodeMask = 0;//�}�X�N��0
	descHeapDesc.NumDescriptors = 2;//�r���[�͍��̂Ƃ���P����
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//�V�F�[�_���\�[�X�r���[(����ђ萔�AUAV��)
	result = mDx12Wrapper.Device()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(mTexDescHeap.ReleaseAndGetAddressOf()));//����


	// �萔�o�b�t�@�쐬 TODO�F�v���C���[������W��n�����悤�ɂ���D���̓e�X�g�p�ɌŒ�l�D
	XMMATRIX worldMat = XMMatrixScaling(1.0f / img->width, 1.0f / img->height, 1.0f);
	XMFLOAT3 eye(0, 0, -1);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	XMMATRIX viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	XMMATRIX projMat = XMMatrixPerspectiveFovLH(
		XM_PIDIV4,
		static_cast<double>(windowWidth) / static_cast<double>(windowHeight),
		1.0f,//�߂���
		10.0f//������
	);
	ID3D12Resource * constBuff = nullptr;
	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(XMMATRIX) + 0xff) & ~0xff);
	result = mDx12Wrapper.Device()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	result = constBuff->Map(0, nullptr, (void**)& mMapMatrix);//�}�b�v
	*mMapMatrix = worldMat * viewMat * projMat;

	//�ʏ�e�N�X�`���r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = metadata.format;//DXGI_FORMAT_R8G8B8A8_UNORM;//RGBA(0.0f�`1.0f�ɐ��K��)
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//��q
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;//�~�b�v�}�b�v�͎g�p���Ȃ��̂�1

	auto texHeapHandle = mTexDescHeap->GetCPUDescriptorHandleForHeapStart();

	mDx12Wrapper.Device()->CreateShaderResourceView(mTexBuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //��قǐݒ肵���e�N�X�`���ݒ���
		texHeapHandle//�q�[�v�̂ǂ��Ɋ��蓖�Ă邩
	);
	texHeapHandle.ptr += mDx12Wrapper.Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = static_cast<UINT>(constBuff->GetDesc().Width);
	mDx12Wrapper.Device()->CreateConstantBufferView(&cbvDesc, texHeapHandle);

	return result;
}
