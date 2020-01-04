#include "Graphics.h"
#include <vector>
#include "..\\GameManager.h"

#define SCALE_RATIO_X (GameManager::ScaleRatioX())
#define SCALE_RATIO_Y (GameManager::ScaleRatioY())

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	if (!InitializeDirectX(hwnd, width, height))
	{
		return false;
	}

	if (!InitializeShaders())
	{
		return false;
	}

	GameManager::Initialize();

	return true;
}

void Graphics::RenderFrame()
{
	float bgColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), bgColor);

	//BEGIN DRAW REGION
	
	DrawMap();
	DrawTank(1);
	DrawTank(2);

	//END DRAW REGION

	m_SwapChain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwdn, int width, int height)
{
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwdn;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&scd,
		m_SwapChain.GetAddressOf(),
		m_Device.GetAddressOf(),
		NULL,
		m_DeviceContext.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(backBuffer.GetAddressOf()));
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_Device->CreateRenderTargetView(backBuffer.Get(), NULL, m_RenderTargetView.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}

	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	m_DeviceContext->RSSetViewports(1, &viewport);

	return true;
}

bool Graphics::InitializeShaders()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,0,0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!m_VertexShader.Initialize(m_Device, L"..\\x64\\Debug\\vertexshader.cso",layout,numElements))
	{
		return false;
	}

	if (!m_PixelShader.Initialize(m_Device , L"..\\x64\\Debug\\pixelshader.cso"))
	{
		return false;
	}

	return true;
}

void Graphics::DrawTank(int player)
{
	float scalex = SCALE_RATIO_X /1.4f;
	float scaley =  SCALE_RATIO_Y/  0.5f;

	Vector2f playerPosition = GameManager::GetPlayerPosition(player);
	float playerX = playerPosition.GetX();
	float playerY = playerPosition.GetY();
	std::vector<Vertex> v
	{
		Vertex(-0.7f*scalex + playerX ,0.0f*scaley + playerY),
		Vertex(-0.7f*scalex + playerX,0.2f*scaley + playerY),
		Vertex(-0.5f*scalex + playerX,0.2f*scaley + playerY),
		Vertex(-0.5f*scalex + playerX,0.5f*scaley + playerY),
		Vertex(0.0f*scalex + playerX,0.5f*scaley + playerY),
		Vertex(0.0f*scalex + playerX,0.4f*scaley + playerY),
		Vertex(0.7f*scalex + playerX,0.4f*scaley + playerY),
		Vertex(0.7f*scalex + playerX,0.3f*scaley + playerY),
		Vertex(0.0f*scalex + playerX,0.3f*scaley + playerY),
		Vertex(0.0f*scalex + playerX,0.2f*scaley + playerY),
		Vertex(0.2f*scalex + playerX,0.2f*scaley + playerY),
		Vertex(0.2f*scalex + playerX,0.0f*scaley + playerY),
		Vertex(-0.7f*scalex + playerX,0.0f*scaley + playerY)
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * v.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v.data();

	HRESULT hr = m_Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_VertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log("Failed to draw tank");
	}

	m_DeviceContext->IASetInputLayout(m_VertexShader.GetInputLayout());
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	m_DeviceContext->VSSetShader(m_VertexShader.GetShader(), NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);


	m_DeviceContext->Draw(v.size() , 0);
	m_VertexBuffer->Release();
}

void Graphics::DrawMap()
{
	const int n = GameManager::GetMapN();
	const int m = GameManager::GetMapM();
	for (unsigned i = 0u; i < n; i++)
	{
		for (unsigned j = 0u; j < m; j++)
		{
			if (GameManager::GetGridValue(i, j))
			{
				DrawGridPart(i, j);
			}
		}
	}
}

void Graphics::DrawGridPart(int i,int j)
{
	i -= GameManager::GetMapN()/2;
	j -= GameManager::GetMapM()/2;
	float scaleRatioX = SCALE_RATIO_X;
	float scaleRatioY = SCALE_RATIO_Y;
	Vertex gridPart[] = {
		Vertex(i*scaleRatioX, j*scaleRatioY),
		Vertex((i+1)*scaleRatioX, j*scaleRatioY),
		Vertex((i+1) * scaleRatioX, (j+1) *scaleRatioY),
		Vertex(i*scaleRatioX,(j+1)*scaleRatioY),
		Vertex(i*scaleRatioX,j*scaleRatioY)
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(gridPart);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = gridPart;

	HRESULT hr = m_Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_VertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log("Failed to draw tank");
	}
	
	m_DeviceContext->IASetInputLayout(m_VertexShader.GetInputLayout());
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	m_DeviceContext->VSSetShader(m_VertexShader.GetShader(), NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);


	m_DeviceContext->Draw(ARRAYSIZE(gridPart), 0);
	
	m_VertexBuffer->Release();
}


