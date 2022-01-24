#include <d3dcompiler.h>
#include "Shader.h"
#include <cassert>
#include <d3dx12.h>

Shader::Shader() {

}

Shader::~Shader() {

}

void Shader::CompileShader() {
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