#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Shaders.h"
#include "Vertex.h"
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")


class Graphics
{
public:

	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	void DrawShape(Vertex array[], D3D11_PRIMITIVE_TOPOLOGY primitiveTopology,unsigned arraySize);

protected:

	void DrawShape(Vertex array[], unsigned arraySize);
	void DrawTank(int player);
	void DrawMap();
	void DrawGridPart(int i, int j);

private:

	bool InitializeDirectX(HWND hwdn, int width, int height);
	bool InitializeShaders();

	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;

	VertexShader m_VertexShader;
	PixelShader m_PixelShader;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
};