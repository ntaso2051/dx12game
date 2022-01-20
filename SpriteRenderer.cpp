#include "SpriteRenderer.h"
#include "Dx12Wrapper.h"
#include <random>

using namespace DirectX;


SpriteRenderer::SpriteRenderer(Dx12Wrapper& dx12Wrapper) :mDx12Wrapper(dx12Wrapper) {
	// �f�o�b�O�p�̃m�C�Y�e�N�X�`�����쐬
	for (int i = 0; i < 256 * 256; i++) {
		TexRGBA temp;
		temp.A = 255;
		temp.R = rand() % 256;
		temp.B = rand() % 256;
		temp.G = rand() % 256;
		noiseTexData.push_back(temp);
	}
}
SpriteRenderer::~SpriteRenderer() {}

void SpriteRenderer::InitView() {
	// ���_�o�b�t�@
	Vertex vertices[] = {
		{{-0.4f,-0.7f,0.0f}, {0.0f, 1.0f}},//����
		{{-0.4f,0.7f,0.0f}, {0.0f, 0.0f}} ,//����
		{{0.4f,-0.7f,0.0f}, {1.0f, 1.0f}} ,//�E��
		{{0.4f,0.7f,0.0f}, {1.0f, 0.0f}} ,//�E��
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

void SpriteRenderer::CompileShader() {
	ID3DBlob* errorBlob = nullptr;
	auto result = D3DCompileFromFile(
		L"Shaders/BasicVertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"BasicVS", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		mVsBlob.ReleaseAndGetAddressOf(),
		&errorBlob
	);
	if (FAILED(result)) {
		assert(0);
	}
	result = D3DCompileFromFile(
		L"Shaders/BasicPixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"BasicPS", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		mPsBlob.ReleaseAndGetAddressOf(),
		&errorBlob
	);
	if (FAILED(result)) {
		assert(0);
	}
}

HRESULT SpriteRenderer::InitRootSignature() {
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* rootSigBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = mDx12Wrapper.Device()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(mRootSignature.ReleaseAndGetAddressOf()));
	rootSigBlob->Release();

	return result;
}

HRESULT SpriteRenderer::InitGraphicPipeline() {
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	gpipeline.pRootSignature = nullptr;
	gpipeline.VS.pShaderBytecode = mVsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = mVsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = mPsBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = mPsBlob->GetBufferSize();

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//���g��0xffffffff

	//
	gpipeline.BlendState.AlphaToCoverageEnable = false;
	gpipeline.BlendState.IndependentBlendEnable = false;

	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};

	//�ЂƂ܂����Z���Z�⃿�u�����f�B���O�͎g�p���Ȃ�
	renderTargetBlendDesc.BlendEnable = false;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//�ЂƂ܂��_�����Z�͎g�p���Ȃ�
	renderTargetBlendDesc.LogicOpEnable = false;

	gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;


	gpipeline.RasterizerState.MultisampleEnable = false;//�܂��A���`�F���͎g��Ȃ�
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//���g��h��Ԃ�
	gpipeline.RasterizerState.DepthClipEnable = true;//�[�x�����̃N���b�s���O�͗L����

	//�c��
	gpipeline.RasterizerState.FrontCounterClockwise = false;
	gpipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	gpipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	gpipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	gpipeline.RasterizerState.AntialiasedLineEnable = false;
	gpipeline.RasterizerState.ForcedSampleCount = 0;
	gpipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


	gpipeline.DepthStencilState.DepthEnable = false;
	gpipeline.DepthStencilState.StencilEnable = false;

	gpipeline.InputLayout.pInputElementDescs = inputLayout;//���C�A�E�g�擪�A�h���X
	gpipeline.InputLayout.NumElements = _countof(inputLayout);//���C�A�E�g�z��

	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//�X�g���b�v���̃J�b�g�Ȃ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//�O�p�`�ō\��

	gpipeline.NumRenderTargets = 1;//���͂P�̂�
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`1�ɐ��K�����ꂽRGBA

	gpipeline.SampleDesc.Count = 1;//�T���v�����O��1�s�N�Z���ɂ��P
	gpipeline.SampleDesc.Quality = 0;//�N�I���e�B�͍Œ�

	gpipeline.pRootSignature = mRootSignature.Get();

	auto result = mDx12Wrapper.Device()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(mPipeline.ReleaseAndGetAddressOf()));

	return result;
}

void SpriteRenderer::Draw() {
	mDx12Wrapper.CmdList()->SetPipelineState(mPipeline.Get());
	mDx12Wrapper.CmdList()->RSSetViewports(1, &mDx12Wrapper.Viewport());
	mDx12Wrapper.CmdList()->RSSetScissorRects(1, &mDx12Wrapper.Scissorrect());
	mDx12Wrapper.CmdList()->SetGraphicsRootSignature(mRootSignature.Get());
	mDx12Wrapper.CmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDx12Wrapper.CmdList()->IASetVertexBuffers(0, 1, &mVbView);
	mDx12Wrapper.CmdList()->IASetIndexBuffer(&mIbView);
	mDx12Wrapper.CmdList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}