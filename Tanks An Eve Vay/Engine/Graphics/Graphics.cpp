#include "Graphics.h"
#include <vector>
#include "..\\GameManager.h"

#define SCALE_RATIO_X (GameManager::ScaleRatioX())
#define SCALE_RATIO_Y (GameManager::ScaleRatioY())


#define Rsin(x,y,angle) (std::sqrt(x*x+y*y) * std::sin(angle + std::abs(std::atan(y/x))))
#define Rcos(x,y,angle) (std::sqrt(x*x+y*y) * std::cos(angle + std::abs(std::atan(y/x))))

#define Rotx(x,y,angle) (x*std::cos(angle) - y*std::sin(angle))
#define Roty(x,y,angle) (x*std::sin(angle) + y*std::cos(angle))
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
	
	//DrawMap();
	DrawTank(1);
	DrawTank(2);

	if (GameManager::Projectile())
	{
		DrawProjectile();
	}

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
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);

	m_DeviceContext->RSSetViewports(1, &viewport);

	return true;
}

bool Graphics::InitializeShaders()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,0,0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0}
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

void Graphics::DrawShape(Vertex array[],unsigned arraySize)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * arraySize;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = array;

	HRESULT hr = m_Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_VertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log("Failed to draw object");
	}

	m_DeviceContext->IASetInputLayout(m_VertexShader.GetInputLayout());

	m_DeviceContext->VSSetShader(m_VertexShader.GetShader(), NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	m_DeviceContext->Draw(arraySize, 0);
	m_VertexBuffer->Release();
}

void Graphics::DrawMouseIndicator()
{
	double angle = GameManager::GetPlayerAngle();
	Vector2f playerPosition = GameManager::GetPlayerPosition(1);
	Vector2f playerSize = GameManager::GetPlayerSize(1);

	Vertex line []
	{
		Vertex(playerPosition.GetX(),playerPosition.GetY() + playerSize.GetY()),
		Vertex(0.5*std::cos(angle) + playerPosition.GetX(),0.5*std::sin(angle)+ playerPosition.GetY())
	};
	DrawShape(line, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST, ARRAYSIZE(line));
}

void Graphics::DrawShape(Vertex array[], D3D11_PRIMITIVE_TOPOLOGY primitiveTopology,unsigned arraySize)
{
	m_DeviceContext->IASetPrimitiveTopology(primitiveTopology);
	DrawShape(array,arraySize);
}

void Graphics::DrawTank(int player)
{
	float scalex = 0.1 / 1.4f;
	float scaley = 0.1 / 0.5f;

	Vector2f playerPosition = GameManager::GetPlayerPosition(player);
	float playerX = static_cast<float>(playerPosition.GetX());
	float playerY = static_cast<float>(playerPosition.GetY());

	Vertex base[]
	{
		Vertex(-0.7f*scalex + playerX ,0.0f*scaley + playerY),
		Vertex(-0.7f*scalex + playerX,0.2f*scaley + playerY),
		Vertex(0.2f*scalex + playerX,0.2f*scaley + playerY),

		Vertex(-0.7f*scalex + playerX ,0.0f*scaley + playerY),
		Vertex(0.2f*scalex + playerX,0.2f*scaley + playerY),
		Vertex(0.2f*scalex + playerX,0.0f*scaley + playerY),
	};

	Vertex top[]
	{
		Vertex(-0.5f*scalex + playerX,0.2f*scaley + playerY),
		Vertex(-0.5f*scalex + playerX,0.4f*scaley + playerY),
		Vertex(0.0f*scalex + playerX,0.4f*scaley + playerY),

		Vertex(-0.5f*scalex + playerX,0.2f*scaley + playerY),
		Vertex(0.0f*scalex + playerX,0.4f*scaley + playerY),
		Vertex(0.0f*scalex + playerX,0.2f*scaley + playerY)
	};

#ifdef DEBUG
	Vertex system[] 
	{
		Vertex(-1,0),
		Vertex(1,0),
		Vertex(0,-1),
		Vertex(0,1)
	};
	DrawShape(system, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST,ARRAYSIZE(system));
#endif
	//TODO: Fix to drawing that are more appropriate for rotation

	double angle = GameManager::GetPlayerAngle();
	//double fixScalex = scalex;
	//double fixScaley = scaley;
	//scaley *= std::sin(angle);
	//scalex *= std::cos(angle);
	
	double angleSin = std::sin(angle);
	double angleCos = std::cos(angle);

	Vertex turret[]
	{
		Vertex(-0.2f*scalex + playerX,0.4f*scaley + playerY,1,0,0),
		Vertex(0.5f*scalex + playerX,0.4f*scaley + playerY,1,0,0),
		Vertex(-0.2f*scalex + playerX,0.3f*scaley + playerY,1,0,0),

		Vertex(-0.2f*scalex + playerX,0.3f*scaley + playerY,1,0,0),
		Vertex(0.5f*scalex + playerX,0.4f*scaley + playerY,1,0,0),
		Vertex(0.5f*scalex + playerX,0.3f*scaley + playerY,1,0,0),

		/*Vertex(Rotx(-0.1,0.0,angle),Roty(-0.1,0.0,angle)),
		Vertex(Rotx(-0.1,0.1,angle),Roty(-0.1,0.1,angle)),
		Vertex(Rotx(0.1,0.1,angle),Roty(0.1,0.1,angle)),

		Vertex(Rotx(0.1,0.1,angle),Roty(0.1,0.1,angle)),
		Vertex(Rotx(0.1,0.0,angle),Roty(0.1,0.0,angle)),
		Vertex(Rotx(-0.1,0.0,angle),Roty(-0.1,0.0,angle))*/

	};

	DrawShape(base, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, ARRAYSIZE(base));
	DrawShape(top, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, ARRAYSIZE(top));
	DrawShape(turret, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, ARRAYSIZE(turret));
	DrawMouseIndicator();
}

void Graphics::DrawMap()
{
	const unsigned n = GameManager::GetMapN();
	const unsigned m = GameManager::GetMapM();
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
	Vertex gridPart[] =
	{
		Vertex(i*scaleRatioX, j*scaleRatioY,0,0,1),
		Vertex(i*scaleRatioX,(j + 1)*scaleRatioY,0,0,1),
		Vertex((i + 1) * scaleRatioX, (j + 1) *scaleRatioY,0,0,1),

		Vertex(i*scaleRatioX, j*scaleRatioY,0,0,1),
		Vertex((i + 1) * scaleRatioX, (j + 1) *scaleRatioY,0,0,1),
		Vertex((i+1)*scaleRatioX, j*scaleRatioY,0,0,1),
	};
	DrawShape(gridPart, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST,ARRAYSIZE(gridPart));
}

void Graphics::DrawProjectile()
{
	Vector2f position = GameManager::GetProjectilePosition();
	double x = position.GetX();
	double y = position.GetY();

	Vector2f size = GameManager::GetProjectileSize();
	double w = size.GetX();
	double h = size.GetY();
	
	Vertex v[] = {
		Vertex(x-w/2, y-h/2),
		Vertex(x+w/2,y-h/2),
		Vertex(x+w/2,y+h/2),
		Vertex(x-w/2,y+h/2),
		Vertex(x-w/2,y-h/2)
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;

	HRESULT hr = m_Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_VertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log("Failed to draw Projectile");
	}

	m_DeviceContext->IASetInputLayout(m_VertexShader.GetInputLayout());
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	m_DeviceContext->VSSetShader(m_VertexShader.GetShader(), NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	m_DeviceContext->Draw(ARRAYSIZE(v), 0);

	m_VertexBuffer->Release();
}


