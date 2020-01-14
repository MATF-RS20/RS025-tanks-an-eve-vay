#include "Graphics.h"
#include <vector>
#include "..\\GameManager.h"
#include <DirectXMath.h>
#include <WinUser.h>

#define SCALE_RATIO_X (GameManager::ScaleRatioX())
#define SCALE_RATIO_Y (GameManager::ScaleRatioY())

#define PI (atan(1.0f)*4.0f)
//#define DEBUG
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
	m_SpriteFont = std::make_unique<DirectX::SpriteFont>(m_Device.Get(), L"Fonts\\stats.spritefont");

	m_Data = std::vector<Vertex>(1000000);

	GameManager::Initialize();
	UpdateMapState();
	GameManager::SetWindowSize(Vector2f(width,height));



	return true;
}

void Graphics::RenderFrame()
{
	float bgColor[] = { 135.f / 255.0f,206.0f / 255.0f,250.0f / 255.0f };
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

	if (m_ShowPopUp)
	{
		ShowPopUp();
	}

	if (GameManager::getGameIndicator())
	{

		DrawGameOver();

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

	if (!m_VertexShader.Initialize(m_Device, L"..\\x64\\Debug\\vertexshader.cso", layout, numElements))
	{
		return false;
	}

	if (!m_PixelShader.Initialize(m_Device, L"..\\x64\\Debug\\pixelshader.cso"))
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



void Graphics::DrawGameOver()
{
	std::string player1Name = GameManager::GetPlayerName(1);
	std::string player2Name = GameManager::GetPlayerName(2);
	int player1Health = GameManager::GetPlayerHealth(1);
	int player2Health = GameManager::GetPlayerHealth(2);

	std::string gameOver = "GAME OVER";
	std::string winnerIs = "Winner is: ";

	if (player1Health <= 0)
	{
		winnerIs = winnerIs + player2Name;
		GameManager::AddScoreToPlayer(2, GameManager::GetPlayerScore(2) + 1);
	}
	else if (player2Health <= 0)
	{
		winnerIs = winnerIs + player1Name;
		GameManager::AddScoreToPlayer(1, GameManager::GetPlayerScore(1) + 1);
	}

	DrawTextOnScreen(gameOver, Vector2f(m_ViewWidth / 2.2, m_ViewHeight / 3.5));
	DrawTextOnScreen(winnerIs, Vector2f(m_ViewWidth / 2.2 - 35, m_ViewHeight / 3.5 + 30));

	m_ShowPopUp = true;
	
}

void Graphics::ShowPopUp()
{
	std::string player1Name = GameManager::GetPlayerName(1);
	std::string player2Name = GameManager::GetPlayerName(2);
	unsigned player1Score = GameManager::GetPlayerScore(1);
	unsigned player2Score = GameManager::GetPlayerScore(2);

	std::string scoreInfo = "Current score is:\n" + player1Name + " " + std::to_string(player1Score) + " : " + std::to_string(player2Score) + " " + player2Name + "\nDo you want rematch?";
	std::wstring wScore = std::wstring(scoreInfo.begin(), scoreInfo.end());
	LPCWSTR sw = wScore.c_str();

	ShowCursor(TRUE);

	int msgboxID = MessageBox(
		NULL,
		(LPCWSTR)sw,
		(LPCWSTR)L"Good Game Well Played",
		MB_YESNO | MB_DEFBUTTON1 | MB_ICONQUESTION
	);

	switch (msgboxID)
	{
	case IDYES:
		GameManager::RestartGameState();
		UpdateMapState();
		GameManager::AddScoreToPlayer(1, player1Score);
		GameManager::AddScoreToPlayer(2, player2Score);
		ShowCursor(FALSE);
		break;
	case IDNO:
		GameManager::ShutDown();
		exit(0);
		break;
	}

	m_ShowPopUp = false;

}

void Graphics::DrawStats()
{
	std::string player1Name = GameManager::GetPlayerName(1);
	std::string player2Name = GameManager::GetPlayerName(2);
	int player1Health = GameManager::GetPlayerHealth(1);
	int player2Health = GameManager::GetPlayerHealth(2);
	int currentPlayer = GameManager::GetCurrentPlayer();
	int playerAngle = static_cast<int>(GameManager::GetPlayerAngle() * 180 / PI);
	int playerPower = static_cast<int>(GameManager::GetPlayerPower() * 100);
	int leftMoves = GameManager::MovesLeft();
	std::string playerBarStr = "Player: ";
	std::string playerNameStr = "Name : ";
	std::string playerHealthStr = "Health: ";
	std::string playerAngleStr = "Angle: ";
	std::string playerPowerStr = "Power: ";
	std::string playerLeftMovesStr = "Moves Left: ";

	DrawTextOnScreen(playerBarStr, Vector2f(20, 20));
	DrawTextOnScreen(playerNameStr + player1Name, Vector2f(20, 40));
	DrawTextOnScreen(playerHealthStr + std::to_string(player1Health), Vector2f(20, 60));

	if (currentPlayer == 1)
	{
		DrawTextOnScreen(playerAngleStr + std::to_string(playerAngle), Vector2f(20, 80));
		DrawTextOnScreen(playerPowerStr + std::to_string(playerPower), Vector2f(20, 100));
		DrawTextOnScreen(playerLeftMovesStr + std::to_string(leftMoves), Vector2f(20, 120));
	}

	DrawTextOnScreen(playerBarStr, Vector2f(m_ViewWidth - 200, 20));
	DrawTextOnScreen(playerNameStr + player2Name, Vector2f(m_ViewWidth - 200, 40));
	DrawTextOnScreen(playerHealthStr + std::to_string(player2Health), Vector2f(m_ViewWidth - 200, 60));

	if (currentPlayer == 2)
	{
		DrawTextOnScreen(playerAngleStr + std::to_string(180 - playerAngle), Vector2f(m_ViewWidth - 200, 80));
		DrawTextOnScreen(playerPowerStr + std::to_string(playerPower), Vector2f(m_ViewWidth - 200, 100));
		DrawTextOnScreen(playerLeftMovesStr + std::to_string(leftMoves), Vector2f(m_ViewWidth - 200, 120));
	}
}

void Graphics::DrawShape(Vertex array[], unsigned arraySize)
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
	double power = GameManager::GetPlayerPower()*0.6;

	Vertex line[]
	{
		Vertex(playerPosition.GetX(),playerPosition.GetY() + playerSize.GetY()-0.03),
		Vertex(power*std::cos(angle) + playerPosition.GetX(),power*std::sin(angle) + playerPosition.GetY())
	};

	//Vertex line []
	//{
	//	Vertex(0+playerPosition.GetX(),playerPosition.GetY() + playerSize.GetY(), 1.0, 0.0, 0.0),
	//	/*Vertex(0.1*std::cos(angle) + playerPosition.GetX(),0.1*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
	//	Vertex(0.15*std::cos(angle) + playerPosition.GetX(),0.15*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
	//	Vertex(0.20*std::cos(angle) + playerPosition.GetX(),0.20*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
	//	Vertex(0.25*std::cos(angle) + playerPosition.GetX(),0.25*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
	//	Vertex(0.30*std::cos(angle) + playerPosition.GetX(),0.30*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
	//	Vertex(0.35*std::cos(angle) + playerPosition.GetX(),0.35*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
	//	Vertex(0.40*std::cos(angle) + playerPosition.GetX(),0.40*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
	//	Vertex(0.45*std::cos(angle) + playerPosition.GetX(),0.45*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0),
	//	*/Vertex(std::cos(angle)*power + playerPosition.GetX(),playerSize.GetY()+std::sin(angle)*power + playerPosition.GetY(), 1.0, 0.0, 0.0),
	//	//Vertex(0.55*std::cos(angle) + playerPosition.GetX(),0.55*std::sin(angle) + playerSize.GetY(), 1.0, 0.0, 0.0)
	//};
	DrawShape(line, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST, ARRAYSIZE(line));
	
}

void Graphics::DrawShape(Vertex array[], D3D11_PRIMITIVE_TOPOLOGY primitiveTopology, unsigned arraySize)
{
	m_DeviceContext->IASetPrimitiveTopology(primitiveTopology);
	DrawShape(array, arraySize);
}

void Graphics::DrawTextOnScreen(std::string text, Vector2f position)
{
	const std::wstring s = StringConverter::StringToWide(text);
	m_SpriteBatch->Begin();
	m_SpriteFont->DrawString(m_SpriteBatch.get(), s.data(), DirectX::XMFLOAT2(position.GetX(), position.GetY()), DirectX::Colors::Black);
	m_SpriteBatch->End();
}

void Graphics::DrawTank(int player)
{
	float scalex = 0.1 / 1.4f;
	float scaley = 0.1 / 0.5f;
	double playerAngle = 0;
	double cosAngle = std::cos(playerAngle);
	double sinAngle = std::sin(playerAngle);

	Vector2f playerPosition = GameManager::MovePlayer(player);
	Vector2f playerSize = GameManager::GetPlayerSize(player);
	float playerX = static_cast<float>(playerPosition.GetX());
	float playerY = static_cast<float>(playerPosition.GetY());

	float playerW = static_cast<float>(playerSize.GetX());
	float playerH = static_cast<float>(playerSize.GetY());

	float red = 0;
	float blue = 0;

	if (player == 1)
	{
		red = 1;
		blue = 0;
	}
	else if (player == 2)
	{
		blue = 1;
		red = 0;
	}
	else
	{
		ErrorLogger::Log("DrawTank color setup: invalid player");
		GameManager::ShutDown();
		exit(1);

	}

	float xB1 = -0.7f*scalex + playerX;
	float xB2 = 0.2f*scalex + playerX;
	float yB1 = 0.0f*scaley + playerY;
	float yB2 = 0.2f*scaley + playerY;

	Vertex base[]
	{
		Vertex(xB1, yB1, red, blue, 0),
		Vertex(xB1, yB2, red, blue, 0),
		Vertex(xB2, yB2, red, blue, 0),
		Vertex(xB1, yB1, red, blue, 0),
		Vertex(xB2, yB2, red, blue, 0),
		Vertex(xB2, yB1, red, blue, 0)
	};

	xB1 = -0.5f*scalex + playerX;
	xB2 = 0.0f*scalex + playerX;
	yB1 = 0.2f*scaley + playerY;
	yB2 = 0.4f*scaley + playerY;

	Vertex top[]
	{
		Vertex(xB1, yB1, red, blue, 0),
		Vertex(xB1, yB2, red, blue, 0),
		Vertex(xB2, yB2, red, blue, 0),
		Vertex(xB1, yB1, red, blue, 0),
		Vertex(xB2, yB2, red, blue, 0),
		Vertex(xB2, yB1, red, blue, 0)
	};

	/*{
		double angle = PI/4;
		int playerOnMove = GameManager::GetCurrentPlayer();
		if (player == playerOnMove)
		{
			angle = GameManager::GetPlayerAngle();
		}
	
		Vector2f playerPosition = GameManager::GetPlayerPosition(player);
		Vector2f playerSize = GameManager::GetPlayerSize(player);

		Vertex top[]
		{
			Vertex(playerPosition.GetX() - 0.02,playerPosition.GetY() + -0.03 + playerSize.GetY()),
			Vertex(0.06*std::cos(angle) + playerPosition.GetX(),0.06*std::sin(angle) + playerPosition.GetY()),
			Vertex(playerPosition.GetX() - 0.021,playerPosition.GetY() + -0.03 + playerSize.GetY()),
			Vertex(0.06*std::cos(angle) + playerPosition.GetX(),0.06*std::sin(angle) + playerPosition.GetY()),
			Vertex(playerPosition.GetX() - 0.019,playerPosition.GetY() + -0.03 + playerSize.GetY()),
			Vertex(0.06*std::cos(angle) + playerPosition.GetX(),0.06*std::sin(angle) + playerPosition.GetY())
		};

		DrawShape(top, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST, ARRAYSIZE(top));
	}*/


	// TODO: Rotation
		//Vertex(-0.7f*scalex*cosAngle - 0.0f*scaley*sinAngle +playerX,-0.7f*scalex*sinAngle + 0.0f*scaley*cosAngle + playerY),
	//	Vertex(-0.7f*scalex*cosAngle - 0.2f*scaley*sinAngle + playerX,-0.7f*scalex*sinAngle + 0.2f*scaley*cosAngle + playerY),
		//Vertex(0.2f*scalex*cosAngle - 0.2f*scaley*sinAngle + playerX,0.2f*scalex*sinAngle + 0.2f*scaley*cosAngle + playerY),
		//Vertex(-0.7f*scalex*cosAngle - 0.0f*scaley*sinAngle + playerX,-0.7f*scalex*sinAngle + 0.0f*scaley*cosAngle + playerY),
//		Vertex(0.2f*scalex*cosAngle - 0.2f*scaley*sinAngle + playerX,0.2f*scalex*sinAngle + 0.2f*scaley*cosAngle + playerY),
	//	Vertex(0.2f*scalex*cosAngle - 0.0f*scaley*sinAngle + playerX,0.2f*scalex*sinAngle + 0.0f*scaley*cosAngle + playerY),
		/*Vertex(-0.7f*scalex*cosAngle,0.2f*scaley*sinAngle ),
		Vertex(0.2f*scalex*cosAngle ,0.2f*scaley*sinAngle ),
		Vertex(-0.7f*scalex*cosAngle ,0.0f*scaley*sinAngle ),
		Vertex(0.2f*scalex*cosAngle,0.2f*scaley*sinAngle ),
		Vertex(0.2f*scalex*cosAngle,0.0f*scaley*sinAngle ),*/
		//Vertex(-0.5f*scalex*cosAngle - 0.2f*scaley*sinAngle + playerX,-0.5f*scalex*sinAngle + 0.2f*scaley*cosAngle + playerY),
		//Vertex(-0.5f*scalex*cosAngle - 0.4f*scaley*sinAngle + playerX,-0.5f*scalex*sinAngle + 0.4f*scaley*cosAngle + playerY),
		//Vertex(0.0f*scalex*cosAngle - 0.4f*scaley*sinAngle + playerX,0.0f*scalex*sinAngle + 0.4f*scaley*cosAngle + playerY),
	//	Vertex(-0.5f*scalex*cosAngle - 0.2f*scaley*sinAngle + playerX,-0.5f*scalex*sinAngle + 0.2f*scaley*cosAngle + playerY),
//		Vertex(0.0f*scalex*cosAngle - 0.4f*scaley*sinAngle + playerX,0.0f*scalex*sinAngle + 0.4f*scaley*cosAngle + playerY),
		//Vertex(0.0f*scalex*cosAngle - 0.2f*scaley*sinAngle + playerX,0.0f*scalex*sinAngle + 0.2f*scaley*cosAngle + playerY),
		//Vertex(-0.5f*scalex*cosAngle - 0.2f*scaley*sinAngle + playerX,-0.5f*scalex*sinAngle + 0.2f*scaley*cosAngle + playerY),

#ifdef DEBUG
	Vertex system[]
	{
		Vertex(-1,0),
		Vertex(1,0),
		Vertex(-0.5,-1),
		Vertex(-0.5,1)
	};
	DrawShape(system, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST, ARRAYSIZE(system));
#endif
	//TODO: Fix to drawing that are more appropriate for rotation

	double angle = GameManager::GetPlayerAngle();

	if (GameManager::GetCurrentPlayer() != player)
	{
		sinAngle = std::sin(PI/2);
		cosAngle = std::cos(PI/2);
	}

	else
	{
		sinAngle = std::sin(angle);
		cosAngle = std::cos(angle);
	}

	Vertex turret[]
	{
	//	/*Vertex(-0.2f*scalex + playerX,0.4f*scaley + playerY,0,0,0),
	//	Vertex(0.5f*scalex + playerX,0.4f*scaley + playerY,0,0,0),
	//	Vertex(-0.2f*scalex + playerX,0.35f*scaley + playerY,0,0,0),

	//	Vertex(-0.2f*scalex + playerX,0.35f*scaley + playerY,0,0,0),
	//	Vertex(0.5f*scalex + playerX,0.4f*scaley + playerY,0,0,0),
	//	Vertex(0.5f*scalex + playerX,0.35f*scaley + playerY,0,0,0),*/

		/*Vertex(-0.2f*scalex*cosAngle - 0.4f*scaley*sinAngle  + playerX,-0.2f*scalex*sinAngle + 0.4f*scaley*cosAngle  + playerY),
		Vertex(0.5f*scalex*cosAngle - 0.4f*scaley*sinAngle + playerX,0.5f*scalex*sinAngle + 0.4f*scaley*cosAngle  + playerY),
		Vertex(-0.2f*scalex*cosAngle - 0.35f*scaley*sinAngle + playerX,-0.2f*scalex*sinAngle + 0.35f*scaley*cosAngle + playerY),
		Vertex(-0.2f*scalex*cosAngle - 0.35f*scaley*sinAngle + playerX,-0.2f*scalex*sinAngle + 0.35f*scaley*cosAngle + playerY),
		Vertex(0.5f*scalex*cosAngle - 0.4f*scaley*sinAngle +  playerX,0.5f*scalex*sinAngle + 0.4f*scaley*cosAngle + playerY),
		Vertex(0.5f*scalex*cosAngle - 0.35f*scaley*sinAngle + playerX,0.5f*scalex*sinAngle + 0.35f*scaley*cosAngle + playerY)*/
		//Vertex(-0.5f*scalex*cosAngle - 0.2f*scaley*sinAngle + playerX,-0.5f*scalex*sinAngle + 0.2f*scaley*cosAngle + playerY),

		Vertex(playerX - 0.032,playerY+0.06),
		Vertex(cosAngle*0.1 - 0.032 +  playerX, sinAngle*0.1 + playerY),
		Vertex(playerX - 0.032 - 0.001,playerY + 0.001 +0.06),
		Vertex(cosAngle*0.1 - 0.032 + playerX-0.001, sinAngle*0.1 + playerY),
		Vertex(playerX - 0.032 - 0.002,playerY + 0.002 + 0.06),
		Vertex(cosAngle*0.1 - 0.032 + playerX - 0.002, sinAngle*0.1 + playerY),
		Vertex(playerX - 0.032 - 0.003,playerY + 0.003 + 0.06),
		Vertex(cosAngle*0.1 - 0.032 + playerX - 0.003, sinAngle*0.1 + playerY),
		Vertex(playerX - 0.032 - 0.004,playerY + 0.004 + 0.06),
		Vertex(cosAngle*0.1 - 0.032 + playerX - 0.004, sinAngle*0.1 + playerY),
		Vertex(playerX - 0.032 - 0.005,playerY + 0.005 + 0.06),
		Vertex(cosAngle*0.1 - 0.032 + playerX - 0.005, sinAngle*0.1 + playerY),
		Vertex(playerX - 0.032 - 0.006,playerY + 0.006 + 0.06),
		Vertex(cosAngle*0.1 - 0.032 + playerX - 0.006, sinAngle*0.1 + playerY),

		Vertex(playerX + 0.001,playerY + 0.001 + 0.06),
		Vertex(cosAngle*0.1 + playerX + 0.001, sinAngle*0.1 + playerY),
		Vertex(playerX + 0.002,playerY + 0.002 + 0.06),
		Vertex(cosAngle*0.1 + playerX + 0.002, sinAngle*0.1 + playerY),
		Vertex(playerX + 0.003,playerY + 0.003 + 0.06),
		Vertex(cosAngle*0.1 + playerX + 0.003, sinAngle*0.1 + playerY),
		Vertex(playerX + 0.004,playerY + 0.004 + 0.06),
		Vertex(cosAngle*0.1 + playerX + 0.004, sinAngle*0.1 + playerY),
		Vertex(playerX + 0.005,playerY + 0.005 + 0.06),
		Vertex(cosAngle*0.1 + playerX + 0.005, sinAngle*0.1 + playerY),
		Vertex(playerX + 0.006,playerY + 0.006 + 0.06),
		Vertex(cosAngle*0.1 + playerX + 0.006, sinAngle*0.1 + playerY)
		//Vertex(-0.2f*scalex*cosAngle - 0.35f*scaley*sinAngle + playerX,-0.2f*scalex*sinAngle + 0.35f*scaley*cosAngle + playerY),
		//Vertex(0.5f*scalex*cosAngle - 0.4f*scaley*sinAngle + playerX,0.5f*scalex*sinAngle + 0.4f*scaley*cosAngle + playerY),
		//Vertex(0.5f*scalex*cosAngle - 0.35f*scaley*sinAngle + playerX,0.5f*scalex*sinAngle + 0.35f*scaley*cosAngle + playerY)

	};

	DrawShape(base, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, ARRAYSIZE(base));
	DrawShape(top, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, ARRAYSIZE(top));
	DrawShape(turret, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST, ARRAYSIZE(turret));
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

	double x1 = x - w / 2.0f;
	double x2 = x + w / 2.0f;

	double y1 = y - h / 2.0f;
	double y2 = y + h / 2.0f;

	Vertex v[]
	{
		Vertex(x1, y1),
		Vertex(x1 ,y2),
		Vertex(x2 ,y2),

		Vertex(x1, y1),
		Vertex(x2,y2),
		Vertex(x2,y1),
	};

	DrawShape(v, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, ARRAYSIZE(v));
}