#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDescription, UINT numElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), m_ShaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	hr = device->CreateVertexShader(m_ShaderBuffer->GetBufferPointer(), m_ShaderBuffer->GetBufferSize(), NULL, m_Shader.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}


	hr = device->CreateInputLayout(layoutDescription, numElements, m_ShaderBuffer->GetBufferPointer(), m_ShaderBuffer->GetBufferSize(), m_InputLayout.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}


	return true;
}

ID3D11VertexShader * VertexShader::GetShader()
{
	return m_Shader.Get();
}

ID3D10Blob * VertexShader::GetBuffer()
{
	return m_ShaderBuffer.Get();
}

ID3D11InputLayout * VertexShader::GetInputLayout()
{
	return m_InputLayout.Get();
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), m_ShaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	hr = device->CreatePixelShader(m_ShaderBuffer->GetBufferPointer(), m_ShaderBuffer->GetBufferSize(), NULL, m_Shader.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

ID3D11PixelShader * PixelShader::GetShader()
{
	return m_Shader.Get();
}

ID3D10Blob * PixelShader::GetBuffer()
{
	return m_ShaderBuffer.Get();
}
