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
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;//特殊な設定なのでdefaultでもuploadでもなく
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//ライトバックで
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//転送がL0つまりCPU側から直で
	texHeapProp.CreationNodeMask = 0;//単一アダプタのため0
	texHeapProp.VisibleNodeMask = 0;//単一アダプタのため0

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = metadata.format;//DXGI_FORMAT_R8G8B8A8_UNORM;//RGBAフォーマット
	resDesc.Width = static_cast<UINT>(metadata.width);//幅
	resDesc.Height = static_cast<UINT>(metadata.height);//高さ
	resDesc.DepthOrArraySize = static_cast<uint16_t>(metadata.arraySize);//2Dで配列でもないので１
	resDesc.SampleDesc.Count = 1;//通常テクスチャなのでアンチェリしない
	resDesc.SampleDesc.Quality = 0;//
	resDesc.MipLevels = static_cast<uint16_t>(metadata.mipLevels);//ミップマップしないのでミップ数は１つ
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);//2Dテクスチャ用
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;//レイアウトについては決定しない
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;//とくにフラグなし

	auto result = mDx12Wrapper.Device()->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,//特に指定なし
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,//テクスチャ用(ピクセルシェーダから見る用)
		nullptr,
		IID_PPV_ARGS(mTexBuff.ReleaseAndGetAddressOf())
	);

	result = mTexBuff->WriteToSubresource(
		0,
		nullptr,//全領域へコピー
		img->pixels,//元データアドレス
		static_cast<UINT>(img->rowPitch),//1ラインサイズ
		static_cast<UINT>(img->slicePitch)//全サイズ
	);
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NodeMask = 0;//マスクは0
	descHeapDesc.NumDescriptors = 2;//ビューは今のところ１つだけ
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//シェーダリソースビュー(および定数、UAVも)
	result = mDx12Wrapper.Device()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(mTexDescHeap.ReleaseAndGetAddressOf()));//生成


	// 定数バッファ作成 TODO：プレイヤーから座標を渡されるようにする．今はテスト用に固定値．
	XMMATRIX worldMat = XMMatrixScaling(1.0f / img->width, 1.0f / img->height, 1.0f);
	XMFLOAT3 eye(0, 0, -1);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	XMMATRIX viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	XMMATRIX projMat = XMMatrixPerspectiveFovLH(
		XM_PIDIV4,
		static_cast<double>(windowWidth) / static_cast<double>(windowHeight),
		1.0f,//近い方
		10.0f//遠い方
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

	result = constBuff->Map(0, nullptr, (void**)& mMapMatrix);//マップ
	*mMapMatrix = worldMat * viewMat * projMat;

	//通常テクスチャビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = metadata.format;//DXGI_FORMAT_R8G8B8A8_UNORM;//RGBA(0.0f～1.0fに正規化)
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//後述
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;//ミップマップは使用しないので1

	auto texHeapHandle = mTexDescHeap->GetCPUDescriptorHandleForHeapStart();

	mDx12Wrapper.Device()->CreateShaderResourceView(mTexBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //先ほど設定したテクスチャ設定情報
		texHeapHandle//ヒープのどこに割り当てるか
	);
	texHeapHandle.ptr += mDx12Wrapper.Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = static_cast<UINT>(constBuff->GetDesc().Width);
	mDx12Wrapper.Device()->CreateConstantBufferView(&cbvDesc, texHeapHandle);

	return result;
}
