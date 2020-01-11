#include "Graphics.h"
#include <vector>
#include "..\\GameManager.h"
#include <DirectXMath.h>

#define SCALE_RATIO_X (GameManager::ScaleRatioX())
#define SCALE_RATIO_Y (GameManager::ScaleRatioY())

#define PI (atan(1.0f)*4.0f)

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

	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(m_DeviceContext.Get());
	m_SpriteFont = std::make_unique<DirectX::SpriteFont>(m_Device.Get(),L"Fonts\\stats.spritefont");

	m_Data = std::vector<Vertex>(60000);
	GameManager::Initialize();
	UpdateMapState();



	return true;
}

void Graphics::RenderFrame()
{
	float bgColor[] = { 135.f/255.0f,206.0f/255.0f,250.0f/255.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), bgColor);

	//BEGIN DRAW REGION
	
	DrawMap();
	DrawTank(1);
	DrawTank(2);

	if (GameManager::Projectile())
	{
		DrawProjectile();
		if (GameManager::CheckCollision())
		{
			UpdateMapState();
			GameManager::UpdateTerrainOutline();
		}
	}
	DrawStats();
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

	m_ViewHeight = height;
	m_ViewWidth = width;

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

void Graphics::UpdateMapState()
{
	const unsigned n = GameManager::GetMapN();
	const unsigned m = GameManager::GetMapM();
	float scaleRatioX = SCALE_RATIO_X;
	float scaleRatioY = SCALE_RATIO_Y;
	unsigned vectorSize = 0;
	for (unsigned i = 0u; i < n; i++)
	{
		for (unsigned j = 0u; j < m; j++)
		{
			if (GameManager::GetGridValue(i, j))
			{
				int ii = i - n / 2;
				int jj = j - m / 2;

				Vertex gridPart[] =
				{
					Vertex(ii*scaleRatioX, jj*scaleRatioY,0,0,1),
					Vertex(ii*scaleRatioX,(jj + 1)*scaleRatioY,0,0,1),
					Vertex((ii + 1) * scaleRatioX, (jj + 1) *scaleRatioY,0,0,1),

					Vertex(ii*scaleRatioX, jj*scaleRatioY,0,0,1),
					Vertex((ii + 1) * scaleRatioX, (jj + 1) *scaleRatioY,0,0,1),
					Vertex((ii + 1)*scaleRatioX, jj*scaleRatioY,0,0,1),
				};
				for (int k = 0; k < 6; k++)
				{
					m_Data[vectorSize] = gridPart[k];
					vectorSize++;
				}
			}
		}
	}
	m_DataSize = vectorSize;
}

void Graphics::DrawStats()
{
	std::string player1Name = GameManager::GetPlayerName(1);
	std::string player2Name = GameManager::GetPlayerName(1);
	int player1Health = GameManager::GetPlayerHealth(1);
	int player2Health = GameManager::GetPlayerHealth(2);
	int currentPlayer = GameManager::GetCurrentPlayer();
	double playerAngle = GameManager::GetPlayerAngle() * 180 / PI;
	double playerPower = GameManager::GetPlayerPower();
	std::string playerBarStr = "Player: ";
	std::string playerNameStr = "Name : ";
	std::string playerHealthStr = "Health: ";
	std::string playerAngleStr = "Angle: ";
	std::string playerPowerStr = "Power: ";

	DrawTextOnScreen(playerBarStr, Vector2f(20, 20));
	DrawTextOnScreen(playerNameStr + player1Name, Vector2f(20, 40));
	DrawTextOnScreen(playerHealthStr + std::to_string(player1Health), Vector2f(20, 60));
	if (currentPlayer == 1)
	{
		DrawTextOnScreen(playerAngleStr + std::to_string(playerAngle), Vector2f(20, 80));
		DrawTextOnScreen(playerPowerStr + std::to_string(playerPower), Vector2f(20, 100));
	}

	DrawTextOnScreen(playerBarStr, Vector2f(m_ViewWidth-150, 20));
	DrawTextOnScreen(playerNameStr + player2Name, Vector2f(m_ViewWidth - 150, 40));
	DrawTextOnScreen(playerHealthStr + std::to_string(player2Health), Vector2f(m_ViewWidth - 150, 60));
	if (currentPlayer == 2)
	{
		DrawTextOnScreen(playerAngleStr + std::to_string(playerAngle), Vector2f(m_ViewWidth - 150, 80));
		DrawTextOnScreen(playerPowerStr + std::to_string(playerPower), Vector2f(m_ViewWidth - 150, 100));
	}

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
	int playerOnMove = GameManager::GetCurrentPlayer();
	Vector2f playerPosition = GameManager::GetPlayerPosition(playerOnMove);
	Vector2f playerSize = GameManager::GetPlayerSize(playerOnMove);

	Vertex line []
	{
		Vertex(playerPosition.GetX(),playerPosition.GetY() + playerSize.GetY(), 1.0, 0.0, 0.0),
		Vertex(0.1*std::cos(angle) + playerPosition.GetX(),0.1*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
		Vertex(0.15*std::cos(angle) + playerPosition.GetX(),0.15*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
		Vertex(0.20*std::cos(angle) + playerPosition.GetX(),0.20*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
		Vertex(0.25*std::cos(angle) + playerPosition.GetX(),0.25*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
		Vertex(0.30*std::cos(angle) + playerPosition.GetX(),0.30*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
		Vertex(0.35*std::cos(angle) + playerPosition.GetX(),0.35*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
		Vertex(0.40*std::cos(angle) + playerPosition.GetX(),0.40*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
		Vertex(0.45*std::cos(angle) + playerPosition.GetX(),0.45*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
		Vertex(0.50*std::cos(angle) + playerPosition.GetX(),0.50*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
		Vertex(0.55*std::cos(angle) + playerPosition.GetX(),0.55*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0)
	};
	DrawShape(line, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST, ARRAYSIZE(line));
}

void Graphics::DrawShape(Vertex array[], D3D11_PRIMITIVE_TOPOLOGY primitiveTopology,unsigned arraySize)
{
	m_DeviceContext->IASetPrimitiveTopology(primitiveTopology);
	DrawShape(array,arraySize);
}

void Graphics::DrawTextOnScreen(std::string text, Vector2f position)
{
	const std::wstring s = StringConverter::StringToWide(text);
	m_SpriteBatch->Begin();
	m_SpriteFont->DrawString(m_SpriteBatch.get(),s.data(), DirectX::XMFLOAT2(position.GetX(), position.GetY()), DirectX::Colors::Black);
	m_SpriteBatch->End();
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
	};

	DrawShape(base, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, ARRAYSIZE(base));
	DrawShape(top, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, ARRAYSIZE(top));
	DrawShape(turret, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, ARRAYSIZE(turret));
	DrawMouseIndicator();
}

void Graphics::DrawMap()
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_DataSize;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = m_Data.data();

	HRESULT hr = m_Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_VertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log("Failed to initialize vertex buffer");
	}
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->IASetInputLayout(m_VertexShader.GetInputLayout());
	m_DeviceContext->VSSetShader(m_VertexShader.GetShader(), NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	m_DeviceContext->Draw(4, 0);
	for (unsigned i = 0; i < m_DataSize; i += 3)
	{
		m_DeviceContext->Draw(3, i);
	}
	m_VertexBuffer->Release();
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