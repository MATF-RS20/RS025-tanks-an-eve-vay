#pragma once
#include "..\\ErrorLogger.h"
#pragma comment(lib,"D3DCompiler.lib")
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

class VertexShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDescription, UINT numElements);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_Shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_ShaderBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

};

class PixelShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaderPath);
	ID3D11PixelShader* GetShader();
	ID3D10Blob* GetBuffer();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_Shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_ShaderBuffer = nullptr;

};