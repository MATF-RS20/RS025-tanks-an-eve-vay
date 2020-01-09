#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Shaders.h"
#include <vector>
#include "Vertex.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")

#define DEBUG

class Graphics
{
public:

	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	void DrawShape(Vertex array[], D3D11_PRIMITIVE_TOPOLOGY primitiveTopology,unsigned arraySize);

private:

	bool InitializeDirectX(HWND hwdn, int width, int height);
	bool InitializeShaders();
	
	void DrawTank(int player);
	void DrawMap();
	void DrawGridPart(int i,int j);
	void DrawProjectile();
	void DrawShape(Vertex array[], unsigned arraySize);
	void DrawMouseIndicator();

	void UpdateMapState();

	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;

	VertexShader m_VertexShader;
	PixelShader m_PixelShader;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	std::vector<Vertex> m_Data;
	unsigned m_DataSize = 0;
};