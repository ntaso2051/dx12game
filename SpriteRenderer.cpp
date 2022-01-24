#include "SpriteRenderer.h"
#include "Dx12Wrapper.h"
#include <random>
#include "Texture.h"
#include "d3dx12.h"
#include "Shader.h"



SpriteRenderer::SpriteRenderer(Dx12Wrapper& dx12Wrapper) :mDx12Wrapper(dx12Wrapper) {
}
SpriteRenderer::~SpriteRenderer() {}

void SpriteRenderer::ReComputeMatrix(XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projMat) {
	mWorldMat = worldMat;
	mViewMat = viewMat;
	mProjMat = projMat;
}

void SpriteRenderer::InitMatrix(XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projMat) {

}

void SpriteRenderer::InitView(float windowWidth, float windowHeight, const wchar_t* filename) {
	mTexture = new Texture();
	if (FAILED(mTexture->LoadImgFile(filename))) {
		assert(0);
	}
	// ���_�o�b�t�@
	Vertex vertices[] = {
		{{-1.0f,-1.0f,0.0f}, {0.0f, 1.0f}},//����
		{{-1.0f,1.0f,0.0f}, {0.0f, 0.0f}} ,//����
		{{1.0f,-1.0f,0.0f}, {1.0f, 1.0f}} ,//�E��
		{{1.0f,1.0f,0.0f}, {1.0f, 0.0f}} ,//�E��
	};
	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resdesc = {};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeof(vertices);
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.Format = DXGI_FORMAT_UNKNOWN;
	resdesc.SampleDesc.Count = 1;
	resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto result = mDx12Wrapper.Device()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mVb.ReleaseAndGetAddressOf())
	);
	Vertex* vertMap = nullptr;
	result = mVb->Map(0, nullptr, (void**)& vertMap);
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	mVb->Unmap(0, nullptr);

	mVbView.BufferLocation = mVb->GetGPUVirtualAddress();
	mVbView.SizeInBytes = sizeof(vertices);
	mVbView.StrideInBytes = sizeof(vertices[0]);

	// �C���f�b�N�X�o�b�t�@
	unsigned short indices[] = { 0,1,2, 2,1,3 };

	//�ݒ�́A�o�b�t�@�̃T�C�Y�ȊO���_�o�b�t�@�̐ݒ���g���܂킵��
	//OK���Ǝv���܂��B
	resdesc.Width = sizeof(indices);
	result = mDx12Wrapper.Device()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mIb.ReleaseAndGetAddressOf()));

	//������o�b�t�@�ɃC���f�b�N�X�f�[�^���R�s�[
	unsigned short* mappedIdx = nullptr;
	mIb->Map(0, nullptr, (void**)& mappedIdx);
	std::copy(std::begin(indices), std::end(indices), mappedIdx);
	mIb->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[���쐬
	mIbView.BufferLocation = mIb->GetGPUVirtualAddress();
	mIbView.Format = DXGI_FORMAT_R16_UINT;
	mIbView.SizeInBytes = sizeof(indices);
}



void SpriteRenderer::Draw() {
	*mMapMatrix = mWorldMat * mViewMat * mProjMat;
	mDx12Wrapper.CmdList()->SetPipelineState(mDx12Wrapper.GetPipelinestateForSprite().Get());
	mDx12Wrapper.CmdList()->RSSetViewports(1, &mDx12Wrapper.Viewport());
	mDx12Wrapper.CmdList()->RSSetScissorRects(1, &mDx12Wrapper.Scissorrect());
	mDx12Wrapper.CmdList()->SetGraphicsRootSignature(mDx12Wrapper.GetRootsignatureForSprite().Get());
	mDx12Wrapper.CmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDx12Wrapper.CmdList()->IASetVertexBuffers(0, 1, &mVbView);
	mDx12Wrapper.CmdList()->IASetIndexBuffer(&mIbView);
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
	mWorldMat = XMMatrixScaling(1.0f / img->width, 1.0f / img->height, 1.0f);
	XMFLOAT3 eye(0, 0, -1);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	mViewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	mProjMat = XMMatrixPerspectiveFovLH(
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
	*mMapMatrix = mWorldMat * mViewMat * mProjMat;

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

D3D12_HEAP_PROPERTIES SpriteRenderer::GetHeapPropForTex() {
	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapprop.CreationNodeMask = 0;
	heapprop.VisibleNodeMask = 0;
	return heapprop;
}

D3D12_RESOURCE_DESC SpriteRenderer::GetResourseDescForTex() {
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resDesc.Width = 256;
	resDesc.Height = 256;
	resDesc.DepthOrArraySize = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.MipLevels = 1;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	return resDesc;
}