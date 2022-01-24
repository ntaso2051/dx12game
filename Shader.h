#pragma once
#include <wrl.h>
#include <d3d12.h>

class Shader {
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Shader();
	~Shader();
	void CompileShader();
	ComPtr<ID3DBlob> GetVsBlob() { return mVsBlob; }
	ComPtr<ID3DBlob> GetPsBlob() { return mPsBlob; }
private:
	// シェーダー関連
	ComPtr<ID3DBlob> mVsBlob = nullptr;
	ComPtr<ID3DBlob> mPsBlob = nullptr;
};