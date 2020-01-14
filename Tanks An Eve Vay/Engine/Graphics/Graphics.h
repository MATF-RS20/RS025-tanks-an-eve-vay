#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Shaders.h"
#include <SpriteFont.h>
#include <vector>
#include "Vertex.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")

//#define DEBUG
#define MAP_SIZE_250 (400000)
class Vector2f;

class Graphics
{
public:

	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	void DrawShape(Vertex array[], D3D11_PRIMITIVE_TOPOLOGY primitiveTopology,unsigned arraySize);

	void DrawTextOnScreen(std::string text, Vector2f position);

private:

	bool InitializeDirectX(HWND hwdn, int width, int height);
	bool InitializeShaders();
	
	void DrawTank(int player);
	void DrawMap();
	void DrawProjectile();
	void DrawShape(Vertex array[], unsigned arraySize);
	void DrawMouseIndicator();

	void UpdateMapState();

	void DrawGameOver();

	void ShowPopUp();

	void DrawStats();

	int m_ViewWidth;
	int m_ViewHeight;

	bool m_ShowPopUp = false;

	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;

	VertexShader m_VertexShader;
	PixelShader m_PixelShader;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	std::vector<Vertex> m_Data;
	unsigned m_DataSize = 0;

	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_SpriteFont;
};