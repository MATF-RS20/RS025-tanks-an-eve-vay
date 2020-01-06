#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex(){}
	Vertex(float x,float y)
		:pos(x,y),color(0,0,0) {}

	Vertex(float x, float y, float r, float g, float b)
		:pos(x, y), color(r, b, g)
	{

	}
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT3 color;
};